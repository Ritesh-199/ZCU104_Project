/**
 * @file      : functions.h
 * @author    : riteshthakur@bitstar-tech.com
 * @brief     : This is header files which contain user declared function .
 * @copyright :
 * 
 * */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void send_connection_check(int new_socket1);

void send_image(int new_socket1, const char *filename , int BUFFER_SIZE);

#endif
