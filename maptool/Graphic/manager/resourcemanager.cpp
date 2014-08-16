
#include "stdafx.h"
#include "resourcemanager.h"
#include "../importer/modelimporter.h"
#include "../base/material.h"


using namespace graphic;


cResourceManager::cResourceManager() :
//	m_mediaDirectory("../media/")
//�߰�
	m_mediaDirectory("../../media/")
{

}

cResourceManager::~cResourceManager()
{
	Clear();
}


// load model file
sRawMeshGroup* cResourceManager::LoadModel( const string &fileName )
{
	if (sRawMeshGroup *data = FindModel(fileName))
		return data;

	sRawMeshGroup *meshes = new sRawMeshGroup;
	meshes->name = fileName;

	if (!importer::ReadRawMeshFile(fileName, *meshes))
	{
		delete meshes;
		return NULL;
	}

	// ���͸��� ����.
	BOOST_FOREACH (auto &mesh, meshes->meshes)
	{
		if (mesh.mtrlId >= 0)
		{
			mesh.mtrl = meshes->mtrls[ mesh.mtrlId];
		}
	}

	m_meshes[ fileName] = meshes;
	return meshes;
}


// �ִϸ��̼� ���� �ε�.
sRawAniGroup* cResourceManager::LoadAnimation( const string &fileName )
{
	if (sRawAniGroup *data = FindAnimation(fileName))
		return data;

	sRawAniGroup *anies = new sRawAniGroup;
	anies->name = fileName;

	if (!importer::ReadRawAnimationFile(fileName, *anies))
	{
		delete anies;
		return NULL;
	}

	m_anies[ fileName] = anies;
	return anies;
}


// find model data
sRawMeshGroup* cResourceManager::FindModel( const string &fileName )
{
	auto it = m_meshes.find(fileName);
	if (m_meshes.end() == it)
		return NULL; // not exist
	return it->second;
}


// find animation data
sRawAniGroup* cResourceManager::FindAnimation( const string &fileName )
{
	auto it = m_anies.find(fileName);
	if (m_anies.end() == it)
		return NULL; // not exist
	return it->second;
}


// �ؽ��� �ε�.
cTexture* cResourceManager::LoadTexture( const string &fileName, const bool isSizePow2 )//isSizePow2=true
{
	if (cTexture *p = FindTexture(fileName))
		return p;

	cTexture *texture = new cTexture();
	if (!texture->Create(fileName, isSizePow2))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!texture->Create(newPath, isSizePow2))
			{
				delete texture;
				return false;
			}
		}
	}
	m_textures[ fileName] = texture;
	return texture;
}

// �ؽ��� �ε�.
// fileName �� �ش��ϴ� ������ ���ٸ�, "../media/" + dirPath  ��ο��� ������ ã�´�.
cTexture* cResourceManager::LoadTexture( const string &dirPath, const string &fileName, const bool isSizePow2)
	//isSizePow2=true
{
	if (cTexture *p = FindTexture(fileName))
		return p;

	cTexture *texture = new cTexture();
	if (!texture->Create(fileName, isSizePow2))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory +dirPath+"/", newPath))
		{
			if (!texture->Create(newPath, isSizePow2))
			{
				delete texture;
				return false;
			}
		}
	}
	m_textures[ fileName] = texture;
	return texture;
}


// ���̴� �ε�.
cShader* cResourceManager::LoadShader( const string &fileName )
{
	if (cShader *p = FindShader(fileName))
		return p;

	cShader *shader = new cShader();
	if (!shader->Create(fileName, "TShader", false))
	{
		string newPath;
		if (common::FindFile(fileName, m_mediaDirectory, newPath))
		{
			if (!shader->Create(newPath, "TShader"))
			{
				delete shader;
				return NULL; // ���� ����.
			}
		}
		else
		{
			string msg = fileName + " ������ �������� �ʽ��ϴ�.";
			MessageBoxA( NULL, msg.c_str(), "ERROR", MB_OK);
		}
	}

	m_shaders[ fileName] = shader;
	return shader;
}


// �ؽ��� ã��.
cTexture* cResourceManager::FindTexture( const string &fileName )
{
	auto it = m_textures.find(fileName);
	if (m_textures.end() == it)
		return NULL; // not exist
	return it->second;
}


// ���̴� ã��.
cShader* cResourceManager::FindShader( const string &fileName )
{
	auto it = m_shaders.find(fileName);
	if (m_shaders.end() == it)
		return NULL; // not exist
	return it->second;
}


// remove all data
void cResourceManager::Clear()
{
	// remove raw mesh
	BOOST_FOREACH (auto kv, m_meshes)
	{
		delete kv.second;
	}
	m_meshes.clear();

	// remove texture
	BOOST_FOREACH (auto kv, m_textures)
	{
		delete kv.second;
	}
	m_textures.clear();

	// remove raw ani
	BOOST_FOREACH (auto kv, m_anies)
	{
		delete kv.second;
	}
	m_anies.clear();

	// remove shader
	BOOST_FOREACH (auto kv, m_shaders)
	{
		delete kv.second;
	}
	m_shaders.clear();
}


// ���� ������ �����Ѵ�.
RESOURCE_TYPE::TYPE cResourceManager::GetFileKind( const string &fileName )
{
	return importer::GetFileKind(fileName);
}