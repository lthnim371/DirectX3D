#pragma once

namespace graphic
{
	class cTrack
	{
	public:  //����Ʈ�Լ�
		cTrack(const tagRawAni& rawAni); 
		~cTrack();
	public:  //�����Լ�
		void Move( const int currFrame, OUT Matrix44& out );
	private:  //�����ü
		const tagRawAni& m_rawAni;
		WORD m_IndexMove;
		float alpha;
	};
}