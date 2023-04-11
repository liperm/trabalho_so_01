//Servidor pipe (testado usando WSL)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#define ESTEIRA2_PATH "/tmp/esteira2"
#define ESTEIRA1_PATH "/tmp/esteira1"

int main()
{
    clock_t inicio, fim;
    float tempo_execucao;
    int flag = 0;
    int sockfd1,sockfd2, newsockfd1,newsockfd2, len,peso=0, cont=0;
    struct sockaddr_un local, remote;
    char buffer1[1024],buffer2[1024];


    
   
    // Create socket
    sockfd1 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd1 < 0)
    {
        perror("Falha em criar o pipe");
        return 1;
    }

    // Bind socket to local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, ESTEIRA1_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd1, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socketn esteira 1");
        close(sockfd1);
        return 1;
    }

    

    // Listen for connections
    if (listen(sockfd1, 5) < 0)
    {
        perror("Falha em escutar o socket esteira 1");
        close(sockfd1);
        return 1;
    }

    printf("Servidor Named pipe ouvindo em %s...\n", ESTEIRA1_PATH);

    // Accept connections
    memset(&remote, 0, sizeof(remote));
    len = sizeof(remote);
    newsockfd1 = accept(sockfd1, (struct sockaddr *)&remote, &len);
    if (newsockfd1 < 0)
    {
        perror("Falha em aceitar coneccao esteira 1");
        close(sockfd1);
        return 1;
    }

    printf("Cliente conectado!\n");


  

    
/////////////////////////////////////////////////////////////////////// iniciando pipe da esteira 2
    // Create socket
    sockfd2 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd2 < 0)
    {
        perror("Falha em criar o pipe esteira 2");
        return 1;
    }
    
    //Bind socket 2 para o local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, ESTEIRA2_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd2, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socket esteira 2");
        close(sockfd2);
        return 1;
    }

     // Listen for connections
    if (listen(sockfd2, 5) < 0)
    {
        perror("Falha em escutar o socket esteira 2");
        close(sockfd2);
        return 1;
    }

    printf("Servidor Named pipe ouvindo em %s...\n", ESTEIRA2_PATH);

    // Accept connections
    memset(&remote, 0, sizeof(remote));
    len = sizeof(remote);
    newsockfd2 = accept(sockfd2, (struct sockaddr *)&remote, &len);
    if (newsockfd2 < 0)
    {
        perror("Falha em aceitar coneccao esteira 2");
        close(sockfd1);
        return 1;
    }

    //printf("Cliente conectado!\n");
     inicio = clock(); // registra o tempo de início
      //laço onde os dados dos buffers serão recebidos e calculados
   while(!flag){
     // Read data from client
        if (read(newsockfd1, buffer1, sizeof(buffer1)) < 0)
        {
            perror("Falha em ler do socket esteira 1");
            close(newsockfd1);
            close(sockfd1);
            return 1;
        }
        if (read(newsockfd2, buffer2, sizeof(buffer2)) < 0)
        {
            perror("Falha em ler do socket esteira 2");
            close(newsockfd2);
            close(sockfd2);
            return 1;
        }
        system("clear");
        printf("numero de itens: %d\n", cont+1);

        if(cont>=9){
            printf("O peso total é : %d\n",peso);
            
            flag=1;
        }
        cont+=3;
        peso += atoi(buffer1)+atoi(buffer2);
        sleep(1);
        peso += atoi(buffer2);
        sleep(1);
        
   }

    // Close sockets and exit
    close(newsockfd1);
    close(sockfd1);
    close(newsockfd2);
    close(sockfd2);


    fim = clock(); // registra o tempo de fim

    tempo_execucao = (float)(fim - inicio)/CLOCKS_PER_SEC; // calcula o tempo de processamento em segundos

    printf("Tempo de processamento: %f segundos\n", tempo_execucao);

    return 0;

}