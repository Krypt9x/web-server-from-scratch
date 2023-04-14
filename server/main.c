#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

char *writeHeader(int len){
    char *lenStr = malloc(32);
    snprintf(lenStr, 32, "%d", len);

    strcat(lenStr, "\n\n");
    char *header = malloc(strlen("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ") + strlen(lenStr) + 2);
    strcpy(header, "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ");
    strcat(header, lenStr);
    
    free(lenStr);
    return header;
}

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int new_socket;
    long valread;
    char *message = "hi from server";
    char *header = writeHeader(strlen(message));

    char *response = malloc(strlen(header) + strlen(message) + 1);
    strncpy(response, header, strlen(header));
    strncat(response, message, strlen(message));

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if(server_fd < 0){
        perror("failed to create socket");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("in bind");
    }
    if(listen(server_fd, 10) < 0){
        perror("in listen");
    }

    while(1){
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        
        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , response , strlen(response));
        printf("------------------Message sent-------------------\n");
        close(new_socket);
    }

    return 0;
}