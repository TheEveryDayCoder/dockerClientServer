#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H
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

int construct_udp_to_send(int portNumber, struct sockaddr_in *server_addr);
//construct_udp_to_send
int containsWallet(char * msg, char **username);



int containsTXLIST(char * input); 
#endif
