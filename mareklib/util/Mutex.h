/*
 *  Mutex.h
 *  VimeoAwards
 *
 *  Created by Marek Bereza on 01/10/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#ifdef __APPLE_CC__
#include <pthread.h>
#include <semaphore.h>
#else
#include <Windows.h>
#include <process.h>
#endif

class Mutex {
public:
	
	Mutex() {
#ifdef __APPLE_CC__ 
		pthread_mutex_init(&mutex, NULL); 		
#else 
		InitializeCriticalSection(&critSec); 
#endif 
		
	}
	
	~Mutex() {
#ifdef __APPLE_CC__ 
		pthread_mutex_destroy(&mutex); 
#endif 
	}
	
#ifdef __APPLE_CC__
	pthread_mutex_t  mutex;
#else
	CRITICAL_SECTION  critSec;  	//same as a mutex
#endif
	
	void lock() {
		
#ifdef __APPLE_CC__ 
		pthread_mutex_lock(&mutex);		
#else 
		EnterCriticalSection(&critSec); 
#endif 
	}
	
	
	void unlock() {
#ifdef __APPLE_CC__ 
		pthread_mutex_unlock(&mutex);		
#else 
		LeaveCriticalSection(&critSec); 
#endif
	}
};
