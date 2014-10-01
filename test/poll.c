#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> /*文件控制*/
#include <sys/time.h> /*时间方面的函数*/
#include <errno.h> /*有关错误方面的宏*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/poll.h> //poll()

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
			nread = read(0,buffer,nread);
			if(0 == nread) {
				printf("Read over!\n");
				break;
			}
			printf("read %d from keyboard:%s\n",nread,buffer);
		}	
	}
	return 0;
}
