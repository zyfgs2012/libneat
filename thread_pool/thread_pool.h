#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <assert.h>
#include <pthread.h>
using namespace std;

class CJob {
	protected:
		//TODO:set Job id to distribute thread;atomic operations required
		void *m_pData; //Job data;
	public:
		CJob () {
			m_pData = NULL;
		}
		void SetJobData(void *pData) {
			m_pData = pData;
		}
		virtual void Run() = 0;
};

struct ThreadContext {
	pthread_t tWorkThread;
	int nThreadId;
	//volatile int nJobNum;
};

class CThreadPool {//TODO: change : detach some struct from class,so that ThreadFunc can assecc them. 
	private:
		static vector<CJob*> m_pJobVec; //TODO: sync queue : every thread owns queue; for granularity
		int m_nThreadNum;
		ThreadContext *m_pThreadContexts;
		static pthread_mutex_t m_ThreadMutex;
		static pthread_cond_t m_ThreadCond;
		int m_nJobNum;
		static bool m_bIsRunning;//!!! bad : shouldn't be static in logic
	public:
		CThreadPool(int nNum) {
			assert(nNum > 0);
			m_nThreadNum = nNum;
		}
		void Start(); // create recource
		static void* ThreadFunc(void* pData);
		void AddJob(CJob*);//Add Job
		int GetJobNum() {
			return (int)m_pJobVec.size();
		}
		void Stop();
};

#endif

