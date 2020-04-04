#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <string>
#include "server.hpp"

#ifndef RPISOCKET_WIFISERVER_H
#define RPISOCKET_WIFISERVER_H

namespace rpisocket {
    class WiFiServer
    {
    private:
        int sockfd, newsockfd, port;
        socklen_t clilen;
        struct sockaddr_in server;
        std::vector<struct sockaddr_in> clients;
        const int accept_key = 1234;

    public:
        WiFiServer(int port);
        ~WiFiServer();

        const std::vector<std::string>& getBuffer() const;
        void sendMsgToClient(sockaddr_in client, const std::string& msg) const;
        bool hasClients() const; //Server has connected clients
        std::vector<sockaddr_in>  getClients() const;
        void stopServer() const;
        void disconnectClient(sockaddr_in client) const;
    };
}
#endif
