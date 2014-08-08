#pragma once

namespace framework
{
	class cStage_Select : public cStage
	{
	public:
		cStage_Select();
		~cStage_Select();

		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

	private:
	};
}