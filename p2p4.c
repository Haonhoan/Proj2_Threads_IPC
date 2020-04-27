#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

pthread_t threadmain, thread0, thread1, thread2;

void sigint_handler()
{
	printf("Signal number %d\n", SIGINT);
	printf("ThreadID %ld\n", pthread_self());
	signal(SIGINT, SIG_DFL);
}

void sigstop_handler()
{
	printf("Signal number %d\n", SIGSTOP);
	printf("ThreadID %ld\n", pthread_self());
	signal(SIGSTOP, SIG_DFL);
}

void sigill_handler()
{
	printf("Signal number %d\n", SIGILL);
	printf("ThreadID %ld\n", pthread_self());
	signal(SIGILL, SIG_DFL);
}

void func1(void *arg)
{
	signal(SIGINT, sigint_handler);
	printf("First thread here\n");
	//line of code below used to test Part 2 Q2	
	//pthread_kill(threadmain, SIGILL);
	
	int sum = 0;
	for(int i = 0; i < (1000*pthread_self()); i++){
		sum +=i;
	}
	
	sleep(10);
	pthread_exit(NULL);
}

void func2(void *arg)
{
	signal(SIGSTOP, sigstop_handler);
	printf("Second thread here\n");

	int sum = 0;
	for(int i = 0; i < (1000*pthread_self()); i++){
		sum +=i;
	}
	
	sleep(10);
	pthread_exit(NULL);
}

void func3(void *arg)
{
	signal(SIGILL, sigill_handler);
	printf("Third thread here\n");
	
	int sum = 0;
	for(int i = 0; i < (1000*pthread_self()); i++){
		sum +=i;
	}
	
	sleep(10);
	pthread_exit(NULL);
}


int main()
{
	sigset_t set;
	
	threadmain = pthread_self();	

	//create set of signals to block and create thread
	//SIGINT
	sigemptyset(&set);
	sigaddset(&set, SIGSTOP);
	sigaddset(&set, SIGILL);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	printf("SIGINT set\n");
	pthread_create(&thread0, NULL, (void*)func1, NULL);
	
	//SIGSTOP
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGILL);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	printf("SIGSTOP set\n");
	pthread_create(&thread1, NULL, (void*)func2, NULL);
	
	//SIGILL
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGSTOP);
	pthread_sigmask(SIG_SETMASK, &set, NULL);
	printf("SIGILL set\n");
	pthread_create(&thread2, NULL, (void*)func3, NULL);

	sleep(10);

	//join threads
	pthread_join(thread0, NULL);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	//unblock signals
	pthread_sigmask(SIG_UNBLOCK, &set, NULL);

	printf("Original thread\n");
	
	signal(SIGINT, SIG_DFL);
	signal(SIGSTOP, SIG_DFL);
	signal(SIGILL, SIG_DFL);

	sleep(5);

	printf("End\n");
}
