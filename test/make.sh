#!/bin/sh
cd ../thread_pool
g++ -c thread_pool.cpp
cd ../test/
g++ thread_pool_test.cpp ../thread_pool/thread_pool.o -lpthread
