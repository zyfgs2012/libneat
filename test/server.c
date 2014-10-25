#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	int svr_fd, clt_fd;
	int svr_len, clt_len;
	struct sockaddr_in svr_addr,clt_addr;
	svr_fd = socket(AF_INET, SOCK_STREAM, 0);
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(9001);
	/*server_len = sizeof(svr_addr);*/
	bind(svr_fd, (struct sockaddr*)&svr_addr, sizeof(svr_addr));
	listen(svr_fd,5);
	signal(SIGCHLD,SIG_IGN);
	while(1) {
		char ch;
		clt_len = sizeof(clt_addr);
		clt_fd = accept(svr_fd,(struct sockaddr*)&clt_addr,&clt_len);
		if(fork() == 0) { //child
			close(svr_fd);
			read(clt_fd,&ch,1);
			/*sleep(3);*/
			ch++;
			write(clt_fd,&ch,1);
			close(clt_fd);
			exit(0);
		} else {
			close(clt_fd);
		}
	}
	return 0;
}
