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
        notify(msg);
        std::this_thread::sleep_for(wait_duration_);
    }
}

void rpisocket::Server::readThreadOff()
{
    threadOn_ = false;
    thread_.join();
}

int rpisocket::Server::subscribe(subFunc func)
{
    std::lock_guard<std::mutex> guard(mtx_);
    subs_.push_back(func);
    return subs_.size() - 1;
}


bool rpisocket::Server::unsubscribe(int pos)
{
    if(pos >= subs_.size() || pos < 0) return false;

    std::lock_guard<std::mutex> guard(mtx_);
    subs_.erase (subs_.begin() + pos);
    return true;
}

void rpisocket::Server::notify(const std::string& msg)
{
    std::lock_guard<std::mutex> guard(mtx_);
    for(auto f : subs_) {
        try{
            (*f)(msg);
        } catch(...) {
            continue;
        }
    }
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