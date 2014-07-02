#pragma once

#include "node.h"

namespace graphic
{

	class cMesh : public cNode
	{
	public:
		cMesh(const int id, const sRawMesh &raw);
		virtual ~cMesh();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;
		void RenderBBox();  //경계박스에 관한


	protected:
		bool m_isSkinned;
		cMaterial m_mtrl;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		//추후 텍스쳐 추가해야함
	};

}
