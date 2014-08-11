#pragma once

namespace framework
{
	class cStageMgr : public common::cSingleton<cStageMgr>
	{
	public:
		enum STAGE
		{
			MAIN, NETWORK_SELECT, NETWORK_LOADING, INGAME,
		};

		cStageMgr();
		~cStageMgr();
		
		cStage* FindStage(const int nId);
		void Release();

		void SetStage(const int nId);
		void SetWindowHandle(const HWND& hWnd);
		bool SetSocket();
		
		cStage* GetStage() const;
		int GetCurrentStage() const;
		const HWND& GetWindowHandle() const;
		SOCKET& GetSocket();
		int GetPort();
		string& GetIpAddress();
	//	DWORD GetIpAddress();

	private:
		std::map<int, cStage*> m_list;
		cStage* m_stage;
		short m_currentStage;
		HWND m_hWnd;
		SOCKET m_socket;
	//	CIPAddressCtrl m_ip;
		string m_ip;
		int m_port;
	};
	
	inline cStageMgr* GetStageMgr() { return cStageMgr::Get(); }

	inline void cStageMgr::SetStage(const int nId) { m_stage = FindStage(nId); m_currentStage = nId; }
	inline void cStageMgr::SetWindowHandle(const HWND& hWnd) { m_hWnd = hWnd; }

	inline cStage* cStageMgr::GetStage() const { return m_stage; }
	inline int cStageMgr::GetCurrentStage() const { return m_currentStage; }
	inline const HWND& cStageMgr::GetWindowHandle() const { return m_hWnd; }
	inline SOCKET& cStageMgr::GetSocket() { return m_socket; }
	inline int cStageMgr::GetPort() { return m_port; }
	inline string& cStageMgr::GetIpAddress() { return m_ip; }
//	inline DWORD cStageMgr::GetIpAddress() { DWORD address; m_ip.GetAddress(address); return address; }
}