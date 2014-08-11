#pragma once

namespace framework
{
//모든 스테이지들을 통합관리하는 매니저클래스
	class cStageMgr : public common::cSingleton<cStageMgr>
	{
	public:
		enum STAGE  //스테이지 명칭
		{
			MAIN, NETWORK_SELECT, NETWORK_LOADING, INGAME,
		};

		cStageMgr();  //모든 스테이지 생성 및 현재 스테이지를 처음 메인 스테이지로
		~cStageMgr();
		
		cStage* FindStage(const int nId);
		void Release();  //소켓닫기 (추후 스테이지 제거도 포함시키기)

		void SetStage(const int nId);  //현재 스테이지를 원하는 스테이지로 교체
		void SetWindowHandle(const HWND& hWnd);  //gameapp으로부터 윈도우핸들을 받아올때 사용
		bool SetSocket();  //네트워크 클라이언트로 접속
		
		cStage* GetStage() const;
		int GetCurrentStage() const;
		const HWND& GetWindowHandle() const;
		SOCKET& GetSocket();
		int GetPort();
		string& GetIpAddress();
	//	DWORD GetIpAddress();

	private:
		std::map<int, cStage*> m_list;  //해당 스테이지 객체 주소를 보관
		cStage* m_stage;  //현재 스테이지 주소
		short m_currentStage;  //현재 스테이지의 이름
		HWND m_hWnd;  //전역적으로 필요한 윈도우핸들
		SOCKET m_socket;  //전역적으로 필요한 소켓
	//	CIPAddressCtrl m_ip;
		string m_ip;  //네트워크 접속을 위한 ip주소
		int m_port;  //네트워크 접속을 위한 포트
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