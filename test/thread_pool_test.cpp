#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../thread_pool/thread_pool.h"

using namespace std;

class MyJob : public CJob {
	public:
		void Run() {
			printf("%s\n",(char*)m_pData);
			sleep(3);
		}
};

int main() {
	MyJob jobs;
	char tmp[45] = "zyf123";
	jobs.SetJobData((void*)tmp);
	CThreadPool pool(5);
	pool.Start();
	int k = 10;
	for(int i=0;i<k;i++) {
		pool.AddJob(&jobs);
	}
	printf("Add %d Jobs\n",k);
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
