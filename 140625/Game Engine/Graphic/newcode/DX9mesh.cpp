#include "stdafx.h"
#include "DX9mesh.h"

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

	string buffer;
	while(true)
	{
		getline(fin, buffer);
		if( buffer.find( "MATERIAL 1") != string::npos )
			break;
	}

	float dif[4], amb[4], spec[4], emis[4], power;
	fin >> buffer >> dif[0] >> dif[1] >> dif[2] >> dif[3];
	fin >> buffer >> amb[0] >> amb[1] >> amb[2] >> amb[3];
	fin >> buffer >> spec[0] >> spec[1] >> spec[2] >> spec[3];
	fin >> buffer >> emis[0] >> emis[1] >> emis[2] >> emis[3];
	fin >> buffer >> power;

	m_material.Initial( D3DXCOLOR( amb[0], amb[1], amb[2], amb[3] ),
		D3DXCOLOR( dif[0], dif[1], dif[2], dif[3] ),
		D3DXCOLOR( spec[0], spec[1], spec[2], spec[3] ),
		D3DXCOLOR( emis[0], emis[1], emis[2], emis[3] ),
		power );

	//���� ���ϰ�� ������ ����
	getline(fin, buffer);
	getline(fin, buffer);
	getline(fin, buffer);

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


	// �ε��� ���� �ʱ�ȭ.
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

			// ���������� ����� ���ؼ� �Ҵ��Ѵ�.
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

