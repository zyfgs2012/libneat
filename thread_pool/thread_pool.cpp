#include "../common/commonop.h"
#include "thread_pool.h"

using namespace std;

vector<CJob*> CThreadPool::m_pJobVec;
pthread_mutex_t CThreadPool::m_ThreadMutex;
pthread_cond_t CThreadPool::m_ThreadCond;
bool CThreadPool::m_bIsRunning = true;

void CThreadPool::Start() {
	//m_bIsRunning = true;
	pthread_mutex_init(&m_ThreadMutex,NULL);
	pthread_cond_init(&m_ThreadCond,NULL);
	m_pThreadContexts = (ThreadContext*)ZeroMalloc(sizeof(ThreadContext)*m_nThreadNum);
	for(int i=0;i<m_nThreadNum;i++) {
		//m_pThreadContexts[i].nJobNum = 0;
		if( 0 != pthread_create(&m_pThreadContexts[i].tWorkThread,NULL,ThreadFunc,
					(void*)&m_pThreadContexts[i])) {
			fprintf(stderr,"%s\n","Create Thread Error");
		}
	}
}

void* CThreadPool::ThreadFunc(void *pData) {
	ThreadContext *pTrdCtx = (ThreadContext*)pData;
	pTrdCtx->nThreadId = get_tid();
	while(true) {
		pthread_mutex_lock(&m_ThreadMutex);
		while (m_pJobVec.size() == 0 && m_bIsRunning) {//!!! bad : continously waiting
			pthread_cond_wait(&m_ThreadCond,&m_ThreadMutex);
		}
		if(!m_bIsRunning) {
			pthread_mutex_unlock(&m_ThreadMutex);
			fprintf(stderr,"tid:%d exiting\n",pTrdCtx->nThreadId);
			pthread_exit(NULL);
		}
		fprintf(stderr,"tid:%d\n",pTrdCtx->nThreadId);
		vector<CJob*>::iterator iter = m_pJobVec.begin();
		CJob *pJob = NULL;
		if(iter != m_pJobVec.end()) {
			pJob = *iter;
			m_pJobVec.erase(iter);
		}
		pthread_mutex_unlock(&m_ThreadMutex);
		if(pJob) {
			pJob->Run();
			fprintf(stderr," over\n");
		}
	}
	return NULL;
}

void CThreadPool::AddJob(CJob *pJob) {
	pthread_mutex_lock(&m_ThreadMutex);
	m_pJobVec.push_back(pJob);
	pthread_mutex_unlock(&m_ThreadMutex);
	pthread_cond_signal(&m_ThreadCond);
}

void CThreadPool::Stop() {
	fprintf(stderr,"Stop all..\n");
	m_bIsRunning = false;
	pthread_cond_broadcast(&m_ThreadCond);
	for (int i=0;i<m_nThreadNum;i++) {
		pthread_join(m_pThreadContexts[i].tWorkThread,NULL);
	}
	NullFree(m_pThreadContexts);
	pthread_mutex_destroy(&m_ThreadMutex);
	pthread_cond_destroy(&m_ThreadCond);
}

