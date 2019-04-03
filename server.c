#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define PORT 2112

int main(const int argc, const char *argv[]) {
    // Create local variables
    struct sockaddr_in sin;
    const char message[] = "Hello from server!\0";
    char *buffer = (char *)malloc(sizeof(char) * 4096);
    if(buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    // Sock socket description
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = INADDR_ANY;

    // Bind information to port
    if(bind(sockfd, (const struct sockaddr *)&sin, sizeof(sin)) == -1) {
        fprintf(stderr, "Failed to bind to socket\n");
        exit(EXIT_FAILURE);
    }

    // Listen on socket
    if(listen(sockfd, 5) == -1) {
        fprintf(stderr, "Failed to setup listener\n");
        exit(EXIT_FAILURE);
    }

    int new_sockfd = 0;
    socklen_t len = sizeof(sin);

    // Accept new connection
    if((new_sockfd = accept(sockfd, (struct sockaddr *)&sin, &len)) == -1) {
        fprintf(stderr, "Failed to accept connection from client\n");
        exit(EXIT_FAILURE);
    }

    // Recieve data from client
    if(recv(new_sockfd, buffer, 4096, 0) == -1) {
        perror("Failed to recieve packet from client\n");
        exit(EXIT_FAILURE);
    }

    printf("Client: %s\n", buffer);

    // Send information to client
    if(send(new_sockfd, message, strlen(message), 0) == -1) {
        perror("Failed to send information to server\n");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", message);

    // Cleanup
    free(buffer);
    close(sockfd);
    close(new_sockfd);

    exit(EXIT_SUCCESS);
}