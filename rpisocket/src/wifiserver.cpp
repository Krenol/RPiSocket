#include "wifiserver.hpp"
#include "socket_exception.hpp"
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <iostream>
#include <string.h>

namespace rpisocket
{
    void WiFiServer::initSocket()
    {
        int opt = 1;
        sock_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_ < 0)
        {
            throw socket_exception("error opening socket!");
        }

        // Forcefully attaching socket to the port
        if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            throw socket_exception("error while attaching to port!");
        }

        server_.sin_family = AF_INET;
        server_.sin_addr.s_addr = INADDR_ANY;
        server_.sin_port = htons(port_);

        //bind socket to the port
        if (bind(sock_, (struct sockaddr *)&server_, sizeof(server_)) < 0)
        {
            throw socket_exception("error while binding the server!");
        }
    }

    void WiFiServer::readBytes(std::vector<char> &buf)
    {
        int bytes_read = 0;
        bytes_read = read(newsock_, &buf[0], buf.size());
        if (bytes_read <= 0)
        {
            connected_ = false;
            throwConnectionLost();
        }
    }

    WiFiServer::WiFiServer(int port, int msg_size, const std::string &delimiter) : Server(msg_size, delimiter), port_{port}
    {
    }

    WiFiServer::~WiFiServer()
    {
        //close the connection
        disconnect();
    }

    void WiFiServer::disconnect()
    {
        close(newsock_);
    }

    bool WiFiServer::connect()
    {
        initSocket();
        connected_ = false;
        listen(sock_, 1);

        int c = sizeof(struct sockaddr_in);
        newsock_ = accept(sock_, (struct sockaddr *)&client_, (socklen_t *)&c);
        if (newsock_ > 0)
        {
            connected_ = true;
            close(sock_);
        }
        return connected_;
    }

    std::string WiFiServer::readBytes()
    {
        std::string msg;
        readBytes(msg, msg_size_);
        return msg;
    }

    int WiFiServer::writeBytes(const std::string &msg)
    {
        checkConnection();
        int status = write(newsock_, msg.c_str(), msg.length());
        if (status == -1)
        {
            connected_ = false;
            throwConnectionLost();
        }
        return status;
    }

    std::string WiFiServer::getConnectedClient() const
    {
        return inet_ntoa(client_.sin_addr);
    }

    std::string WiFiServer::getServerAddress()
    {
        std::string ip;
        getServerAddress(ip);
        return ip;
    }

    void WiFiServer::getConnectedClient(std::string &out)
    {
        out = inet_ntoa(client_.sin_addr);
    }

    void WiFiServer::readBytes(std::string &out)
    {
        readBytes(out, msg_size_);
    }

    void WiFiServer::readBytes(std::string &out, int buffer_lgth)
    {
        checkConnection();
        //char buf[buffer_lgth] = { 0 };
        std::vector<char> buf(buffer_lgth);
        readBytes(buf);
        out = std::string(buf.begin(), buf.end());
    }

    void WiFiServer::getServerAddress(std::string &out)
    {
        //from https://stackoverflow.com/a/2283541
        struct ifreq ifr;
        ifr.ifr_addr.sa_family = AF_INET;

        /* I want IP address attached to "wlan0" */
        strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);
        ioctl(sock_, SIOCGIFADDR, &ifr);
        out = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    }

    int WiFiServer::getServerPort()
    {
        return port_;
    }
} // namespace rpisocket