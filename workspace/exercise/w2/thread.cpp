#include <pthread.h>
#include <assert.h>

int x = 0, y = 0;

void* P1(void* arg) {
    for (int i = 1; i <= 10; i++) {
        x = i;
    }
}

void* P2(void* arg) {
    for (int i = 5; i <= 9; i++) {
        y = i;
    }

}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, P1, NULL);
    pthread_create(&t2, NULL, P2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    assert(x != y + 5); // Kiểm tra điều kiện x = y + 5 có xảy ra không
    return 0;
    
}
