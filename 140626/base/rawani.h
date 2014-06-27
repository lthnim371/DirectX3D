#pragma once

#include <vector>

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

	struct tagRawAni
	{
		string name;
		float start;
		float end;
		std::vector<tagKeyPos> pos;
		std::vector<tagKeyRot> rot;
		std::vector<tagKeyScale> scale;
	};
}