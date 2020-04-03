#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <iostream>
#include <string>

#include <thread>
#include <mutex>          // std::mutex

#ifndef RPISOCKET_SERVER_H
#define RPISOCKET_SERVER_H

#define NOTCONNECTED "not connected"
#define NODATA "no data"
namespace rpisocket {

    class Server
    {
    private:
        std::thread thrd;
        bool threadOn, bufferChange;
        std::string buffer;
    protected:
        int sock;
        double waitingTime = 0.05;
        std::mutex mtx;
        void readBuffer();

    public:
        virtual bool hasConnection() = 0;
        virtual bool connect() = 0;
        virtual std::string getConnectedClient() = 0;
        virtual std::string readBytes()=0;
        virtual int writeBytes(string msg)=0;
        void readThreadOn();
        void readThreadOff();
        std::string getBuffer();
        bool bufferChanged();

    };
}
#endif
