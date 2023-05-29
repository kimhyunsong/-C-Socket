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


int main(int argc, char *argv[])
{
  int sockid, nread, addrlen;
  struct sockaddr_in my_addr, client_addr;
  char msg[50];
  if(argc!=2){
    printf("%s myportid\n", argv[0]);
    return 0;
  }
  printf("Server: creating socket\n");
  if((sockid = socket(AF_INET, SOCK_DGRAM, 0))< 0)
  {
    printf("Server: socket error : %d\n", errno);
    exit(0);
  }
  printf("Server: binding my local socket\n");

  memset((char *)&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(atoi(argv[1]));
  if ((bind(sockid, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0))
  {
    printf("Server : bind fail : %d\n", errno);
    exit(0);
  }
  for (;;){
    printf("Server : starting blocking message read\n");
    addrlen = sizeof(client_addr);
    nread = recvfrom(sockid, msg, sizeof(msg) - 1, 0, (struct sockaddr *)&client_addr, ((socklen_t *)&addrlen));
    msg[nread] = '\0';
    printf("Server: return code from read is %d\n", nread);
    printf("msg from host %s, port %d, socket %d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port), sockid);
    if (nread >0) printf("Server : message is : %s\n", msg);
    for (int i = 0 ; i < nread; i++)
    {
      msg[i] = toupper(msg[i]);
      sendto(sockid, msg, strlen(msg), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
    }
  }

  close(sockid);
}