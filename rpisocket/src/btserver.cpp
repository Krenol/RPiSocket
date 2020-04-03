#include "BtServer.hpp"

BTServer::BTServer()
{
    opt = sizeof(client_address);
    bdaddr_t tmp = {0,0,0,0,0,0}; //define empty bt address

    // allocate socket
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available
    // local bluetooth adapter
    local_address.rc_family = AF_BLUETOOTH;
    local_address.rc_bdaddr = tmp;
    local_address.rc_channel = (uint8_t) 1;
    bind(sock,(struct sockaddr*)&local_address, sizeof(local_address));
}

BTServer::~BTServer()
{
    shutdown(sock, 2);
    //close the connection
    close(client);
    close(sock);
}

bool BTServer::hasConnection()
{
    return connected;
}

bool BTServer::connect()
{
    char buf[1024] = { 0 };
    listen(sock, 1);
    client = accept(sock, (struct sockaddr *)&client_address, &opt);
    ba2str( &client_address.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    connected = true;
    return connected;
}

int BTServer::writeBytes(string msg)
{
    lock_guard<std::mutex> guard(mtx);
    int status = -1;
    if(!connected)
        return status;  
    status = write(client, msg.c_str(), msg.length());
    if(status == -1)
        connected = false;

    return status;
}

string BTServer::readBytes()
{
    lock_guard<std::mutex> guard(mtx);
    if(!connected)
        return NOTCONNECTED;
    char buf[1024] = { 0 };
    int bytes_read = read(client, buf, sizeof(buf));

    if(bytes_read > 0)
        return (string)buf;
    if(bytes_read == -1)
        connected = false;

    return NODATA;
}

/*void BTServer::readThreadOn()
{
    threadOn = true;
    thrd = thread(&BTServer::readBuffer, this);
}

void BTServer::readBuffer()
{
    double secs = 0.05;
    char buf[1024] = { 0 };
    while(threadOn)
    {
        buffer = readBytes();
        bufferChange = true;

        clock_t end_time = secs * CLOCKS_PER_SEC + clock();
        while(clock() < end_time)
        {
        }
    }
}

void BTServer::readThreadOff()
{
    threadOn = false;
    thrd.join();
}

string BTServer::getBuffer()
{
    lock_guard<std::mutex> guard(mtx);
    string retVal = buffer;
    bufferChange = false;
    return retVal;
}

bool BTServer::bufferChanged()
{
    lock_guard<std::mutex> guard(mtx);
    return bufferChange;
}*/

string BTServer::getConnectedClient()
{
    return string(mac_addr);
}
