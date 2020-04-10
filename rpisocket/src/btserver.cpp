#include "btserver.hpp"
#include "socket_exception.hpp"

rpisocket::BTServer::BTServer()
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
    close(client_);
    close(sock_);
}


bool rpisocket::BTServer::connect() const
{
    connected_ = false;
    try {   
        std::lock_guard<std::mutex> guard(mtx_);
        char* buf;
        listen(sock_, 1);
        client_ = accept(sock_, (struct sockaddr *)&client_address_, &opt_);
        ba2str( &client_address_.rc_bdaddr, buf );
        client_addr_ = std::string(buf);
        memset(buf, 0, sizeof(buf));
        connected_ = true;
    } catch(...) {
        
    }
    
    return connected_;
}

int rpisocket::BTServer::writeBytes(const std::string& msg) const
{
    checkConnection();
    int status = -1;
    std::lock_guard<std::mutex> guard(mtx_);
    status = write(client_, msg.c_str(), msg.length());
    if(status == -1){
        throwConnectionLost();
    }
    return status;
}

std::string rpisocket::BTServer::readBytes() const
{
    checkConnection();
    char buf[1024] = { 0 };
    std::lock_guard<std::mutex> guard(mtx_);
    int bytes_read = read(client_, buf, sizeof(buf));

    if(bytes_read > 0){
        return (std::string)buf;
    }
    if(bytes_read == -1){
        throwConnectionLost();
    }
    return NODATA;
}


std::string rpisocket::BTServer::getConnectedClient() const
{
    return client_addr_;
}
