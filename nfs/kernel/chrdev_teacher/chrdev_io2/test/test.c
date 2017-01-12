#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>


int count = 0;
void foo(void)
{
	char buf[1024 * 1024] = {1,2,3};
	printf("count = %d\n", ++count);
	foo();
}

int main(void)
{
	
	foo();
	//struct rlimit rlm;
	//getrlimit(RLIMIT_STACK, &rlm);
	//printf("rlim_cur = %d rlim_max = %d\n", rlm.rlim_cur, rlm.rlim_max);
	//rlm.rlim_cur = 50 * 1024 * 1024;
	//setrlimit(RLIMIT_STACK, &rlm);
	//foo();
}
