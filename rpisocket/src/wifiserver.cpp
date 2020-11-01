#include "wifiserver.hpp"
#include "socket_exception.hpp"
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iostream>
#include <string.h>

rpisocket::WiFiServer::WiFiServer(int port, int msg_size, char delimeter) : Server (msg_size, delimeter)
{
    int opt = 1;
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_ < 0){
        throw socket_exception("error opening socket!");
    }

    // Forcefully attaching socket to the port
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        throw socket_exception("error while attaching to port!");
    }

    this->port_ = port;
    server_.sin_family = AF_INET;
    server_.sin_addr.s_addr = INADDR_ANY;
    server_.sin_port = htons(port);

    //bind socket to the port
    if (bind(sock_, (struct sockaddr *) &server_, sizeof(server_)) < 0){
        throw socket_exception("error while binding the server!");
    }
}


rpisocket::WiFiServer::~WiFiServer() {
    //close the connection
    disconnect();
    close(sock_);
}

bool rpisocket::WiFiServer::disconnect(){
    close(newsock_);
}

bool rpisocket::WiFiServer::connect() {
    std::lock_guard<std::mutex> guard(mtx_);
    connected_ = false;
    listen(sock_ , 1);
     
    int c = sizeof(struct sockaddr_in);
    newsock_ = accept(sock_, (struct sockaddr *)&client_, (socklen_t*)&c);
    if (newsock_ > 0) {
        connected_ = true;
    }
    return connected_;
}

std::string rpisocket::WiFiServer::readBytes() {
    std::string msg;
    readBytes(msg, msg_size_);
    return msg;
}

int rpisocket::WiFiServer::writeBytes(const std::string& msg) {
    checkConnection();
    int status = -1;
    std::lock_guard<std::mutex> guard(mtx_);
    status = write(newsock_, msg.c_str(), msg.length());
    if(status == -1){
        connected_ = false;
        throwConnectionLost();
    }
    return status;
}

std::string rpisocket::WiFiServer::getConnectedClient() const {
    return inet_ntoa(client_.sin_addr);
}

std::string rpisocket::WiFiServer::getServerIp() {
    std::string ip;
    getServerIp(ip);
    return ip;
}


void rpisocket::WiFiServer::getConnectedClient(std::string& out) 
{
    out = inet_ntoa(client_.sin_addr);
}

void rpisocket::WiFiServer::readBytes(std::string& out) 
{
    readBytes(out, msg_size_);
}


void rpisocket::WiFiServer::readBytes(std::string& out, int buffer_lgth) {
    checkConnection();
    char buf[buffer_lgth] = { 0 };
    int bytes_read = 0;
    {
        std::lock_guard<std::mutex> guard(mtx_);
        bytes_read = read(newsock_, buf, sizeof(buf));
    }
    if(bytes_read > 0){
        out = (std::string)buf;
    }
    else {
        connected_ = false;
        throwConnectionLost();
    }
}

void rpisocket::WiFiServer::getServerIp(std::string& out) 
{
    //from https://stackoverflow.com/a/2283541
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "wlan0" */
    strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);
    ioctl(sock_, SIOCGIFADDR, &ifr);
    out = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}
