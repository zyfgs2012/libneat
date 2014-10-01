#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
	int fd,ret,nread;
	struct epoll_event evts;
	char buffer[128];
	fd = epoll_create(1);

	evts.events = EPOLLIN;
	epoll_ctl(fd,EPOLL_CTL_ADD,0,&evts);
	while(1) {
		ret = epoll_wait(fd,&evts,2,2500);
		if(-1 == ret) {
			printf("Error occurs!\n");
			exit(1);
		} else if(0 == ret) {
			printf("Time out!\n");
			continue;
		} else if(evts.events & EPOLLIN){ 
			ioctl(0,FIONREAD,&nread);
			if(0 == nread) {
				printf("Read over!\n");
				break;
			}
			memset(buffer,0,128);
			nread = read(0,buffer,nread);
			printf("read %d from keyboard:%s\n",nread,buffer);	
		}
	}
	return 0;
}
