#include <unistd.h>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <mutex>      
#include "design_patterns/design_patterns.hpp"

#ifndef RPISOCKET_SERVER_H
#define RPISOCKET_SERVER_H

#define NOTCONNECTED "not connected"
#define NODATA "no data"

namespace rpisocket {

    class Server : public design_patterns::Publisher<std::string>
    {
    private:
        std::thread thread_;
        bool threadOn_;
        void readBuffer();

    protected:
        int sock_, msg_size_;
        std::mutex mtx_;
        bool connected_ {false};
        void checkConnection() const;
        void throwConnectionLost();

    public:
        Server(int msg_size = 1024);
        virtual bool connect() = 0;
        virtual std::string getConnectedClient() const = 0;
        virtual std::string readBytes() = 0;
        virtual void getConnectedClient(std::string& out) = 0;
        virtual void readBytes(std::string& out) = 0;
        virtual int writeBytes(const std::string& msg) = 0;
        virtual bool disconnect() = 0;
        bool hasConnection() const;
        void readThreadOn();
        void readThreadOff();
    };
}
#endif
