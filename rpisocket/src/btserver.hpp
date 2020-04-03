#include "Server.hpp"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#ifndef BTSERVER_H
#define BTSERVER_H



using namespace std;


class BTServer : public Server
{
private:
    int client;
    sockaddr_rc local_address = {0}, client_address= {0};
    bool connected = false, threadOn = false, bufferChange = false;
    socklen_t opt;
    char mac_addr[18] = "XX:XX:XX:XX:XX:XX";
    string buffer = "";
    //void readBuffer();
    //thread thrd;
    //std::mutex mtx;


public:
    BTServer();
    ~BTServer();
    bool connect();
    bool hasConnection();
    string readBytes();
    int writeBytes(string msg);
    //void readThreadOn();
    //void readThreadOff();
    //string getBuffer();
    //bool bufferChanged();
    string getConnectedClient();

};
#endif
