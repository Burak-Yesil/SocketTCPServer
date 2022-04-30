#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>


int main(int argc, char *argv[])
{

    if (argc != 2){
        perror("Error: Invalid number of argument");
        exit(1);
    }

    int inputtedPort = atoi(argv[1]);

    if (inputtedPort <= 1023){
        perror("Error: Port number must be greater than 1023");
        exit(1);
    }


    char *ip = "127.0.0.1"; // local ip address used because both files are on the same machine
    int port = htons(inputtedPort);        // Port number

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;
    
    void signal_handler(int signum){
        close(server_sock);
        close(client_sock);
        printf("\nSuccess: Closed server socket\n");
        exit(0);
    }
    signal(SIGINT, signal_handler);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    
    int true = 1;
    setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    
    
    if (server_sock < 0)
    {
        perror("Error: Socket error");
        exit(1);
    }
    printf("Success: TCP server socket created\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("Error: Bind Error");
        exit(1);
    }
    printf("Success: Binded to port: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    float total_sum = 0;
    addr_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);

    while (1)
    {
        //printf("Success: Client connected\n"); //Connected to the client socket       
        
        bzero(buffer, 1024); //Zeroing out the buffer
        
        recv(client_sock, buffer, sizeof(buffer), 0);
        //printf("Client: %f+%s\n", total_sum, buffer);
        
        float temp = strtod(buffer, NULL);
    

   	    total_sum = total_sum + temp;
        bzero(buffer, 1024);
        sprintf(buffer, "%f", total_sum);
        
        //printf("Server: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer),0);
        
        //printf("Success: Client disconnected\n");
        
    }

    return 0;
}