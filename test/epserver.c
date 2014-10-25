#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

void setnonblocking(int fd) {
	fcntl(fd,F_SETFL,fcntl(fd,F_GETFD,0)|O_NONBLOCK);
}

int main() {
	int lsn_fd,con_fd;
	struct sockaddr_in svr_addr,clt_addr;
	struct epoll_event ev;
	struct epoll_event events[500];

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = htons(9001);

	lsn_fd = socket(AF_INET,SOCK_STREAM,0);
	int opt = 1;//set opt to reuse addr when server release port due to something wrong
	setsockopt(lsn_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));//
	setnonblocking(lsn_fd);
	bind(lsn_fd,(struct sockaddr*)&svr_addr,sizeof(svr_addr));
	listen(lsn_fd,500);
	int ep_fd = epoll_create(1);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = lsn_fd;
	epoll_ctl(ep_fd,EPOLL_CTL_ADD,lsn_fd,&ev);
	int nevts,n;
	int len = sizeof(clt_addr);
	char ch;
	for(;;) {
		nevts = epoll_wait(ep_fd,events,512,-1);
		/*printf("event size:%d\n",nevts);*/
		for(n=0;n<nevts;n++) {
			if(lsn_fd == events[n].data.fd) {// new connection coming
				con_fd = accept(lsn_fd,(struct sockaddr*)&clt_addr,&len);
				printf("accept from:%s:%d\n",inet_ntoa(clt_addr.sin_addr),
						ntohs(clt_addr.sin_port));
				setnonblocking(con_fd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = con_fd;
				epoll_ctl(ep_fd,EPOLL_CTL_ADD,con_fd,&ev);
				continue;
			}
			//else handle client request
			int nread = read(events[n].data.fd,&ch,1);
			if(0 < nread) {
				printf("Recv request:%c\n",ch);
				ch++;
				write(events[n].data.fd,&ch,1);	
			} else {
				if(nread < 0)
					printf("Read Error\n");
				int cfd = events[n].data.fd;
				epoll_ctl(ep_fd,EPOLL_CTL_DEL,cfd,NULL);
				close(cfd);
			}
		}
	}
	close(lsn_fd);
	close(ep_fd);
	return 0;
}
