#include "server.hpp"

void rpisocket::Server::readThreadOn() const
{
    threadOn_ = true;
    thread_ = std::thread(&Server::readBuffer, this);
}

void rpisocket::Server::readBuffer() const
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

void rpisocket::Server::readThreadOff() const
{
    threadOn_ = false;
    thread_.join();
}

int rpisocket::Server::subscribe(subFunc func) const
{
    std::lock_guard<std::mutex> guard(mtx_);
    subs_.push_back(func);
    return subs_.size() - 1;
}


bool rpisocket::Server::unsubscribe(int pos) const
{
    if(pos >= subs_.size() || pos < 0) return false;

    std::lock_guard<std::mutex> guard(mtx_);
    subs_.erase (subs_.begin() + pos);
    return true;
}

void rpisocket::Server::notify(const std::string& msg) const
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
