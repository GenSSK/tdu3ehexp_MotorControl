//
// Created by Genki Sasaki on 2019/05/20.
//

#ifndef UDP_CONTROL_LINUX_UDP_RECEIVE_H
#define UDP_CONTROL_LINUX_UDP_RECEIVE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "dataStruct.h"

class udpReceive {
public:
    udpReceive(int port, const char* address);
    int receive(toPC *,toMbed *);
    int receive(double *data, int size);
    void release();
private:
    int sock;
    struct sockaddr_in addr;
    int retn_byte;
};


#endif //UDP_CONTROL_LINUX_UDP_RECEIVE_H