//�ؽ��� ��ǥ ����

	//�ؽ��� �̸��� ���� ��������
	string tex;
	int numTex;
	fin >> tex >> eq >> numTex;

	//�ؽ��� ��ǥ �о����
	if (numTex > 0)
	{
		float fnum1, fnum2;
		vector<Vector3> texVertices(numTex);  //vector�� �����ڿ��� ������ �Է��ϸ� �ش� ������ŭ �迭�� ������
		//vector<Vector3> texVertices(numTex, 33);  //�ش� ������ŭ �迭�� �����ϸ鼭 33���� �ʱ�ȭ�Ѵٴ� �ǹ�(���� ����)
		for (int i = 0; i < numTex; i++)  //�ؽ��� ���ؽ� ������ŭ
		{
			fin >> fnum1 >> fnum2;
			texVertices[ i] = Vector3(fnum1, fnum2, 0);
		}

		//�ؽ��� ���̽� ��������
		string strTexFace;
		int numTexFace;
		fin >> strTexFace >> eq >> numTexFace;

		vector<int> texFaces;
		texFaces.reserve(numTexFace*3);  //���̽��� ��Ʈ���� 3���̹Ƿ� �����ϰ� �޸� ũ�� �÷��ֱ�
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
		//int : �������ؽ����۰���  vector<int> : ���ؽ������ε����� ������ �ִ� ������
		for (int i=0; i < m_vertexBuffer.GetVertexCount(); ++i)
		{
			vector<int> varray;
			varray.reserve(4);
			varray.push_back(i);  //i : �ε���
			vtxIdxMap[ i] = varray;  //i : key�� ,�׷��� �ش� Ű���� ������ ���� �ִ� ��
			//���� �ߺ��Ǵ� Ű���� ������ ���� �����ؼ� �־��ְ� �ߺ��Ǹ� ������.
		}

		// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� �����Ѵ�. 
		// ���ؽ� ������ uv ���� �ʱ�ȭ ���� �ʾҴٸ�, �ʱ�ȭ �Ѵ�.
		// ���ؽ��� �ϳ� �̻��� uv���� �����Ѵٸ�, ���ؽ��� �߰��ϰ�, �ε������۸� �����Ѵ�.
		for (int i=0; i < (int)texFaces.size(); ++i)  //���̽� �ε���(12)�� �ش��ϴ� ������(�ؽ��� ���ؽ��� ����Ű�� �ε��� = 3��)��ŭ(12 * 3)
		{
			const Vector3 tex = texVertices[ texFaces[ i]];  //�ؽ��� u,v��ǥ ��������
			const int vtxIdx = tempIdxBuff[ i];  //���ؽ����� �ε��� �����ͼ�
			//���ؽ������ε��� = �ؽ��Ĺ����ε��� //why? �Ȱ��� Ʈ���̾ޱ��� ��Ÿ���Ƿ�
			//�������ؽ����� �� �����ε������۴� ���ؽ����� �������� �߰��� �þ���� �𸣱⶧���� ������ �̸� ���� �ű⿡ �����͸� �־ �ؿ��� ���鶧 ���

			bool isFind = false;
			for (int k=0; k < (int)vtxIdxMap[ vtxIdx].size(); ++k)  //�ݺ��ϸ鼭 �ߺ��Ǵ� u,v��ǥ�� �ִ��� �˻�
			{
				const int subVtxIdx = vtxIdxMap[ vtxIdx][ k];  //�ؽ��Ĺ��� �ε���

				// �ؽ��� ��ǥ�� ���ؽ� ���ۿ� ����Ǿ� �ִٸ�, index buffer ���� �ش� vertex index ��
				// ���� �Ѵ�.
				if ((-100 == tempVtxBuff[ subVtxIdx].u) &&  //-100 : ���� u,v���̶� ��, �ʱⰪ���� ��
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

			// ���ؽ� ���ۿ� ���� uv ��ǥ���, �� ���ؽ��� ���ؽ����ۿ� �߰��Ѵ�.
			// �ε��� ���ۿ��� ���� �߰��� ���ؽ� �ε������� �ִ´�.
			if (!isFind)
			{
				graphic::sVertexNormTex v = tempVtxBuff[ vtxIdx];
				v.u = tex.x;
				v.v = tex.y;
				tempVtxBuff.push_back(v);
				const int newVtxIdx = tempVtxBuff.size()-1;  //-1 �ϴ°��� 0���� �����ϹǷ�(ex : 10����� 9�� �ش��ϹǷ�)
				vtxIdxMap[ vtxIdx].push_back( newVtxIdx );
				tempIdxBuff[ i] = newVtxIdx;
			}
		}
	}
//	vtxBuff.SetVertexCount( tempVtxBuff.size() );  //�ش� ���ؽ����� �߰��� u,v��ǥ�� ���� ���ؽ� ������ �þ���Ƿ� ���ؽ� ������(����) �缳��

	// ���ؽ� ���� ����.
	if( !m_vertexBuffer.Create( tempVtxBuff.size(), sizeof(graphic::sVertexNormTex), graphic::sVertexNormTex::FVF ))
		return false;
		
	// ���ؽ� ���� �ʱ�ȭ.
	graphic::sVertexNormTex* vertices = (graphic::sVertexNormTex*)m_vertexBuffer.Lock();
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	m_vertexBuffer.Unlock();

	// �ε��� ���� ����.
	if( !m_indexBuffer.Create( tempIdxCnt ))
		return false;
		
	WORD *indices = (WORD*)m_indexBuffer.Lock();
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	m_indexBuffer.Unlock();

	ComputeNormals();
	return true;
}

void cDX9Mesh::Bind()
{
	m_vertexBuffer.Bind();
	m_indexBuffer.Bind();
	m_material.Bind();
	//m_texture.Bind();
}

//protected
void cDX9Mesh::ComputeNormals()
{
		graphic::sVertexNormTex* vertices = (graphic::sVertexNormTex*)m_vertexBuffer.Lock();//( 0, sizeof(Vertex), (void**)&vertices, 0);
		WORD *indices = (WORD*)m_indexBuffer.Lock();//(0, 0, (void**)&indices, 0);

		for (int i=0; i < m_indexBuffer.GetFaceCount()*3; i+=3)
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

		for (int i=0; i < m_vertexBuffer.GetVertexCount(); ++i)
			vertices[ i].n.Normalize();

		m_vertexBuffer.Unlock();
		m_indexBuffer.Unlock();
}