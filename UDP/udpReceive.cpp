//
// Created by Genki Sasaki on 2019/05/20.
//

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <tuple>

#include "udpReceive.h"

udpReceive::udpReceive(int port, const char *address) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address);

    bind(sock, (struct sockaddr *)&addr, sizeof(addr));

    /* Non-blocking */
    int mode = 1; // if mode = 0 blocking
    ioctl(sock, FIONBIO, &mode);

}

int udpReceive::receive(toPC *pc, toMbed *mbed){
    toPC buf;
    int dif = 0;
    int count = 0;

    while (1) {
        retn_byte = recv(sock, &buf, sizeof(toPC), 0);
        if (retn_byte > 0) memcpy(pc, &buf, sizeof(toPC));

        dif = mbed->checkCount - pc->returnCount;
        if (dif < 1){
            break;
        } else if (count > 100) {
            break;
        }
        count++;
    }
    return retn_byte;
}

int udpReceive::receive(double *data, int size){
    double data_buf[size];

    retn_byte = recv(sock, data_buf, size, 0);
    if(retn_byte > 0)  data[0] = data_buf[0];

    return retn_byte;
}

void udpReceive::release(){
    close(sock);
}
