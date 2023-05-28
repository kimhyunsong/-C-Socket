#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 200
int main(int argc, char *argv[])
{
	// 서버 주소, 클라이언트 주소
	//server: 인터넷 타입의 socket addr을 저장하는 구조체
	struct sockaddr_in server, remote;
	// socket number , 연결 요청이 들어올때마다 새로운 소켓 생성
	int request_sock, new_sock;
	
	int bytesread, addrlen;
	int i;
	char buf[BUFSIZE];
	if (argc !=2){
		(void) fprintf(stderr, "usage: %s port\n", argv[0]);
		exit(1);
	}
	//AF_INET : 인터넷을 사용
	//SOCK_STREAM : 소켓 생성
	if ((request_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <0)
	{
		perror("socket");
		exit(1);
	}
	memset((void *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	// INADDR_ANY == 0; 
	server.sin_addr.s_addr = INADDR_ANY;
	// 포트번호 
	// htons: host to network short 
	// 16비트의 integer 변수를 아키텍처에 따라 다를 수 있기 때문에 맞춰주어야함
	server.sin_port = htons((u_short)atoi(argv[1]));
	//소켓과 포트넘버, ipaddress를 매핑해줌.
	if(bind(request_sock, (struct sockaddr*)&server, sizeof(server)) <0){
		perror("bind");
		exit(1);
	}
	// listen 할 Queue 사이즈를 만듦
	if (listen(request_sock, SOMAXCONN) < 0){
		perror("listen");
		exit(1);
	}
	
	for (;;){
		addrlen = sizeof(remote);
		// 해당 클라이언트 주소와 변수 길이를 넘겨줌
		new_sock = accept(request_sock, (struct sockaddr*)&remote, &addrlen);
		if (new_sock < 0){
			perror("accept");
			exit(1);
		}
		printf("connection from host %s, port %d, socket %d\n", inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), new_sock);
	}
	// socket으로 들어오는 데이터 읽기
	for (;;){
		bytesread = read(new_sock, buf, sizeof(buf) - 1);
		if (bytesread <= 0) {
			close(new_sock);
			break;
		}
		buf[bytesread] = '\0';
		printf("%s: %d bytes from %d : %s\n", argv[0], bytesread, new_sock, buf);
		for (i = 0; i < bytesread; i++)
		{
			buf[i] = toupper(buf[i]);
		}
		if (write(new_sock, buf, bytesread)!=bytesread){
			perror("echo");
		}
	}
	
}
