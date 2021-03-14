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
    std::string msg, buf;
    std::size_t pos;
    while (threadOn_)
    {
        try
        {
            readBytes(msg);
            buf += msg;
            while ((pos = buf.find(delimiter_)) != std::string::npos)
            {
                msg = buf.substr(0, pos);
                notifyAll(msg);
                buf.erase(0, pos + delimiter_.length());
            }
        }
        catch (...)
        {
            // on error stop thread
            break;
        }
    }
}

void rpisocket::Server::readThreadOff()
{
    if (threadOn_)
    {
        threadOn_ = false;
        thread_.join();
    }
}

void rpisocket::Server::checkConnection() const
{
    if (!connected_)
    {
        throw socket_exception("no open connection; connect to a client first via connect()");
    }
}

void rpisocket::Server::throwConnectionLost()
{
    connected_ = false;
    throw socket_exception("lost connection to client!");
}

bool rpisocket::Server::hasConnection() const
{
    return connected_;
}

rpisocket::Server::Server(int msg_size, const std::string &delimiter) : msg_size_{msg_size}, delimiter_{delimiter}
{
}

namespace rpisocket
{
    Server::~Server()
    {
        readThreadOff();
    }
}