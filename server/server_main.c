/**
 * @file      : server_main.c
 * @author    : riteshthakur@bitstar-tech.com
 * @brief     : This server performs image sharing functionality with client.
 */

/*Header files*/

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include "functions.h"

/*Macro definitions*/
#define PORT1 9070    
#define PORT2 9071   
#define BUFFER_SIZE 1024

/** 
* @function name : main()
* @parameter     : no parameter
* @return        : return 0 on success and negative  value  on failure
* @brief         : this main function which create socket ,make connection with client , connection check and image transfer.

**/

int main() {
    int server_fd1, server_fd2, new_socket1, new_socket2;
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in address1, address2;
    int opt = 1;
    int addrlen = sizeof(address1);

    /*Create socket file descriptors for port1 and port2*/
    if ((server_fd1 = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created for port1\n");

    if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created for port2\n");

    /*Set socket options to allow reuse of address and port for port1 and port2 */
    if (setsockopt(server_fd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Configure address structures for port1 and port2 */
    address1.sin_family = AF_INET;
    address1.sin_addr.s_addr = INADDR_ANY;
    address1.sin_port = htons(PORT1);

    address2.sin_family = AF_INET;
    address2.sin_addr.s_addr = INADDR_ANY;
    address2.sin_port = htons(PORT2);

    /* Bind socket to address and port for port1 and port2 */
    if (bind(server_fd1, (struct sockaddr *)&address1, sizeof(address1)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Binding done for port1\n");

    if (bind(server_fd2, (struct sockaddr *)&address2, sizeof(address2)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Binding done for port2\n");

    /* Listen for incoming connections on port1 and port2 */
    if (listen(server_fd1, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Port1 is in listen mode\n");

    if (listen(server_fd2, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Port2 is in listen mode\n");

    while (1) {
        /* Accept incoming connection on port1 */
        if ((new_socket1 = accept(server_fd1, (struct sockaddr *)&address1, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection established on port1\n");

        /* Accept incoming connection on port2 */
        if ((new_socket2 = accept(server_fd2, (struct sockaddr *)&address2, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection established on port2\n");

        while (1) {
            bzero(buffer, sizeof(buffer));
            ssize_t valread = recv(new_socket2, buffer, BUFFER_SIZE, 0);
            if (valread <= 0) {
                if (valread == 0) {
                    printf("Client disconnected.\n");
                } else {
                    perror("recv");
                }
                break;
            }
            buffer[valread] = '\0'; 
            printf("Message from client on port2: %s\n", buffer);
            int option = atoi(buffer);
            switch (option) {
                case 9000:
                    send_connection_check(new_socket1);
                    break;
                case 2000:
                    send_image(new_socket1, "image.jpg", BUFFER_SIZE);
                    break;
                case 1000:
                    printf("Client requested to disconnect.\n");
                    break;
                default:
                    printf("Invalid response from client side\n");
                    break;
            }
        }
        /* Close the sockets */
        close(new_socket1);
        close(new_socket2);
    }
    close(server_fd1);
    close(server_fd2);

    return 0;
}

