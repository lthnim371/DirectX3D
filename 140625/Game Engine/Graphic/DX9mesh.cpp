#include "stdafx.h"
#include "newcode\DX9mesh.h"

using namespace graphic;

cDX9Mesh::cDX9Mesh()
{
}
cDX9Mesh::~cDX9Mesh()
{
}

bool cDX9Mesh::ReadModelFile( const string &fileName )
{
	using namespace std;
	ifstream fin(fileName.c_str());
	if (!fin.is_open())
		return false;

	string exporterVersion;
	fin >>exporterVersion;
	if (exporterVersion != "EXPORTER_V1")
		return false;

	string vtx, eq;
	WORD tempVtxCnt = 0;
	fin >> vtx >> eq >> tempVtxCnt; 
//	vtxBuff.SetVertexCount(tempCnt);

	if (tempVtxCnt <= 0)
		return  false;

	vector<graphic::sVertexNormTex> tempVtxBuff;
	tempVtxBuff.reserve(tempVtxCnt + tempVtxCnt/2);

	float num1, num2, num3;
	for (int i = 0; i < tempVtxCnt; i++)
	{
		fin >> num1 >> num2 >> num3;
		graphic::sVertexNormTex tempBuff;
		tempBuff.p = Vector3(num1, num2, num3);
		tempVtxBuff.push_back( tempBuff );
	}


	// 인덱스 버퍼 초기화.
	string idx;
	WORD tempIdxCnt = 0;
	fin >> idx >> eq >> tempIdxCnt;
//	idxBuff.SetFaceCount(tempCnt);

	vector<int> tempIdxBuff;
	tempIdxBuff.reserve(tempIdxCnt);

	if (tempIdxCnt > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < tempIdxCnt*3; i+=3)
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
		vector<int> vertCount(tempVtxCnt, 0);
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

		for (int i=0; i < tempVtxCnt; ++i)
		{
			tempVtxBuff[ i].n /= (float)vertCount[ i];
			tempVtxBuff[ i].n.Normalize();
		}
	}

//텍스쳐 좌표 저장

	//텍스쳐 이름과 갯수 가져오기
	string tex;
	int numTex;
	fin >> tex >> eq >> numTex;

	//텍스쳐 좌표 읽어오기
	if (numTex > 0)
	{
		float fnum1, fnum2;
		vector<Vector3> texVertices(numTex);  //vector는 생성자에서 갯수를 입력하면 해당 갯수만큼 배열이 생성됨
		//vector<Vector3> texVertices(numTex, 33);  //해당 갯수만큼 배열을 생성하면서 33으로 초기화한다는 의미(변수 가능)
		for (int i = 0; i < numTex; i++)  //텍스쳐 버텍스 갯수만큼
		{
			fin >> fnum1 >> fnum2;
			texVertices[ i] = Vector3(fnum1, fnum2, 0);
		}

		//텍스쳐 페이스 가져오기
		string strTexFace;
		int numTexFace;
		fin >> strTexFace >> eq >> numTexFace;

		vector<int> texFaces;
		texFaces.reserve(numTexFace*3);  //페이스당 인트형이 3개이므로 안전하게 메모리 크기 늘려주기
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
		//int : 원본버텍스버퍼갯수  vector<int> : 버텍스버퍼인덱스가 가지고 있는 데이터
		for (int i=0; i < vtxBuff.GetVertexCount(); ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);  //i : 인덱스
			vtxIdxMap[ i] = varray;  //i : key값 ,그래서 해당 키값에 데이터 집어 넣는 중
			//맵은 중복되는 키값이 없으면 새로 생성해서 넣어주고 중복되면 덮어씌운다.
		}

		// 텍스쳐 좌표를 버텍스 버퍼에 저장한다. 
		// 버텍스 버퍼의 uv 값이 초기화 되지 않았다면, 초기화 한다.
		// 버텍스에 하나 이상의 uv값이 존재한다면, 버텍스를 추가하고, 인덱스버퍼를 수정한다.
		for (int i=0; i < (int)texFaces.size(); ++i)  //페이스 인덱스(12)에 해당하는 데이터(텍스쳐 버텍스를 가리키는 인덱스 = 3개)만큼(12 * 3)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];  //텍스쳐 u,v좌표 가져오기
			const int vtxIdx = tempIdxBuff[ i];  //버텍스버퍼 인덱스 가져와서
			//버텍스버퍼인덱스 = 텍스쳐버퍼인덱스 //why? 똑같은 트라이앵글을 나타내므로
			//템프버텍스버퍼 및 템프인덱스버퍼는 버텍스버퍼 생성전에 추가로 늘어날지도 모르기때문에 그전에 미리 만들어서 거기에 데이터를 넣어서 밑에서 만들때 사용

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)  //반복하면서 중복되는 u,v좌표가 있는지 검사
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];  //텍스쳐버퍼 인덱스

				// 텍스쳐 좌표가 버텍스 버퍼에 저장되어 있다면, index buffer 값을 해당 vertex index 로
				// 설정 한다.
				if ((-100 == tempVtxBuff[ subVtxIdx].u) &&  //-100 : 최초 u,v값이라서 즉, 초기값인지 비교
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
				graphic::sVertexNormTex v = tempVtxBuff[ vtxIdx];
				v.u = tex.x;
				v.v = tex.y;
				tempVtxBuff.push_back(v);
				const int newVtxIdx = tempVtxBuff.size()-1;  //-1 하는것은 0부터 시작하므로(ex : 10개라면 9에 해당하므로)
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				tempIdxBuff[ i] = newVtxIdx;
			}
		}
	}
