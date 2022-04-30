#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int main(int argc, char *argv[])
{

    if (argc != 2){
    	perror("Error: Need a number as an argument");
    	exit(1);
    }

    char *ip = "127.0.0.1"; // local ip address used because both files are on the same machine
    int port = htons(5565);        // Port number

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Error: Socket error");
        exit(1);
    }
    printf("Success: TCP server socket created\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Success: Connected to the server\n");


   
    printf("Client: %s\n", argv[1]);
    send(sock, argv[1], 1, 0);
    
    bzero(buffer, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    close(sock);
    printf("Success: Disconnected from the server\n");

    return 0;
}