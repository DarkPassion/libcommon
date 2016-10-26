
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <unistd.h>
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
            }
        } else if (rp->ai_family == AF_INET6 && af == AF_INET6){
            /* ipv6 */
            if (inet_ntop(rp->ai_family, &(((struct sockaddr_in6*)(rp->ai_addr))->sin6_addr), buf, sizeof(buf))) {
                strcpy(outdata, buf);
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
        } else if (rp->ai_family == AF_INET6 && af == AF_INET6){
            /* ipv6 */
            memcpy(info, rp->ai_addr, sizeof(struct sockaddr_in6));
            struct sockaddr_in6* s_addr_i6 = (struct sockaddr_in6*) info;
            s_addr_i6->sin6_port = htons(port);
            len = sizeof(struct sockaddr_in6);
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

