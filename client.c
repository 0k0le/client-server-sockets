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
    // Define local variables
    struct sockaddr_in sin;
    const char message[] = "Hello from client\0";
    char *buffer = (char *)malloc(sizeof(char) * 4096);
    if(buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory on the heap\n");
        exit(EXIT_FAILURE);
    }

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        fprintf(stderr, "Failed to open UNIX socket\n");
        exit(EXIT_FAILURE);
    }

    // Define socket information
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    
    if(inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr) < 0) {
        fprintf(stderr, "Failed to set socket address\n");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if(connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
        fprintf(stderr, "Failed to connect to server\n");
        exit(EXIT_FAILURE);
    }

    // Send information to server
    if(send(sockfd, message, strlen(message), 0) == -1) {
        fprintf(stderr, "Failed to send packet to server\n");
        exit(EXIT_FAILURE);
    }

    printf("Message: \"%s\" sent to server\n", message);

    // Recieve information from server
    if(recv(sockfd, buffer, 4096, 0) == -1) {
        fprintf(stderr, "Failed to recieve incoming packets\n");
        exit(EXIT_FAILURE);
    }

    printf("Server Response \"%s\"\n", buffer);

    // Cleanup
    free(buffer);
    close(sockfd);

    exit(EXIT_SUCCESS);
}