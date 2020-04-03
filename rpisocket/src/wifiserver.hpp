#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <string>

#ifndef WIFISERVER_H
#define WIFISERVER_H

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

    std::vector<std::string> getBuffer();
    void sendMsgToClient(sockaddr_in client, std::string msg);
    bool hasClients(); //Server has connected clients
    std::vector<sockaddr_in>  getClients();
    void stopServer();
    void disconnectClient(sockaddr_in client);
};
#endif
