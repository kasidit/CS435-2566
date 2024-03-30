#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var; 
int global_var = 100; 

void * foo(void * arg){

   pthread_mutex_lock(&mutex_var);  
   global_var += 25;
   printf("foo() update global_var to %d\n", global_var); 
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

int main(void){

   pthread_t mtid, ntid[3];
   void *tret; 
    
   pthread_mutex_init(&mutex_var, NULL); 

   pthread_create(&ntid[0], NULL, foo, NULL);  
   pthread_create(&ntid[1], NULL, foo, NULL);  
   pthread_create(&ntid[2], NULL, foo, NULL);  

   pthread_mutex_lock(&mutex_var);  
   global_var += 75;
   printf("main() update global_var to %d\n", global_var); 
   pthread_mutex_unlock(&mutex_var);  

   pthread_join(ntid[0], &tret); 
   pthread_join(ntid[1], &tret); 
   pthread_join(ntid[2], &tret); 

}
