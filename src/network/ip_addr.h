#ifndef _NET_WORK_IP_ADDR_H_
#define _NET_WORK_IP_ADDR_H_



#include <string>
#include <vector>

struct sockaddr;
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
    int get_ip_addr2(int af, int port, struct sockaddr* info, int &len);
    
    // 默认获取ipv4
    int get_ip_addr(char* outdata);
    
    // 获取所有ip地址
    int get_ip_addr(std::vector<std::string>& iplist);
    
private:
    std::string _host;
};



#endif


