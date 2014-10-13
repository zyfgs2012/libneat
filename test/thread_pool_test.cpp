#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../thread_pool/thread_pool.h"

using namespace std;

class MyJob : public CJob {
	public:
		void Run() {
			printf("%s/n",(char*)m_pData);
			sleep(10);
		}
};

int main() {
	MyJob jobs;
	char tmp[45] = "zyf123";
	jobs.SetJobData((void*)tmp);
	CThreadPool pool(5);
	for(int i=0;i<10;i++) {
		pool.AddJob(&jobs);
	}
	while(true) {
		if(0 == pool.GetJobNum()) {
			pool.Stop();
			exit(0);
		}
		sleep(1);
	}
	return 0;
}
