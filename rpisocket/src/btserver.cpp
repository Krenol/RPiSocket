#include "btserver.hpp"
#include "socket_exception.hpp"
#include <iostream>

rpisocket::BTServer::BTServer(int msg_size, const std::string& delimiter)  : Server (msg_size, delimiter)
{
    opt_ = sizeof(client_address_);
    bdaddr_t tmp = {0,0,0,0,0,0}; //define empty bt address

    // allocate socket
    sock_ = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    local_address_.rc_family = AF_BLUETOOTH;
    local_address_.rc_bdaddr = tmp;
    local_address_.rc_channel = (uint8_t) 1;
    bind(sock_,(struct sockaddr*)&local_address_, sizeof(local_address_));
}

rpisocket::BTServer::~BTServer() 
{
    shutdown(sock_, 2);
    //close the connection
    disconnect();
    close(sock_);
}

bool rpisocket::BTServer::disconnect(){
    close(client_);
}

bool rpisocket::BTServer::connect()
{
    connected_ = false;
    std::lock_guard<std::mutex> guard(mtx_);
    try {   
        char buf[msg_size_] = { 0 };
        listen(sock_, 1);
        client_ = accept(sock_, (struct sockaddr *)&client_address_, &opt_);
        if(client_ < 0){
            return false;
        }
        ba2str( &client_address_.rc_bdaddr, buf );
        client_addr_ = std::string(buf);
        memset(buf, 0, sizeof(buf));
        connected_ = true;
    } catch(...) {
        
    }
    
    return connected_;
}

int rpisocket::BTServer::writeBytes(const std::string& msg)
{
    checkConnection();
    int status = write(client_, msg.c_str(), msg.length());
    if(status == -1){
        connected_ = false;
        throwConnectionLost();
    }
    return status;
}

std::string rpisocket::BTServer::readBytes()
{
    std::string data;
    readBytes(data, msg_size_);
    return data;
}


std::string rpisocket::BTServer::getConnectedClient() const
{
    return client_addr_;
}


void rpisocket::BTServer::getConnectedClient(std::string& out) 
{
    out = client_addr_;
}

void rpisocket::BTServer::readBytes(std::string& out) 
{
    readBytes(out, msg_size_);
}


void rpisocket::BTServer::readBytes(std::string& out, int buffer_lgth) {
    checkConnection();
    std::vector<char> buf(buffer_lgth);
    readBytes(buf);
    out = std::string(buf.begin(), buf.end());  
}

namespace rpisocket
{
    void BTServer::readBytes(std::vector<char> &buf) 
    {
        int bytes_read = read(client_, &buf[0], buf.size());;
        if(bytes_read < 0){
            connected_ = false;
            throwConnectionLost();
        }
    }

    void BTServer::getServerAddress(std::string& out) {
        out = "";
    }
    std::string BTServer::getServerAddress() {
        return "";
    }
    int BTServer::getServerPort() {
        return -1;
    }
}