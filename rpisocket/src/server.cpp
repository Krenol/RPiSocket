#include "server.hpp"
#include "socket_exception.hpp"
#include <algorithm>

void rpisocket::Server::readThreadOn()
{
    threadOn_ = true;
    thread_ = std::thread(&Server::readBuffer, this);
}

void rpisocket::Server::readBuffer()
{   
    std::string msg;
    std::vector<char> buf, read(msg_size_);
    std::vector<char>::iterator pos;
    while(threadOn_)
    {
        try{
            readBytes(read); // read msg
            buf.insert(buf.end(), read.begin(), read.end()); // ad msg to buffer
            pos = std::find(buf.begin(), buf.end(), delimiter_);
            while (pos != buf.end()) { 
                msg = std::string(buf.begin(), pos); // get msg till delimiter
                buf.erase(buf.begin(), pos); // remove msg from buffer
                //notify all subscribed agents
                notifyAll(msg);
                pos = std::find(buf.begin(), buf.end(), delimiter_);
            }  
        } catch(...) {
            // on error stop thread
            break;
        }
        
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

rpisocket::Server::Server(int msg_size, char delimiter) : msg_size_{msg_size}, delimiter_{delimiter}
{
    
}
