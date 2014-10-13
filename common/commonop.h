#ifndef COMMON_OP_H
#define COMMON_OP_H

#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

static inline void* ZeroMalloc (int nSize) {
	void *p = malloc(nSize);
	if(NULL != p) {
		memset(p,0,nSize);
		return p;
	}
	fprintf(stderr,"%s","Error:Malloc");
	return NULL;
}

static inline void NullFree(void *p) {
	free(p);
	p = NULL;
}

static inline int get_tid() {
	int tid = syscall(__NR_gettid);
	return tid;
}

#endif
