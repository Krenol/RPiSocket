#include "server.hpp"
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#ifndef RPISOCKET_BTSERVER_H
#define RPISOCKET_BTSERVER_H


namespace rpisocket {
    class BTServer : public Server
    {
    private:
        mutable int client_;
        sockaddr_rc local_address_ = {0}, client_address_ = {0};
        mutable bool connected_ = false;
        mutable socklen_t opt_;
        mutable std::string client_addr_ = "";


    public:
        BTServer();
        ~BTServer();
        bool connect() const;
        std::string readBytes() const;
        int writeBytes(const std::string& msg) const;
        std::string getConnectedClient() const;

    };
}
#endif
