#include "network/ip_addr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void test_ip_addr();

void test_ip_addr()
{

    ip_addr_t t("www.baidu.com");
    char buff[64] = {0};
    if (t.get_ip_addr(buff) == 0) {

        printf("got ip address %s \n", buff);
        
        int s = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("got error socket ! \n");
            return ;
        }
        
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(80);
        addr.sin_addr.s_addr = inet_addr(buff);
        
        if (connect(s, (struct sockaddr*)&addr, sizeof(sockaddr_in)) < 0) {
            printf("got error socket connect! [%s]\n", strerror(errno));
            close(s);
            return ;
        }
        
        printf("got succ connect !! \n");
        
        close(s);
    }
    
    memset(buff, 0, sizeof(buff));
    if (t.get_ip_addr(AF_INET, buff) == 0) {
        
        printf("got ip v4 address %s \n", buff);
    }
    
    memset(buff, 0, sizeof(buff));
    if (t.get_ip_addr(AF_INET6, buff) == 0) {
    
        printf("got ip v6 address %s \n", buff);
    }
    
    memset(buff, 0, sizeof(buff));
    struct sockaddr sa = {0};
    int sa_len = 0;
    if (t.get_ip_addr2(AF_INET, 80, &sa, sa_len) == 0) {
        printf("got ip address 2 success !\n");
        
        int s  = socket(AF_INET, SOCK_STREAM, 0);
        if (s < 0) {
            printf("got error socket ! \n");
            return ;
        }
        if (connect(s, &sa, sa_len) < 0) {
            printf("got error socket connect ! [%s] \n", strerror(errno));
            close(s);
            return ;
        }
        
        printf("got socket connect success! \n");
        close(s);
    }
    
    typedef std::vector<std::string> TypeIpList;
    TypeIpList ip_list;
    if (t.get_ip_addr(ip_list) == 0) {
        printf("got ip address list success! \n");
        
        TypeIpList::iterator iter = ip_list.begin();
        while (iter != ip_list.end()) {
            printf("ip list [%s] \n", iter->c_str());
            iter++;
        }
        
    }
   
}


int main()
{
    test_ip_addr();

    return 0;
}

