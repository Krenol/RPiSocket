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
        mutable std::thread thread_;
        mutable bool threadOn_;
        mutable std::vector<subFunc> subs_;
        void notify(const std::string& msg) const;
        void readBuffer() const;

    protected:
        int sock_;
        const std::chrono::milliseconds wait_duration_ = std::chrono::milliseconds(50);
        mutable std::mutex mtx_;
        mutable bool connected_ {false};
        void checkConnection() const;
        void throwConnectionLost() const;

    public:
        virtual bool connect() const = 0;
        virtual std::string getConnectedClient() const = 0;
        virtual std::string readBytes() const = 0;
        virtual int writeBytes(const std::string& msg) const = 0;
        bool hasConnection() const;
        void readThreadOn() const;
        void readThreadOff() const;
        int subscribe(subFunc func) const;
        bool unsubscribe(int pos) const;
    };
}
#endif
