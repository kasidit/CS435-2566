#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var;

void * foo(void * arg){
   pthread_t tid = pthread_self(); 
   printf("foo tid = %lu do something\n", (unsigned long)tid); 

   sleep(5); 

   printf("foo unlock mutex\n"); 
   pthread_mutex_unlock(&mutex_var);
   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t ntid;
   void *tret; 

   pthread_mutex_init(&mutex_var, NULL); 

   pthread_create(&ntid, NULL, foo, NULL);  
   pthread_detach(ntid); 
   printf("main thread do something \n"); 

   //pthread_join(ntid, &tret); 
   pthread_mutex_lock(&mutex_var); 
   printf("main thread wait here...\n"); 
   pthread_mutex_lock(&mutex_var); 
   pthread_mutex_unlock(&mutex_var); 
   printf("main thread continue...\n"); 

}
