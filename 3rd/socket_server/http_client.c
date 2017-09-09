#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

const int httpc_id = 300;

static void *
_poll(void * ud)
{
    struct socket_server *ss = ud;
    struct socket_message result;
    for (;;)
    {
        int type = socket_server_poll(ss, &result, NULL);
        // DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
        switch (type)
        {
        case SOCKET_EXIT:
            return NULL;
        case SOCKET_DATA:
            printf("message(%lu) [id=%d] size=%d\n",result.opaque,result.id, result.ud);
            printf("http client recv: %s \n", result.data);
            free(result.data);
            break;
        case SOCKET_CLOSE:
            printf("close(%lu) [id=%d]\n",result.opaque,result.id);
            break;
        case SOCKET_OPEN:
            printf("open(%lu) [id=%d] %s\n",result.opaque,result.id,result.data);
            break;
        case SOCKET_ERROR:
            printf("error(%lu) [id=%d]\n",result.opaque,result.id);
            break;
        case SOCKET_ACCEPT:
            printf("accept(%lu) [id=%d %s] from [%d]\n",result.opaque, result.ud, result.data, result.id);
            socket_server_start(ss, 1000, result.ud);
            break;
        }
    }
}

static void
test_http(struct socket_server* ss, const char* addr, int port)
{
    pthread_t pid;
    pthread_create(&pid, NULL, _poll, ss);

    printf("socket server test http client!\n");

    int httpc = socket_server_connect(ss, httpc_id, addr, port);

    char buf[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nCache-Control: max-age=0\r\nConnection: keep-alive\r\nAccept: text/html\r\nUser-Agent: Mozilla/5.0\r\n\r\n";
    char* sbuf = strdup(buf);
    int64_t ret = socket_server_send(ss, httpc, sbuf, sizeof(buf));
    if (ret < 0)
    {
        printf("test httpc socket server send fail! \n");
    }

    pthread_join(pid, NULL);
}




int
main()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);

    struct socket_server * ss = socket_server_create();
    test_http(ss, "127.0.0.1", 8080);
    socket_server_release(ss);
    return 0;
}
