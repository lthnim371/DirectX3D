#pragma once

namespace framework
{
	class cStageMgr : public common::cSingleton<cStageMgr>
	{
	public:
		enum STAGE
		{
			MAIN, SELECT, INGAME,
		};

		cStageMgr();
		~cStageMgr();
		
		cStage* FindStage(const int nId);

		void SetStage(const int nId);
		void SetWindowHandle(const HWND& hWnd);
		
		cStage* GetStage() const;
		int GetCurrentStage() const;
		const HWND& GetWindowHandle() const;

	private:
		std::map<int, cStage*> m_list;
		cStage* m_stage;
		short m_currentStage;
		HWND m_hWnd;
	};
	
	inline cStageMgr* GetStageMgr() { return cStageMgr::Get(); }

	inline void cStageMgr::SetStage(const int nId) { m_stage = FindStage(nId); m_currentStage = nId; }
	inline void cStageMgr::SetWindowHandle(const HWND& hWnd) { m_hWnd = hWnd; }

	inline cStage* cStageMgr::GetStage() const { return m_stage; }
	inline int cStageMgr::GetCurrentStage() const { return m_currentStage; }
	inline const HWND& cStageMgr::GetWindowHandle() const { return m_hWnd; }
}