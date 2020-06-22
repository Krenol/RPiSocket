#include "server.hpp"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#ifndef RPISOCKET_BTSERVER_H
#define RPISOCKET_BTSERVER_H


namespace rpisocket {
    class BTServer : public Server
    {
    private:
        int client_;
        sockaddr_rc local_address_ = {0}, client_address_ = {0};
        socklen_t opt_;
        std::string client_addr_ = "";


    public:
        BTServer();
        ~BTServer();
        bool connect();
        bool disconnect();
        std::string readBytes();
        int writeBytes(const std::string& msg);
        std::string getConnectedClient() const;

    };
}
#endif
