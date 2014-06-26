#pragma once

namespace graphic
{
	struct tagKeyPos
	{
		float t;
		Vector3 p;
	};

	struct tagKeyScale
	{
		float t;
		Vector3 s;
	};

	struct tagKeyRot
	{
		float t;
		Quaternion q;
	};

	struct tagRawAni  //회전 -> 이동 -> 크기
	{
		string name;
		float start;
		float end;
		vector<tagKeyPos> pos;
		vector<tagKeyRot> rot;
		vector<tagKeyScale> scale;
	public:

	};
}