#include <unistd.h>
#include <vector>
#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <mutex>      
#include <atomic>
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
        std::atomic_bool threadOn_;
        void readBuffer();

    protected:
        int sock_, msg_size_;
        const std::string delimiter_;
        std::mutex mtx_;
        bool connected_ {false};
        void checkConnection() const;
        void throwConnectionLost();
        

    public:
        Server(int msg_size = 1024, const std::string& delimiter = "\n");
        ~Server();
        virtual bool connect() = 0;
        virtual std::string getConnectedClient() const = 0;
        virtual std::string readBytes() = 0;
        virtual void getConnectedClient(std::string& out) = 0;
        virtual void readBytes(std::string& out) = 0;
        virtual void readBytes(std::string& out, int buffer_lgth) = 0;
        virtual void readBytes(std::vector<char> &buf) = 0;
        virtual int writeBytes(const std::string& msg) = 0;
        virtual bool disconnect() = 0;
        virtual void getServerAddress(std::string& out);
        virtual std::string getServerAddress();
        virtual int getServerPort();
        bool hasConnection() const;
        void readThreadOn();
        void readThreadOff();
    };
}
#endif
