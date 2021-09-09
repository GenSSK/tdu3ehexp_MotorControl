//
// Created by Genki Sasaki on 2019/05/20.
//

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "udpSend.h"

udpSend::udpSend(int port, const char *address) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}

int udpSend::send(toMbed *mbed){
    mbed->checkCount += 1;
    send_byte = sendto(sock, mbed, sizeof(toMbed), 0, (struct sockaddr *)&addr, sizeof(addr));
    return send_byte;
}

int udpSend::send(double *data){
    send_byte = sendto(sock, data, sizeof(data), 0, (struct sockaddr *)&addr, sizeof(addr));

    return send_byte;
}

void udpSend::release(){
    close(sock);
}