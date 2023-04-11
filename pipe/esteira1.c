//Cliente pipe - testado no WSL
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define ESTEIRA1_PATH "/tmp/esteira1"

int main()
{

    int sockfd, len, peso;
    struct sockaddr_un remote;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o socket");
        return 1;
    }

    // Connect to server
    memset(&remote, 0, sizeof(remote));
    remote.sun_family = AF_UNIX;
    strncpy(remote.sun_path, ESTEIRA1_PATH, sizeof(remote.sun_path) - 1);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(sockfd, (struct sockaddr *)&remote, len) < 0)
    {
        perror("Falha em conectar no servidor");
        close(sockfd);
        return 1;
    }

    printf("Conectado ao servidor!\n");

    while(1){
        // Send data to server
        
        
        sprintf(buffer,"%d",5);
        //fgets(buffer, sizeof(buffer), stdin);
        if (write(sockfd, buffer, strlen(buffer) + 1) < 0)
        {
            perror("Falha em escrever no socket");
            close(sockfd);
            return 1;
        }
        printf("Dado enviado ao servidor. %s\n",buffer);
        sleep(2);

    }

    

    
   

    // Close socket and exit
    close(sockfd);
    return 0;
}