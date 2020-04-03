#include "Server.hpp"

void Server::readThreadOn()
{
    threadOn = true;
    thrd = thread(&Server::readBuffer, this);
}

void Server::readBuffer()
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

void Server::readThreadOff()
{
    threadOn = false;
    thrd.join();
}

string Server::getBuffer()
{
    lock_guard<std::mutex> guard(mtx);
    string retVal = buffer;
    bufferChange = false;
    return retVal;
}

bool Server::bufferChanged()
{
    lock_guard<std::mutex> guard(mtx);
    return bufferChange;
}