//	vtxBuff.SetVertexCount( tempVtxBuff.size() );  //해당 버텍스마다 추가된 u,v좌표로 인해 버텍스 갯수가 늘어났으므로 버텍스 사이즈(갯수) 재설정

	// 버텍스 버퍼 생성.
	if( !vtxBuff.Create( tempVtxBuff.size(), sizeof(graphic::sVertexNormTex), graphic::sVertexNormTex::FVF ))
		return false;
		
	// 버텍스 버퍼 초기화.
	graphic::sVertexNormTex* vertices = (graphic::sVertexNormTex*)vtxBuff.Lock();
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	vtxBuff.Unlock();

	// 인덱스 버퍼 생성.
	if( !idxBuff.Create( tempIdxCnt ))
		return false;
		
	WORD *indices = (WORD*)idxBuff.Lock();
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	idxBuff.Unlock();

	ComputeNormals(vtxBuff, idxBuff);
	return true;
}

void cDX9Mesh::ComputeNormals()
{
			graphic::sVertexNormTex* vertices = (graphic::sVertexNormTex*)vtxBuff.Lock();//( 0, sizeof(Vertex), (void**)&vertices, 0);
		WORD *indices = (WORD*)idxBuff.Lock();//(0, 0, (void**)&indices, 0);

		for (int i=0; i < idxBuff.GetFaceCount()*3; i+=3)
		{
			Vector3 p1 = vertices[ indices[ i]].p;
			Vector3 p2 = vertices[ indices[ i+1]].p;
			Vector3 p3 = vertices[ indices[ i+2]].p;

			Vector3 v1 = p2 - p1;
			Vector3 v2 = p3 - p1;
			v1.Normalize();
			v2.Normalize();
			Vector3 n = v1.CrossProduct(v2);
			n.Normalize();

			vertices[ indices[ i]].n += n;
			vertices[ indices[ i+1]].n += n;
			vertices[ indices[ i+2]].n += n;
		}

		for (int i=0; i < vtxBuff.GetVertexCount(); ++i)
			vertices[ i].n.Normalize();

		vtxBuff.Unlock();
		idxBuff.Unlock();
}