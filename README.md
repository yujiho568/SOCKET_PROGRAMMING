## SOCKET PROGRAMMING

### SignalExample.c
CTRL+C 시그널 핸들러

### q2_server.c, q2_client.c 

#### 동시 동작 서버 : Echo 서버

하나의 서버에 여러 사용자가 동시에 접속하여 Echo 서버 기능을 사용할 수 있는 프로그램

SOCK_STREAM 소켓 및 AF_INET 연결을 사용

fork를 사용하여 client 별로 자식 프로세스를 1개씩 할당

하나의 server가 다수의 client를 동시에 서비스

server는 접속한 클라이언트 수를 int client_cnt 변수에 저장

새로운 client가 접속하면, client_cnt 변수를 증가시키고 출력

### q3_server.c, q3_client.c

#### select를 사용한 다중 채팅 프로그램

fork/thread 사용 X

## SOCKET PROGRAMMING2

### /sock_addr/server.c, /sock_addr/client.c

AF_UNIX/SOCK_STREAM 타입의 소켓을 사용하는 server-client 프로그램

client의 요청에 server가 accept로 반응

client는 사용자 터미널로부터 전달받은 문자열을 서버로 전송

server는 client로부터 받은 메시지를 터미널에 출력

### /sock_addr/q2_server.c, /sock_addr/q2_client.c

AF_INET/SOCK_STREAM 타입의 server-client 소켓 프로그램 : 동기형 1 대 1 채팅

동기화된 양방향 메시지 전송, 순서에 따라 채팅

### /MultiService/

AF_INET 도메인 STREAM 소켓 : 멀티 서비스

사용자의 입력 - 1 : 현재 시간을 문자열 형태로 client에게 전달

사용자의 입력 - 2 : 파일을 선택하여 server로 부터 다운로드 하여 현재 디렉토리에 저장

사용자의 입력 - 3 : server가 echo server로 동작

