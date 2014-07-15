#pragma once


namespace framework
{

	class cEvent;
	class cEventHandler;
	typedef void (cEventHandler::*EventFunction)(cEvent &);
	//함수포인터 타입으로 형변환


	class cEventHandler
	{
	public:
		cEventHandler() {}
		virtual ~cEventHandler() {}

		bool EventConnect(cEventHandler *handler, EVENT::TYPE type, int id, EventFunction fn);
		bool EventDisconnect(cEventHandler *handler, EVENT::TYPE type, int id);


	protected:
		virtual bool SearchEventTable( cEvent &event );


	private:
		struct sEventEntry
		{
			EVENT::TYPE type;
			int id;
			cEventHandler *handler;
			EventFunction fn;
			bool operator==(const sEventEntry &rhs);
		};

		std::vector<sEventEntry> m_EventTable;
	};

}
