게임을 실행하기 위해서는 sever를 실행할 pc의 ip주소를 알아내어
stagemanager.cpp의 생성자의 m_ip를 바꿔주셔야 합니다..

만약 port도 변경하기를 원하신다면 마찬가지로
stagemanager.cpp의 생성자의 m_port를 바꿔주시고
sever.cpp의 winmain에서 LaunchServer함수의 첫번째 매개변수를 바꿔주시면 됩니다..
if (!network::LaunchServer(10000, g_svrSock))