
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
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

int ip_addr_t::get_socket_name(int fd, char* ip, int& port)
{
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    
    port = 0;
    if (ip != NULL) {
        ip[0] = '\0';
        ip[1] = '\0';
    }
    
    int ret = getsockname(fd, (struct sockaddr*)&sa, &salen);
    if (ret == 0) {
        LOGD("%s == getsockname succ ", __FUNCTION__);
        
        if (ip)
            strcpy(ip, inet_ntoa(sa.sin_addr));
        port = ntohs(sa.sin_port);
        
    } else {
        LOGD("%s == getsockname fail", __FUNCTION__);
        return -1;
    }
    
    
    return 0;
}

int ip_addr_t::get_peer_name(int fd, char* ip, int& port)
{
    struct sockaddr_in sa;
    socklen_t salen = sizeof(sa);
    
    port = 0;
    if (getpeername(fd, (struct sockaddr*) &sa, &salen) == -1) {
        LOGE("%s == getpeername error!", __FUNCTION__);
        if (ip) {
            ip[0] = '\0';
            ip[1] = '\0';
        }
        return -1;
    }
    LOGD("%s == getpeername succ! ", __FUNCTION__);
    
    if (ip)
        strcpy(ip, inet_ntoa(sa.sin_addr));
    
    port = ntohs(sa.sin_port);
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

connect_addr_t::connect_addr_t(int fd) : _fd(fd)
{

}

connect_addr_t::~connect_addr_t()
{

}

int connect_addr_t::connect(struct sockaddr* addr, int len)
{
    // default ipv4
    struct sockaddr_in* sa = (struct sockaddr_in*)addr;
    if (::connect(_fd, addr, len) == 0) {
        LOGD("%s == %s", __FUNCTION__, inet_ntoa(sa->sin_addr));
        return 0;
    } else {
        LOGD("%s == %s error[%s]", __FUNCTION__, inet_ntoa(sa->sin_addr), strerror(errno));
    }
    
    return -1;
}

int connect_addr_t::connect(const char* host, int port)
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
    
    if (::connect(_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr)) == 0) {
        LOGD("%s == connect success %s", __FUNCTION__, host);
        ret = 0;
    } else {
        LOGD("%s == connect failed %s", __FUNCTION__, host);
        ret = -1;
    }


    return ret;

}


int connect_addr_t::connect_ex(struct sockaddr* addr, int len, int timeout_ms)
{
    int flags = fcntl(_fd, F_GETFL, 0);
    int old_flags = flags;
    if (flags & O_NONBLOCK) {
    
    } else {
        flags |= O_NONBLOCK;
        fcntl(_fd, F_SETFL, flags);
    }
    
    int result = SOCK_CONNECT_ERR;
    do {
    
        int ret = ::connect(_fd, addr, len);
        if (ret == 0) {
            // connect ok
            result = SOCK_CONNECTED;
            break;
        } else {
            fd_set rset;
            fd_set wset;
            FD_ZERO(&rset);
            FD_ZERO(&wset);
            FD_SET(_fd, &rset);
            FD_SET(_fd, &wset);
            
            timeval tv;
            tv.tv_sec = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;
            
            ret = select(_fd + 1, &rset, &wset, NULL, &tv);
            if (ret < 0) {
                // select error
                LOGE("%s == select error %s fd %d", __FUNCTION__, strerror(errno), _fd);
                result = SOCK_CONNECT_ERR;
                break;
            } else if (ret == 0) {
                // timeout
                struct sockaddr_in* sina = (struct sockaddr_in*)addr;
                LOGD("%s == select timeout %s", __FUNCTION__, inet_ntoa(sina->sin_addr));
                result = SOCK_CONNECT_TIMEOUT;
                break;
            } else {
                if (FD_ISSET(_fd, &rset) || FD_ISSET(_fd, &wset)) {
                    int error = 0;
                    int error_len = sizeof(error);
                    if (getsockopt(_fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t*)&error_len) == -1) {
                        LOGE("%s select got socket error %s ", __FUNCTION__, strerror(errno));
                        result = SOCK_CONNECT_ERR;
                    } else {
                        result = SOCK_CONNECTED;
                    }
                } else {
                    // select fd unable read
                    LOGD("%s == select fd unable read!", __FUNCTION__);
                }
            }
        }
    } while (0);
    
    
    fcntl(_fd, F_SETFL, old_flags);
    return result;
}

int connect_addr_t::connect_ex(const char* host, int port, int timeout)
{
    int result = SOCK_CONNECT_ERR;

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
            LOGE("get ip addr failed !! %s [%s]", __FUNCTION__, host);
            return result;
        }
    }
    struct stat st;
    if (fstat(_fd, &st) != 0) {
        LOGE("%s == fd invalid! ", __FUNCTION__);
    }
    
    LOGD("%s == host %s port %d", __FUNCTION__, host, port);
    // connect_ex(struct sockaddr* addr, int len, int timeout_ms)
    result = connect_ex((struct sockaddr*)&sa, sizeof(struct sockaddr_in), timeout);
    
    return result;
}









