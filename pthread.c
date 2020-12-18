#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//#include <synch.h>

int n = 0; /* общие данные для потоков */
int m = 0;
int how_much = 0;

void *potok(void *count){
    for (int i = 0; i < how_much; i ++){
        n++;
    }
    pthread_exit(0);
}; /* потоковая функция */

void *potok1(void *count){
    for (int i = 0; i < how_much; i ++){
        static pthread_mutex_t Global_mutex;
        pthread_mutex_lock(&Global_mutex);

        n++;
        pthread_mutex_unlock(&Global_mutex);
    }
    pthread_exit(0);
}; /* потоковая функция */

void *potok2(void *count){
    static pthread_mutex_t Global_mutex;
    pthread_mutex_lock(&Global_mutex);
    for (int i = 0; i < how_much; i ++){
        n++;
    }
    pthread_mutex_unlock(&Global_mutex);

    pthread_exit(0);
}; /* потоковая функция */

void *potok3(void *result){
    *((int*)result) =  0;
    for (int i = 0; i < how_much; i ++){
        *((int*)result) += 1;
    }
    pthread_exit(0);
}; /* потоковая функция */

int main(int argc, char *argv[])
{
    m = atoi(argv[2]);
    how_much = atoi(argv[1])/m;

    pthread_t tid [m]; /* идентификатор потока */
    pthread_attr_t attr; /* отрибуты потока */

/* получаем дефолтные значения атрибутов */
    pthread_attr_init(&attr);

    struct timespec tm1, tm2;

    clock_gettime (CLOCK_REALTIME, &tm1);

    int Results[m];

/* создаем новый поток */
    for(int i = 0; i < m; i ++) {
        switch (atoi(argv[3])) {
            case 0:
                pthread_create(&tid[i], &attr, potok, 0);
                break;
            case 1:
                pthread_create(&tid[i], &attr, potok1, 0);
                break;
            case 2:
                pthread_create(&tid[i], &attr, potok2, 0);
                break;
            case 3:
                pthread_create(&tid[i], &attr, potok3, Results + i);
                break;
        }
    }

    for(int i = 0; i < m; i ++) {
        pthread_join(tid[i],NULL);
    }/* ждем завершения исполнения потока */

    if(atoi(argv[3]) == 3){
        for(int i = 0; i < m; i ++) {
            n += Results[i];
           // printf("%d\n", Results[i]);
        }
    }

    printf("count = %d\n ",n);

    clock_gettime (CLOCK_REALTIME, &tm2);
    int dt = (tm2.tv_sec - tm1.tv_sec) * 1000 +
             (tm2.tv_nsec - tm1.tv_nsec) / 1000000;
    int dts = dt / 1000, dtm = dt % 1000;

    printf("Затрачено время: %d с %d mс\n", dts, dtm);

    return 0;
}
