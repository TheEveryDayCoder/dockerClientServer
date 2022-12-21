#ifndef SERVER_M_H
#define SERVER_M_H
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <poll.h>


////////////////////////////////////////////////
// This function creates the TCP socket based //
// on th inport number                        //
////////////////////////////////////////////////
int parent_socket_des(const char * port_number);

//int construct_udp(const char * port_number,struct sockaddr_in server_addr);

#endif
