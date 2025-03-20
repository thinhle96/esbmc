#include<assert.h>
#include <pthread.h>

int x=0;
void t1() {
	x++;
    assert(x==1);
}

void t2() {
	x--;
	assert(x==0);
}

void *thread1(void *arg)
{
    t1();
}

void *thread2(void *arg)
{
    t2();
}


int main(void){
	pthread_t id1, id2;
	pthread_create (&id1, NULL, thread1, NULL);
	pthread_create (&id2, NULL, thread2, NULL);
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	// assert(x==1);
	return 0;
}
