#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "thread/thread.h"
#include "thread/lock.h"
#include "queue/block_queue.h"
#include "util/log.h"

class DNSServ : public ThreadHandle
{
public:
    DNSServ()
    {
        _pth = new ThreadImp("DNSServ", this, 1000, true);
        _pth->start();
    }

    ~DNSServ()
    {
        LOGD("%s ==== ", __FUNCTION__);
        delete _pth;
        _pth = NULL;
    }

    void add_request(const char* p)
    {
        if (p == NULL) return ;
        _queue.push_back(new std::string(p));
    }

    // ThreadHandle interface
public:
    virtual int cycle()
    {

        while(_pth->can_loop()) {

            std::string* req = NULL;
            req = _queue.pop_front();

            if (req == NULL) {
                continue;
            }

            do_work(req->c_str());
            AutoFreeScopePtr(std::string, req);
        }

        return 0;
    }

    virtual void on_thread_start()
    {
        LOGD("%s == ", __FUNCTION__);
    }


    virtual void on_thread_stop()
    {
        LOGD("%s == ", __FUNCTION__);
    }

    virtual int on_before_cycle()
    {
        LOGD("%s == ", __FUNCTION__);
        return 0;
    }

    virtual int on_end_cycle()
    {
        LOGD("%s == ", __FUNCTION__);
        return 0;
    }

private:
    void do_work(const char* host)
    {
        LOGD("%s == %s ", __FUNCTION__, host);

  		struct addrinfo hints, *res, *res0;
        int error;
        int s = -1;
        int nsock;
        const char *cause = NULL;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        error = getaddrinfo(host, "http", &hints, &res0);

		if (error != 0) {
			LOGD("===== getaddrinfo error %s %s", __FUNCTION__, strerror(errno));
			LOGD("===== getaddrinfo error %s %s", __FUNCTION__, gai_strerror(error));
			return ;
		}

		for (res = res0; res; res = res->ai_next) {
			s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			if (s < 0) {
				LOGD("==== socket error %s ", __FUNCTION__);
				continue;
			}

			struct sockaddr_in sa;
			memcpy(&sa, res->ai_addr, sizeof(sa));
			if (true) {
				char buff[512] = {0};
				if (getnameinfo((struct sockaddr*)&sa, sizeof(sa),  buff, sizeof(buff) - 1, NULL, 0, 0 ) != 0) {
					LOGD("===== socket getnameinfo error %s strerror(error)", strerror(errno));
				} else {
					LOGD("==== socket getnameinfo buff %s ", buff);
				}
			}
			struct sockaddr_in * sinp = (struct sockaddr_in*) res->ai_addr;
			if (connect(s, res->ai_addr, res->ai_addrlen) < 0) {
				LOGD("===== socket connect error %s ", strerror(errno));
				close(s);
				continue;
			}


			LOGD("==== socket connect %s succ  ip:%s ai_canonname:%s", __FUNCTION__, inet_ntoa(*(struct in_addr*) &sa ), res->ai_canonname);
			close(s);
		}
		freeaddrinfo(res0);

    }

private:
    typedef BlockQueue<std::string> TypeDnsQueue;
    TypeDnsQueue _queue;
    ThreadImp* _pth;
};


int test_dns_serv()
{
    LOGD("%s ==== ", __FUNCTION__);
    DNSServ serv;

    serv.add_request("www.baidu.com");
    serv.add_request("www.csdn.net");

    int i = 100;
    while (i > 0)
    {
        i--;
        usleep(100 * 1000);
    }


    return 0;
}

int main()
{

    test_dns_serv();
    return 0;
}
