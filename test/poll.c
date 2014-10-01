#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/poll.h>

int main() {
	int ret,nread;
	char buffer[128];
	struct pollfd event;

	while(1) {
		memset(&event,0,sizeof(event));
		event.fd = 0;
		event.events = POLLIN;
		ret = poll(&event,1,2500);
		if(ret < 0){
			printf("poll error!\n");
			exit(1);
		} else if (0 == ret) {
			printf("Time out\n");
			continue;
		} else if(event.revents & POLLIN){
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
