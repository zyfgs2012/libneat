#include <sys/types.h>
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>
#include <cstring>
#include "../thread_pool/thread_pool.h"

//!!! ERROR: with Run().
char ch= 'A';

class MyJob: public CJob {
	public:
		void Run() {
			int sockfd;
			int len;
			char temp;
			struct sockaddr_in addr;
			sockfd = socket(AF_INET,SOCK_STREAM,0);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(9001);
			inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
			if(-1 == connect(sockfd,(struct sockaddr*)&addr,sizeof(addr))) {
				fprintf(stderr,"connect errno :%s\n",strerror(errno));
			}
			write(sockfd,&ch,1);
			read(sockfd,&temp,1);
			printf("char: %c\n",temp);
			close(sockfd);
		}
};

int main() {
	MyJob job;
	CThreadPool pool(5);
	pool.Start();
	for(int i=0;i<10;i++) {
		pool.AddJob(&job);
		//sleep(1);
	}
	while(true) {
		printf("still %d jobs\n",pool.GetJobNum());
		if(0 == pool.GetJobNum()) {
			pool.Stop();
			exit(0);
		}
		sleep(5);
	}
	return 0;
}
