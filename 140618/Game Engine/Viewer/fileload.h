
#pragma once
#include "stdafx.h"

class cFileLoad : public cSingleton<cFileLoad>
{
public:
	bool ReadModelFile( const string &fileName, graphic::cDX9VertexBuffer& vtxBuff, graphic::cDX9IndexBuffer& idxBuff );
}

bool cFileLoad::ReadModelFile(const string &fileName, graphic::cDX9VertexBuffer& vtxBuff, graphic::cDX9IndexBuffer& idxBuff )
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
	WORD tempCnt = 0;
	fin >> vtx >> eq >> tempCnt; 
	vtxBuff.SetVertexCount(tempCnt);

	if (vtxBuff.GetVertexCount() <= 0)
		return  false;

	vector<graphic::sVertexNormTex> tempVtxBuff;
	tempVtxBuff.reserve(vtxBuff.GetVertexCount() + vtxBuff.GetVertexCount()/2);

	float num1, num2, num3;
	for (int i = 0; i < vtxBuff.GetVertexCount(); i++)
	{
		fin >> num1 >> num2 >> num3;
		tempVtxBuff.push_back( graphic::sVertexNormTex(num1, num2, num3) );
	}


	// �ε��� ���� �ʱ�ȭ.
	string idx;
	tempCnt = 0;
	fin >> idx >> eq >> tempCnt;
	idxBuff.SetFaceCount(tempCnt);

	vector<int> tempIdxBuff;
	tempIdxBuff.reserve(idxBuff.GetFaceCount());

	if (idxBuff.GetFaceCount() > 0)
	{
		int num1, num2, num3;
		for (int i = 0; i < idxBuff.GetFaceCount()*3; i+=3)
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
		vector<int> vertCount(vtxBuff.GetVertexCount(), 0);
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

		for (int i=0; i < vtxBuff.GetVertexCount(); ++i)
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
		for (int i=0; i < vtxBuff.GetVertexCount(); ++i)
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
	vtxBuff.SetVertexCount( tempVtxBuff.size() );  //�ش� ���ؽ����� �߰��� u,v��ǥ�� ���� ���ؽ� ������ �þ���Ƿ� ���ؽ� ������(����) �缳��

	// ���ؽ� ���� ����.
	if( vtxBuff.Create( vtxBuff.GetVertexCount(), sizeof(graphic::sVertexNormTex), graphic::sVertexNormTex.FVF ))
		return false;
		
	// ���ؽ� ���� �ʱ�ȭ.
	graphic::sVertexNormTex* vertices = (graphic::sVertexNormTex*)vtxBuff.Lock();
	for (int i = 0; i < (int)tempVtxBuff.size(); i++)
		vertices[ i] = tempVtxBuff[ i];
	vtxBuff.Unlock();

	// �ε��� ���� ����.
	if( idxBuff.Create( idxBuff.GetFaceCount() ))
		return false;
		
	WORD *indices = idxBuff.Lock();
	for (int i = 0; i < (int)tempIdxBuff.size(); ++i)
		indices[ i] = tempIdxBuff[ i];
	idxBuff.UnLock();

//	ComputeNormals(vtxBuff, vtxSize, idxBuff, faceSize);
	return true;
}