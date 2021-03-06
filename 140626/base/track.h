#pragma once

namespace graphic
{
	class cTrack
	{
	public:  //디폴트함수
		cTrack(const tagRawAni& rawAni); 
		~cTrack();
	public:  //메인함수
		void Move( const int currFrame, OUT Matrix44& out );
		void KeyPos( const int currFrame, OUT Matrix44& out );
		void KeyRot( const int currFrame, OUT Matrix44& out );
	private:  //멤버객체
		const tagRawAni& m_rawAni;
		WORD m_keyIndexPos;
		WORD m_keyIndexRot;
//		float alpha;
	};
}