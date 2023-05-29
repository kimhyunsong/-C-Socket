#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#define BUFSIZE 200


int main(int argc, char*argv[])
{
  int sockid, retcode, nread;
  struct sockaddr_in my_addr, server_addr;
  struct hostent *hostp;
  char msg[128];
  if (argc != 4){
    printf("%s myport serveraddr serverport\n", argv[0]);
    return 0;
  }

  printf("Client: creating socket\n");
  if((sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    printf("Client: socket failed : %d\n", errno); exit(0);
  }
  printf("Clinet: binding my local socket\n");

  memset((char*)&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(atoi(argv[1]));
  if ((bind(sockid, (struct sockaddr *)&my_addr, sizeof(my_addr)) <0))
  {
    printf("Client : bind fail: %d\n", errno);
    exit(0);
  }

  printf("Client: creating addr structure for server\n");
  bzero((char*) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(argv[2]);
  server_addr.sin_port = htons(atoi(argv[3]));

  printf("Client : initializing messgage and sending \n");
  sprintf(msg, "Hello world");
  retcode = sendto(sockid, msg, strlen(msg), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
  addrlen = sizeof(server_addr);
  nread = recvfrom(sockid, msg, sizeof(msg) - 1, 0, (struct sockaddr *)&server_addr, ((socklen_t *)&server_addr));
  printf("msg : %s", msg);
}