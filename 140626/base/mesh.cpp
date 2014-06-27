
#include "base.h"
#include "mesh.h"
#include <fstream>
#include <vector>
#include <map>
using std::vector;
using std::map;

using namespace graphic;


cMesh::cMesh()
{
	m_mtrl.InitWhite();
}

cMesh::~cMesh()
{

}


bool cMesh::Create(const string &fileName)
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >>exporterVersion;
	if (exporterVersion != "EXPORTER_V4")
		return false;

	int vtxSize;

	string vtx, eq;
	fin >> vtx >> eq >> vtxSize;

	if (vtxSize <= 0)
		return  false;

	vector<sVertexNormTex> tempVtxBuff;
	tempVtxBuff.reserve(vtxSize + vtxSize/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxSize; i++)
	{
		fin >> num1 >> num2 >> num3;
		sVertexNormTex vtx;
		vtx.p = Vector3(num1, num2, num3);
		tempVtxBuff.push_back( vtx );
	}


	// 인덱스 버퍼 초기화.
	int faceSize;
	string idx;
	fin >> idx >> eq >> faceSize;

	vector<int> tempIdxBuff;
	tempIdxBuff.reserve(faceSize);

	if (faceSize > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < faceSize*3; i+=3)
		{
			fin >> num1 >> num2 >> num3;
			tempIdxBuff.push_back(num1);
			tempIdxBuff.push_back(num2);
			tempIdxBuff.push_back(num3);
		}
	}

	string norm;
	int numNormal;
	fin >> norm >> eq >> numNormal;

	if (numNormal > 0)
	{
		float num1, num2, num3;
		vector<int> vertCount(vtxSize, 0);
		for (int i = 0; i < numNormal; i++)
		{
			fin >> num1 >> num2 >> num3;
			Vector3 n(num1, num2, num3);

			// 법선벡터의 평균을 구해서 할당한다.
			for (int k=0; k < 3; ++k)
			{
				const int vtxIdx = tempIdxBuff[ i*3 + k];
				tempVtxBuff[ vtxIdx].n += n;
				++vertCount[ vtxIdx];
			}
		}

		for (int i=0; i < vtxSize; ++i)
		{
			tempVtxBuff[ i].n /= (float)vertCount[ i];
			tempVtxBuff[ i].n.Normalize();
		}
	}


	string tex;
	int numTex;
	fin >> tex >> eq >> numTex;

	if (numTex > 0)
	{
		float fnum1, fnum2;
		vector<Vector3> texVertices(numTex);
		for (int i = 0; i < numTex; i++)
		{
			fin >> fnum1 >> fnum2;
			texVertices[ i] = Vector3(fnum1, fnum2, 0);
		}

		string strTexFace;
		int numTexFace;
		fin >> strTexFace >> eq >> numTexFace;

		vector<int> texFaces;
		texFaces.reserve(numTexFace*3);
		if (numTexFace > 0)
		{
			int num1, num2, num3;
			for (int i=0; i < numTexFace; ++i)
			{
				fin >> num1 >> num2 >> num3;
				texFaces.push_back( num1 );
				texFaces.push_back( num2 );
				texFaces.push_back( num3 );
			}
		}

		map<int, vector<int> > vtxIdxMap; // vertex index, vertex index array
		for (int i=0; i < vtxSize; ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);
			vtxIdxMap[ i] = varray;
		}

		// 텍스쳐 좌표를 버텍스 버퍼에 저장한다. 
		// 버텍스 버퍼의 uv 값이 초기화 되지 않았다면, 초기화 한다.
		// 버텍스에 하나 이상의 uv값이 존재한다면, 버텍스를 추가하고, 인덱스버퍼를 수정한다.
		for (int i=0; i < (int)texFaces.size(); ++i)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];
			const int vtxIdx = tempIdxBuff[ i];

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];

				// 텍스쳐 좌표가 버텍스 버퍼에 저장되어 있다면, index buffer 값을 해당 vertex index 로
				// 설정 한다.
				if ((-100 == tempVtxBuff[ subVtxIdx].u) &&
					(-100 == tempVtxBuff[ subVtxIdx].v))
				{
					tempVtxBuff[ subVtxIdx].u = tex.x;
					tempVtxBuff[ subVtxIdx].v = tex.y;
					isFind = true;
					break;
				}
				else if ((tex.x == tempVtxBuff[ subVtxIdx].u) && 
					(tex.y == tempVtxBuff[ subVtxIdx].v))
				{
					tempIdxBuff[ i] = subVtxIdx;
					isFind = true;
					break;
				}
			}

			// 버텍스 버퍼에 없는 uv 좌표라면, 새 버텍스를 버텍스버퍼에 추가한다.
			// 인덱스 버퍼에도 새로 추가된 버텍스 인덱스값을 넣는다.
			if (!isFind)
			{
				sVertexNormTex v = tempVtxBuff[ vtxIdx];
				v.u = tex.x;
				v.v = tex.y;
				tempVtxBuff.push_back(v);
				const int newVtxIdx = tempVtxBuff.size()-1;
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				tempIdxBuff[ i] = newVtxIdx;
			}
		}
	}

	m_vtxBuff.Create( tempVtxBuff.size() , sizeof(sVertexNormTex), sVertexNormTex::FVF);

	// 버텍스 버퍼 초기화.
	sVertexNormTex* vertices = (sVertexNormTex*)m_vtxBuff.Lock();
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	m_vtxBuff.Unlock();

	m_idxBuff.Create( tempIdxBuff.size() );
	WORD *indices = (WORD*)m_idxBuff.Lock();
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	m_idxBuff.Unlock();

	//키프레임 저장
	string buffer, keyframe1, keyframe2;
	short numframe;
	float num4; 
	getline(fin, buffer);
	getline(fin, buffer);
	fin >> buffer >> eq >> keyframe1 >> keyframe2;
	m_rawani.start = (float)::atoi( keyframe1.c_str() );
	m_rawani.end = (float)::atoi( keyframe2.c_str() );

	//위치 저장
	fin >> buffer >> eq >> numframe;
	for(int i=0; i<numframe; ++i)
	{
		fin >> keyframe1 >> keyframe2 >> num1 >> num2 >> num3;
		tagKeyPos temp;
		temp.t = (float)::atoi( keyframe2.c_str() );
		temp.p = Vector3(num1, num2, num3);
		m_rawani.pos.push_back(temp);
	}

	//회전 저장
	fin >> buffer >> eq >> numframe;
	for(int i=0; i<numframe; ++i)
	{
		fin >> keyframe1 >> keyframe2 >> num1 >> num2 >> num3 >> num4;
		tagKeyRot temp;
		temp.t = (float)::atoi( keyframe2.c_str() );
		temp.q = Quaternion(num1, num2, num3, num4);
		m_rawani.rot.push_back(temp);
	}

	//크기 저장
	fin >> buffer >> eq >> numframe;
	for(int i=0; i<numframe; ++i)
	{
		fin >> keyframe1 >> keyframe2 >> num1 >> num2 >> num3;
		tagKeyScale temp;
		temp.t = (float)::atoi( keyframe2.c_str() );
		temp.s = Vector3(num1, num2, num3);
		m_rawani.scale.push_back(temp);
	}
	
	return true;
}


