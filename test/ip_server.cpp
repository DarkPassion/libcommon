#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "network/ip_addr.h"
#include "thread/thread.h"

using namespace libcommon;

#define TS_PORT 8080
class ServerImp : public ThreadHandle
{
public:
    ServerImp() : _fd(-1)
    {
        init_server();
        _pth = new ThreadImp("ServerImp", this, 1000, true);
        _pth->start();
    }

    ~ServerImp()
    {
        delete _pth;
    }

    void init_server()
    {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("== %s socket error ! \n", __FUNCTION__);
            return ;
        }

        bind_addr_t bind_t("127.0.0.1", TS_PORT);
        struct sockaddr addr;
        int addr_len = 0;
        memset(&addr, 0, sizeof(addr));
        if (bind_t.get_bind_addr(AF_INET, &addr, addr_len) != 0) {
            printf("get_bind_addr fail! \n");
            return ;
        }

        printf("get_bind_addr succ! \n");

        if (::bind(s, &addr, addr_len) != 0) {
            printf("socket bind error! \n");
            return ;
        }

        printf("socket bind success! \n");


        if (::listen(s, 10) != 0) {
            printf("socket listen error! \n");
            return ;
        }

        printf("socket listen success! \n");

        _fd = s;
    }

    virtual int cycle()
    {

        while(_pth->can_loop()) {

            struct sockaddr addr;
            socklen_t addr_len;
            int s = ::accept(_fd, &addr, &addr_len);
            if (s < 0) {
                printf("accept error! %s \n", strerror(errno));
                continue;
            }

            printf("accept success! \n");
            struct sockaddr_in * sa = (struct sockaddr_in *)(&addr);
            printf("accept client ip: [%s] port [%d] \n", inet_ntoa(sa->sin_addr), ntohs(sa->sin_port));
            // process client fd
        }
        return 0;
    }


private:
    ThreadImp*      _pth;
    int             _fd;
};



class ClientImp : public ThreadHandle
{
public:
    ClientImp() : _fd (-1)
    {
        init_client();
        _pth = new ThreadImp("ClientImp", this, 1000, true);
        _pth->start();
    }

    ~ClientImp()
    {
        delete _pth;
    }

    void init_client()
    {
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("%s == socket error! \n", __FUNCTION__);
            return ;
        }

        connect_addr_t connect_t(s);
        if (connect_t.connect_addr("127.0.0.1", TS_PORT) != 0) {
            printf("%s == connect_t.connect error! \n ", __FUNCTION__);
            return ;
        }

        printf("%s == connect_t.connect success! \n", __FUNCTION__);


        _fd = s;
    }

    virtual int cycle()
    {
        while(_pth->can_loop()) {

            char buff[1024] = {0};
            int ret = ::recv(_fd, buff, sizeof(buff) -1, 0);
            if (ret < 0) {
                printf(" %s == client recv error %d! \n", __FUNCTION__, ret);

                if (ret == EAGAIN) {
                    printf("%s == client recv error EAGAIN \n", __FUNCTION__);
                }
            } else if (ret == 0) {
                printf("client BOKEN pipe line! \n");
                _pth->stop_loop();
            }else {
                printf("%s == client recv succ %d [%s]! \n", __FUNCTION__, ret, buff);
            }

            usleep(10 * 1000);

        }
        return 0;
    }

private:
    ThreadImp*  _pth;
    int         _fd;
};




int main()
{

    ServerImp server;
    ClientImp client;

    while(1) {
        usleep(100* 1000);

    }

    return 0;
}
