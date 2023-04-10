#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

//#include "sensor.h"

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

double peso_total = 0, pesagem1 = 0, pesagem2 = 0, tempo = 0;

int unidades = 0, unidades_esteira1 = 0, unidades_esteira2 = 0;

void atualiza_sensor1();
void atualiza_sensor2();
void atualiza_display();
void atualiza_pesagem();

int main(){
   pthread_t t1, t2, t3, t4;

   
   pthread_create(&t1, NULL, (void ) atualiza_sensor1, NULL); /*criando a thread para a sensor 1*/
   pthread_create(&t2, NULL, (void ) atualiza_sensor2, NULL); /*criando a thread para a sensor 2*/
   pthread_create(&t3, NULL, (void ) atualiza_display, NULL); /*criando a thread para atualizar o display*/
   pthread_create(&t4, NULL, (void ) atualiza_pesagem, NULL); /*criando a thread para Somar as pesagens quando atingir o limite de unidades*/

   //thread principal espera as threas criadas retornarem para encerrar o processo
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);
   pthread_join(t4, NULL);
   pthread_join(t3, NULL);

   return 0;
}

//função para a thread realizar a atualização de itens detectados pelo sensor 1
void atualiza_sensor1(){
   double lido = 5;
	   while(1){
      pthread_mutex_lock( &exclusao_mutua); //Trava a seção critica/
      pesagem1 += lido;//uart1_get_value() 
      unidades_esteira1++;
      unidades++;

      pthread_mutex_unlock( &exclusao_mutua);//destrava a seção critica/

      //Simula o intervalo de tempo que itens passão pelo sensor da esteira  1/
      sleep(2);
   }
}

//função para a thread realizar a atualização de itens detectados pelo sensor 2/
void atualiza_sensor2(){
   double lido = 2;
	   while(1){
      pthread_mutex_lock( &exclusao_mutua); //Trava a seção critica/
      pesagem2 += lido;//uart1_get_value() 
      unidades_esteira2++;
      unidades++;

      pthread_mutex_unlock( &exclusao_mutua);//destrava a seção critica/

      //Simula o intervalo de tempo que itens passão pelo sensor da esteira 2/
      sleep(1);
   }
}

//função para a thread realizar a soma das pesagens e caso atinja o limite de 500 unidades, imprime o peso total/
void atualiza_pesagem(){
   while(1){
      pthread_mutex_lock(&exclusao_mutua); //Trava a seção critica/
      if(unidades >= 500){
         peso_total = pesagem1 + pesagem2;
            printf ("Tempo total = %lf minutos\n", tempo/60);
         printf("Peso Total Processado: %lf Kg\n", peso_total);
         unidades = 0;
         pesagem1 = 0;
         pesagem2 = 0;
      }
      pthread_mutex_unlock(&exclusao_mutua); //destrava a seção critica/
   }
}

//função para a thread mostrar a atualização da quantidade de unidades que passaram por cada esteira/
void atualiza_display(){
   while(1){
      pthread_mutex_lock( &exclusao_mutua); //Trava a seção critica/
      printf("Itens lidos pela esteira 1: %d \n", unidades_esteira1);
      printf("Itens lidos pela esteira 2: %d \n", unidades_esteira2);

      pthread_mutex_unlock( &exclusao_mutua);//destrava a seção critica/

      tempo =+ 2;
      /* tempo para ataulizar o display*/
      sleep(2);            
   }
}
