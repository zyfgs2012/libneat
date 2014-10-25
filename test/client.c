#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
	int sockfd;
	int len;
	struct sockaddr_in addr;
	char ch= 'A';
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9001);
	inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
	connect(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	write(sockfd,&ch,1);
	read(sockfd,&ch,1);
	/*write(sockfd,&ch,1);*/
	/*read(sockfd,&ch,1);*/
	printf("Char from server: %c\n",ch);
	close(sockfd);
	exit(0);
}
