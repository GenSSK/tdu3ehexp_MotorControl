//
// Created by Genki Sasaki on 2019/05/20.
//

#ifndef UDP_CONTROL_LINUX_UDP_SEND_H
#define UDP_CONTROL_LINUX_UDP_SEND_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "dataStruct.h"

class udpSend {
public:
    udpSend(int port, const char* address);
    int send(toMbed *);
    int send(double *);
    void release();
private:
    int sock;
    struct sockaddr_in addr;
    int send_byte;
};

#endif //UDP_CONTROL_LINUX_UDP_SEND_H