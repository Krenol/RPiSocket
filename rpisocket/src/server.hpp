#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>      

#ifndef RPISOCKET_SERVER_H
#define RPISOCKET_SERVER_H

#define NOTCONNECTED "not connected"
#define NODATA "no data"

typedef std::function<void(const std::string&)>* subFunc;

namespace rpisocket {

    class Server
    {
    private:
        std::thread thread_;
        bool threadOn_;
        std::vector<subFunc> subs_;
        void notify(const std::string& msg);
        void readBuffer();

    protected:
        int sock_;
        const std::chrono::milliseconds wait_duration_ = std::chrono::milliseconds(50);
        std::mutex mtx_;
        bool connected_ {false};
        void checkConnection() const;
        void throwConnectionLost();

    public:
        virtual bool connect() = 0;
        virtual std::string getConnectedClient() const = 0;
        virtual std::string readBytes() = 0;
        virtual int writeBytes(const std::string& msg) = 0;
        virtual bool disconnect() = 0;
        bool hasConnection() const;
        void readThreadOn();
        void readThreadOff();
        int subscribe(subFunc func);
        bool unsubscribe(int pos);
    };
}
#endif
