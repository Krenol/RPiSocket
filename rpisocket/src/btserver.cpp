#include "btserver.hpp"

rpisocket::BTServer::BTServer()
{
    opt = sizeof(client_address);
    bdaddr_t tmp = {0,0,0,0,0,0}; //define empty bt address

    // allocate socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    local_address.rc_family = AF_BLUETOOTH;
    local_address.rc_bdaddr = tmp;
    local_address.rc_channel = (uint8_t) 1;
    bind(sock,(struct sockaddr*)&local_address, sizeof(local_address));
}

rpisocket::BTServer::~BTServer()
{
    shutdown(sock, 2);
    //close the connection
    close(client);
    close(sock);
}

bool rpisocket::BTServer::hasConnection()
{
    return connected;
}

bool rpisocket::BTServer::connect()
{
    char buf[1024] = { 0 };
    listen(sock, 1);
    client = accept(sock, (struct sockaddr *)&client_address, &opt);
    ba2str( &client_address.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    connected = true;
    return connected;
}

int rpisocket::BTServer::writeBytes(string msg)
{
    std::lock_guard<std::mutex> guard(mtx);
    int status = -1;
    if(!connected){
        return status; 
    } 
    status = write(client, msg.c_str(), msg.length());
    if(status == -1){
        connected = false;
    }
    return status;
}

std::string rpisocket::BTServer::readBytes()
{
    std::lock_guard<std::mutex> guard(mtx);
    if(!connected){
        return NOTCONNECTED;
    }
    char buf[1024] = { 0 };
    int bytes_read = read(client, buf, sizeof(buf));

    if(bytes_read > 0){
        return (std::string)buf;
    }
    if(bytes_read == -1){
        connected = false;
    }
    return NODATA;
}


std::string rpisocket::BTServer::getConnectedClient()
{
    return std::string(mac_addr);
}
