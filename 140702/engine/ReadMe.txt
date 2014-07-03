01.viewer : 파일을 드래그하면 메시지처리에서 cModel* 의 create로 실행
02.model : 리소스매니저의 loadmodel 실행
03.resourcemanager : 중복되는 파일인지 검사
04. importer의 readrawmeshfile 실행
05.modelimpoter : 익스포트된 파일 버젼 확인하여 해당 버젼 readrawmeshfile 함수 호출
06. 각 해당 버젼에 따라 정보를 저장할 함수들 호출
07.model : 멤버객체에 값을 넣기전 clear로 미리 비워두기
08. 스키닝...(생략)
09. new rigidmesh를 생성할 때 상속을 따라 올라가며 mesh, node객체에도 값을 넣어줌
10. 스키닝이 아니라면 해당파일명으로 메쉬애니를 가지고 있는지 검사하여 만약 있다면 새로 넣어준다....?

question
1.vector타입에서 pushback하고 난 후 back()하면 데이터를 넣을 수 있다?
(back()하면 래퍼런스 반환?)

2.modelimpoter.cpp : ReadTextureCoordinate함수에서 텍스쳐 파일이름 로딩할 때
(특히 trim함수에 대하여)

3.resize와 reserve의 차이

4.map타입 객체인데
m_meshes[ fileName] = meshes;
m_anies[ fileName] = anies;
여기서 [ ]의 역할이 어떤 것인지?(key값? 그러면 리턴될때 세컨드값이 리턴되는?)

5.modelimpoter.cpp : id를 0번으로 한 것은?