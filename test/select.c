#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char buffer[128];
	int res,nread;
	fd_set inputs,test_fds;
	struct timeval timeout;
	FD_ZERO(&inputs);
	FD_SET(0,&inputs);

	while(1){
		test_fds = inputs;
		timeout.tv_sec = 2;
		timeout.tv_usec = 500000;
		res = select(FD_SETSIZE,&test_fds,NULL,NULL,NULL);
		switch (res) {
			case 0:
				printf("Timeout\n");
				break;
			case -1:
				perror("Error in select\n");
				exit(1);
			default:
				if(FD_ISSET(0,&test_fds)) {
					ioctl(0,FIONREAD,&nread);
					if(0 == nread) {
						printf("input end!\n");
						exit(0);
					}
					memset(buffer,0,128);
					nread = read(0,buffer,nread);
					printf("read %d from keyboard: %s",nread,buffer);
				}
				break;
		}
	}
	return 0;
}

