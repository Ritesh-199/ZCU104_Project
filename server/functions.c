/**
 * @file      : functions.c
 * @author    : riteshthakur@bitstar-tech.com
 * @brief     : This file contains definitions of user-defined functions.
 */

/* Header files */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include "functions.h"

/** 
* @function name : send_connection_check()
* @parameter     : integer type socket descriptor
* @return        : not return value
* @brief         : this function is sending  array which confirms that connection is verified with client.

**/

void send_connection_check(int new_socket1) {
    printf("Connection Check Request\n");
    int data = 1, cam1 = 1, cam2 = 1, cam3 = 1, cam4 = 1, cam5 = 1, stD = 100, freeD = 50;

    unsigned char comm_data[] = {data, cam1, cam2, cam3, cam4, cam5, (unsigned char)stD, (unsigned char)freeD};
    size_t size = sizeof(comm_data), i;

    printf("Contents of comm_data: ");
    for (i = 0; i < size; ++i) {
        printf("%u ", comm_data[i]);
    }
    printf("\n");
    printf("Size of comm_data: %zu\n", size);
    send(new_socket1, &size, sizeof(size), 0);
    send(new_socket1, comm_data, size, 0);
}

/** 
* @function name : send_image()
* @parameter     : integer type socket descriptor , const char  pointer type filename and  integer type buffer
* @return        : not return value
* @brief         : this function is sending images from server side.

**/
void send_image(int new_socket1, const char *filename, int BUFFER_SIZE) {
    char buffer[BUFFER_SIZE];
    size_t total_sent = 0, sent, byte_read;
    long size1 = 0;
    bzero(buffer, sizeof(buffer));
    printf("Request to send image\n");

    FILE *image_file = fopen(filename, "rb");
    if (!image_file) {
        perror("Failed to open image file");
        return;
    }

    /* Calculate the size of the image */
    fseek(image_file, 0, SEEK_END);
    size1 = ftell(image_file);
    if (size1 == -1) {
        perror("ftell failed");
        fclose(image_file);
        return;
    }
    fseek(image_file, 0, SEEK_SET);

    /* Convert size to network byte order */
    uint32_t size_network_order = htonl((uint32_t)size1);

    /* Send the size of the image file */
    if (send(new_socket1, &size_network_order, sizeof(size_network_order), 0) == -1) {
        perror("Failed to send image size");
        fclose(image_file);
        return;
    }

    printf("Size of the image file: %ld bytes\n", size1);
    printf("Sending image data...\n");

    /* Read and send the image data in chunks */
    while ((byte_read = fread(buffer, 1, sizeof(buffer), image_file)) > 0) {
        sent = 0;
        while (sent < byte_read) {
            ssize_t n = send(new_socket1, buffer + sent, byte_read - sent, 0);
            if (n == -1) {
                perror("Failed to send image data");
                fclose(image_file);
                return;
            }
            sent += n;
            total_sent += n;
        }
    }

    printf("Total bytes sent: %zu\n", total_sent);
    printf("Image sent successfully\n");
    fclose(image_file);
}

