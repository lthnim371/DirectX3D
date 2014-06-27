#include "base.h"
#include "rawani.h"

using namespace graphic;

tagRawAni::tagRawAni()
	//: keyNumber(0)//, proportion(0.f)
{}
tagRawAni::~tagRawAni()
{}

//Matrix44 tagRawAni::AnimationMove()
//{
//	Matrix44 frame;
//	
//	if(proportion >= 1.f)
//	{
//		proportion = 0.f;
//		keyNumber++;
//		if( keyNumber >= (int)pos.size() - 1 )
//			keyNumber = 0;
//	}
//	
//	proportion = proportion +
//	(float)(( pos[keyNumber+1].t - pos[keyNumber].t )
//	/ 2 ) * 0.01f;
//	
//
//	frame.SetTranslate( pos[keyNumber].p.Interpolate(
//		pos[keyNumber+1].p, proportion) );
//
////	graphic::GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&frame);
//
//	return frame;
//}