#pragma once

namespace graphic
{

	class cMesh
	{
	public:
		cMesh();
		virtual ~cMesh();

		bool Create(const string &fileName);
		void Render(const Matrix44 &tm);
		void Move(const float elapseT);

		/*Matrix44 Rotation(const float elapseT);
		Matrix44 Move(const float elapseT);
		Matrix44 Scale(const float elapseT);*/
	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		cMaterial m_mtrl;
		cTexture m_tex;

		Matrix44 m_tm;

		cTrack* m_track;

//		tagRawAni m_rawani;
	};
}