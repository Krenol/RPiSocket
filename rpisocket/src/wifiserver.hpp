#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include "server.hpp"

#ifndef RPISOCKET_WIFISERVER_H
#define RPISOCKET_WIFISERVER_H

namespace rpisocket {
    class WiFiServer : public Server
    {
    private:
        mutable int sock_, newsock_, port_;
        mutable struct sockaddr_in server_, client_;

    public:
        WiFiServer(int port);
        ~WiFiServer();
        bool disconnect();
        bool connect();
        std::string readBytes();
        int writeBytes(const std::string& msg);
        std::string getConnectedClient() const;
    };
}
#endif
