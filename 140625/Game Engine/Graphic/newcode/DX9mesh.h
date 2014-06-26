
#pragma once
#include "..\stdafx.h"  //빨간줄 방지용..실제 사용시 주석처리 바람

namespace graphic
{
	class cDX9Mesh
	{
	public:
		cDX9Mesh();
		~cDX9Mesh();
		bool ReadModelFile( const string &fileName );
		void Bind();

		inline cDX9VertexBuffer GetVertexBuffer() const { return m_vertexBuffer; };
		inline cDX9IndexBuffer GetIndexBuffer() const { return m_indexBuffer; };
		inline cDX9Material GetMaterialBuffer() const { return m_material; };
		inline cDX9Texture GetTextureBuffer() const { return m_texture; };
	protected:
		void ComputeNormals();
	private:
		cDX9VertexBuffer m_vertexBuffer;
		cDX9IndexBuffer m_indexBuffer;
		cDX9Material m_material;
		cDX9Texture m_texture;
	};
}