#include "server.hpp"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#ifndef RPISOCKET_BTSERVER_H
#define RPISOCKET_BTSERVER_H


namespace rpisocket {
    class BTServer : public Server
    {
    private:
        int client;
        sockaddr_rc local_address = {0}, client_address= {0};
        bool connected = false, threadOn = false, bufferChange = false;
        socklen_t opt;
        char mac_addr[18] = "XX:XX:XX:XX:XX:XX";
        std::string buffer = "";


    public:
        BTServer();
        ~BTServer();
        bool connect();
        bool hasConnection();
        std::string readBytes();
        int writeBytes(string msg);
        std::string getConnectedClient();

    };
}
#endif
