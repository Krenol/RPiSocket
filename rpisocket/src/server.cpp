#include "server.hpp"
#include "socket_exception.hpp"

void rpisocket::Server::readThreadOn()
{
    threadOn_ = true;
    thread_ = std::thread(&Server::readBuffer, this);
}

void rpisocket::Server::readBuffer()
{
    while(threadOn_)
    {
        //read incoming msg
        auto msg = readBytes();
        //notify all subscribed agents
        notifyAll(msg);
    }
}

void rpisocket::Server::readThreadOff()
{
    threadOn_ = false;
    thread_.join();
}

void rpisocket::Server::checkConnection() const {
    if(!connected_){
        throw socket_exception("no open connection; connect to a client first via connect()");
    }
}

void rpisocket::Server::throwConnectionLost() {
    connected_ = false;
    throw socket_exception("lost connection to client!");
}

bool rpisocket::Server::hasConnection() const
{
    return connected_;
}
