#ifndef _NET_WORK_IP_ADDR_H_
#define _NET_WORK_IP_ADDR_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include <string>
#include <vector>

namespace libcommon
{
    enum {
    SOCK_CONNECTED = 0, // 链接成功
    SOCK_CONNECT_ERR = 1,   // 出现错误
    SOCK_CONNECT_TIMEOUT = 2, // 链接timeout
    };
    
    void set_socket_block(int fd, bool isblock);
    
    class ip_addr_t
    {
    public:
        ip_addr_t(const char* host);
        
        ~ip_addr_t();
        
        // @af AF_INET ipv4 AF_INET6 ipv6
        // @outdata ip字符串
        int get_ip_addr(int af, char* outdata);
        
        // 获取sockadr info
        // @af 取值sa_family_t
        // @port 端口号
        // @info 获取sockaddr地址
        // @len 获取sockaddr 长度 ipv4 / ipv6 长度不一样
        int get_ip_addr2(int af, int port, sockaddr* info, int &len);
        
        // 默认获取ipv4
        int get_ip_addr(char* outdata);
        
        // 获取所有ip地址
        int get_ip_addr(std::vector<std::string>& iplist);
        
        // getsockname
        int get_socket_name(int fd, char* ip, int& port);
        
        // getpeername
        int get_peer_name(int fd, char* ip, int& port);
        
    private:
        std::string _host;
    };
    
    
    class bind_addr_t
    {
    public:
        bind_addr_t(const char* ip, int port);
        
        ~bind_addr_t();
        
        int bind(int fd);
    private:
        int get_bind_addr(int af, sockaddr* info, int &len);

    private:
        std::string _ip;
        int _port;
    };
    
    class connect_addr_t
    {
    public:
        connect_addr_t(int fd);
        
        ~connect_addr_t();
        
        int connect_addr(sockaddr* addr, int len);
        
        int connect_addr(const char* host, int port);
        
        int connect_addr_ex(sockaddr* addr, int len, int timeout);
        
        int connect_addr_ex(const char* host, int port, int timeout);
    private:
        int _fd;
    };
    
}


#endif


