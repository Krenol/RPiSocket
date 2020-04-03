#include <unistd.h>
#include <sys/socket.h>
#include <vector>
#include <iostream>
#include <string>

#include <thread>
#include <mutex>          // std::mutex

#ifndef SERVER_H
#define SERVER_H

#define NOTCONNECTED "not connected"
#define NODATA "no data"

using namespace std;

class Server
{
private:
    thread thrd;
    bool threadOn, bufferChange;
    string buffer;
protected:
    int sock;
    double waitingTime = 0.05;
    mutex mtx;
    void readBuffer();

public:
    virtual bool hasConnection() = 0;
    virtual bool connect() = 0;
    virtual string getConnectedClient() = 0;
    virtual string readBytes()=0;
    virtual int writeBytes(string msg)=0;
    void readThreadOn();
    void readThreadOff();
    string getBuffer();
    bool bufferChanged();

};
#endif
