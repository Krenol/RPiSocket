#ifndef RPISOCKET_WIFISERVER_H
#define RPISOCKET_WIFISERVER_H

#define _XOPEN_SOURCE_EXTENDED 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.hpp"

namespace rpisocket {
    class WiFiServer : public Server
    {
    private:
        int sock_, newsock_, port_;
        struct sockaddr_in server_, client_;

        void initSocket();

    public:

        WiFiServer(int port, int msg_size = 1024, const std::string& delimiter = "\n");
        ~WiFiServer();
        void disconnect();
        bool connect();
        std::string readBytes();
        int writeBytes(const std::string& msg);
        std::string getConnectedClient() const;
        std::string getServerAddress();
        void getConnectedClient(std::string& out);
        void readBytes(std::string& out);
        void readBytes(std::string& out, int buffer_lgth);
        void readBytes(std::vector<char> &vec);
        void getServerAddress(std::string& out);
        int getServerPort();

    };
}
#endif