void cMesh::Render(const Matrix44 &tm)
{
	const Matrix44 mat = m_tm * tm;
	GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&mat);

	m_tex.Bind(0);
	m_mtrl.Bind();
	m_idxBuff.Bind();
	m_vtxBuff.Bind();
	GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_vtxBuff.GetVertexCount(), 
		0, m_idxBuff.GetFaceCount());
}


Matrix44 cMesh::Rotation(const float elapseT)
{
	Quaternion frame;
	static float frameR = 0.f;
	static short keyNumberR = 0;
	
	if(frameR >= 1.f)
	{
		frameR = 0.f;
		keyNumberR++;
		if( keyNumberR >= (int)m_rawani.rot.size() - 1 )
			keyNumberR = 0;
	}
	
	frameR = frameR +
	(float)(( m_rawani.rot[keyNumberR + 1].t
	- m_rawani.rot[keyNumberR].t )
	/ 2 ) * 0.01f;
	
	frame = m_rawani.rot[keyNumberR].q.Interpolate(
		m_rawani.rot[keyNumberR + 1].q,
		frameR);
	
	return frame.GetMatrix();
}

Matrix44 cMesh::Move(const float elapseT)
{
	Matrix44 frame;
	static float frameM = 0.f;
	static short keyNumberM = 0;
	
	if(frameM >= 1.f)
	{
		frameM = 0.f;
		keyNumberM++;
		if( keyNumberM >= (int)m_rawani.pos.size() - 1 )
			keyNumberM = 0;
	}
	
	frameM = frameM +
	(float)(( m_rawani.pos[keyNumberM + 1].t
	- m_rawani.pos[keyNumberM].t )
	/ 2 ) * 0.01f;
	

	frame.SetTranslate( m_rawani.pos[keyNumberM].p.Interpolate(
		m_rawani.pos[keyNumberM + 1].p,
		frameM) );

	return frame;
}

Matrix44 cMesh::Scale(const float elapseT)
{
	Matrix44 frame;
	static float frameS = 0.f;
	static short keyNumberS = 0;
	
	if(frameS >= 1.f)
	{
		frameS = 0.f;
		keyNumberS++;
		if( keyNumberS >= (int)m_rawani.scale.size() - 1 )
			keyNumberS = 0;
	}
	
	frameS = frameS +
	(float)(( m_rawani.scale[keyNumberS + 1].t
	- m_rawani.scale[keyNumberS].t )
	/ 2 ) * 0.01f;
	

	frame.SetScale( m_rawani.scale[keyNumberS].s.Interpolate(
		m_rawani.scale[keyNumberS + 1].s,
		frameS) );

	return frame;
}
