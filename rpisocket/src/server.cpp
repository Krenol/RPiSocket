#include "Server.hpp"

void rpisocket::Server::readThreadOn()
{
    threadOn = true;
    thrd = thread(&Server::readBuffer, this);
}

void rpisocket::Server::readBuffer()
{
    while(threadOn)
    {
        buffer = readBytes();
        bufferChange = true;

        clock_t end_time = waitingTime * CLOCKS_PER_SEC + clock();
        while(clock() < end_time)
        {
        }
    }
}

void rpisocket::Server::readThreadOff()
{
    threadOn = false;
    thrd.join();
}

std::string rpisocket::Server::getBuffer()
{
    std::lock_guard<std::mutex> guard(mtx);
    string retVal = buffer;
    bufferChange = false;
    return retVal;
}

bool rpisocket::Server::bufferChanged()
{
    std::lock_guard<std::mutex> guard(mtx);
    return bufferChange;
}
