
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "util/log.h"
#include "ip_addr.h"

ip_addr_t::ip_addr_t(const char* host) : _host(host)
{

}

ip_addr_t::~ip_addr_t()
{

}

int ip_addr_t::get_ip_addr(int af, char* outdata)
{
    int ret = 0;
    struct addrinfo *result, *rp;
    
    ret = getaddrinfo(_host.c_str(), NULL, NULL, &result);
    if (ret != 0) {
        LOGD("%s fail to getaddrinfo: host [%s], err [%s]", __FUNCTION__, _host.c_str(), gai_strerror(ret));
        return -1;
    }
    
    char buf[256] = {0};
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET && af == AF_INET) {
            /* ipv4 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in*)(rp->ai_addr))->sin_addr), buf, sizeof(buf))) {
                strcpy(outdata, buf);
                break;
            }
        } else if (rp->ai_family == AF_INET6 && af == AF_INET6){
            /* ipv6 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in6*)(rp->ai_addr))->sin6_addr), buf, sizeof(buf))) {
                strcpy(outdata, buf);
                break;
            }
        }
    }
    freeaddrinfo(result);

    return 0;
}

int ip_addr_t::get_ip_addr2(int af, int port, struct sockaddr* info, int& len)
{
    int ret = 0;
    struct addrinfo *result, *rp;
    
    if (info == NULL) {
        LOGD("%s error info == NULL ", __FUNCTION__);
        return -1;
    }
    
    len = 0;
    ret = getaddrinfo(_host.c_str(), NULL, NULL, &result);
    if (ret != 0) {
        LOGD("%s fail to getaddrinfo: host [%s], err [%s]", __FUNCTION__, _host.c_str(), gai_strerror(ret));
        return -1;
    }
    
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET && af == AF_INET) {
            /* ipv4 */
            memcpy(info, rp->ai_addr, sizeof(struct sockaddr_in));
            struct sockaddr_in* s_addr_i = (struct sockaddr_in*) info;
            s_addr_i->sin_port = htons(port);
            len = sizeof(struct sockaddr_in);
            break;
        } else if (rp->ai_family == AF_INET6 && af == AF_INET6){
            /* ipv6 */
            memcpy(info, rp->ai_addr, sizeof(struct sockaddr_in6));
            struct sockaddr_in6* s_addr_i6 = (struct sockaddr_in6*) info;
            s_addr_i6->sin6_port = htons(port);
            len = sizeof(struct sockaddr_in6);
            break;
        }
    }
    freeaddrinfo(result);
    return 0;
}

int ip_addr_t::get_ip_addr(char* outdata)
{
    int ret = 0;
    struct addrinfo *result, *rp;
    
    ret = getaddrinfo(_host.c_str(), NULL, NULL, &result);
    if (ret != 0) {
        LOGD("%s fail to getaddrinfo: host [%s], err [%s]", __FUNCTION__, _host.c_str(), gai_strerror(ret));
        return -1;
    }
    
    char buf[256] = {0};
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        if (rp->ai_family == AF_INET) {
            /* ipv4 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in*)(rp->ai_addr))->sin_addr), buf, sizeof(buf))) {
                strcpy(outdata, buf);
                break;
            }
        }
    }
    freeaddrinfo(result);
    return 0;
}


int ip_addr_t::get_ip_addr(std::vector<std::string>& ip_list)
{

    int ret = 0;
    struct addrinfo *result, *rp;
    
    ret = getaddrinfo(_host.c_str(), NULL, NULL, &result);
    if (ret != 0) {
        LOGD("%s fail to getaddrinfo: host [%s], err [%s]", __FUNCTION__, _host.c_str(), gai_strerror(ret));
        return -1;
    }
    
    ip_list.clear();
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        char buf[256] = {0};
        if (rp->ai_family == AF_INET) {
            /* ipv4 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in*)(rp->ai_addr))->sin_addr), buf, sizeof(buf))) {
                ip_list.push_back((char*)buf);
                LOGD("%s ipv4 [%s]", __FUNCTION__, buf);
            }
        } else {
            /* ipv6 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in6*)(rp->ai_addr))->sin6_addr), buf, sizeof(buf))) {
                ip_list.push_back((char*)buf);
                LOGD("%s ipv6 [%s]", __FUNCTION__, buf);
            }
        }
    }
    freeaddrinfo(result);
    
    if (ip_list.empty()) {
        LOGD((char*) "%s : ip_list is empty", __FUNCTION__);
        return -1;
    }
    return 0;
}

bind_addr_t::bind_addr_t(const char* ip, int port) : _ip(ip), _port(port)
{

}

bind_addr_t::~bind_addr_t()
{

}

int bind_addr_t::get_bind_addr(int af, struct sockaddr* info, int& len)
{
    if (info == NULL) {
        LOGE("%s == info == NULL !", __FUNCTION__);
        return -1;
    }
    
    len = 0;
    std::string localhost = "localhost";
    if (af == AF_INET) {
        struct in_addr addr;
        int ret = inet_pton(af, _ip.c_str(), &addr);
        if (ret == 0) {
            LOGE("%s error inet_pton == ", __FUNCTION__);
            return -1;
        }
        struct sockaddr_in sa;
        sa.sin_family = AF_INET;
        sa.sin_port = htons(_port);
        sa.sin_addr.s_addr = addr.s_addr;
        memcpy(info, &sa, sizeof(sa));
        len = sizeof(sa);
    } else if (af == AF_INET6){
        struct in6_addr addr;
        int ret = inet_pton(af, _ip.c_str(), &addr);
        if (ret == 0) {
            LOGE("%s error inet_pton == ", __FUNCTION__);
            return -1;
        }
        struct sockaddr_in6 sa;
        sa.sin6_family = AF_INET6;
        sa.sin6_port = htons(_port);
        memcpy(&(sa.sin6_addr.s6_addr), &addr.s6_addr, sizeof(struct in6_addr));
        memcpy(info, &sa, sizeof(sa));
        len = sizeof(sa);
    }
    
    return 0;
}

connect_addr_t::connect_addr_t()
{

}

connect_addr_t::~connect_addr_t()
{

}

int connect_addr_t::connect(int fd, struct sockaddr* addr, int len)
{
    // default ipv4
    if (connect(fd, addr, len) == 0) {
        struct sockaddr_in* sa = addr;
        LOGD("%s == %s", __FUNCTION__, inet_ntoa(sa->sin_addr));
        return 0;
    } else {
        LOGD("%s == %s error[%s]", __FUNCTION__, inet_ntoa(sa->sin_addr), strerror(errno));
    }
    
    return -1;
}

int connect_addr_t::connect(int fd, const char* host, int port)
{
    int ret = -1;
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(host);
    sa.sin_port = htons(port);
    if (sa.sin_addr.s_addr == INADDR_NONE) {
        ip_addr_t t(host);
        char ip_buff[64] = {0};
        if (t.get_ip_addr(ip_buff) == 0) {
            sa.sin_addr.s_addr = inet_addr(ip_buff);
            sa.sin_port = htons(port);
        } else {
            LOGD("get ip addr failed !! %s [%s]", __FUNCTION__, host);
            return -1;
        }
    }
    
    if (connect(fd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) == 0) {
        LOGD("%s == connect success %s", __FUNCTION__, host);
        ret = 0;
    } else {
        LOGD("%s == connect failed %s", __FUNCTION__, host);
    }


    return -1;

}


int connect_addr_t::connect_ex(int fd, struct sockaddr* addr, int len, int timeout)
{
    
    return 0;
}

int connect_addr_t::connect_ex(int fd, const char* host, int port, int timeout);
{
    
}









