#pragma once

namespace framework
{
	class cStage_Ingame : public cStage
	{
	public:
		cStage_Ingame();
		~cStage_Ingame();

		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

	private:
	};
}