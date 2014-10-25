#!/bin/sh
if [ 0 = 1 ]
then
	cd ../thread_pool
	g++ -c thread_pool.cpp
	cd ../test
	g++ -o client mt_client.cpp ../thread_pool/thread_pool.o -lpthread -g
	gcc -o server epserver.c
else
	g++ thread_pool_test.cpp ../thread_pool/thread_pool.o -lpthread
fi
