#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define MAX_CLIENTS (1<<16)
#define HTTP_LR     "\r\n"
#define HTTP_LR_LR  "\r\n\r\n"

struct http_context
{
    char buf[2048];
    int len;
    int id;  // client socket
    char cip[20]; // client ip
    int cport; // client port
};

struct http_context* H[MAX_CLIENTS] = {NULL};

void dump_http_context(struct http_context* c)
{
    printf("dump http context = %d %s ", c->len, c->buf);

    if (c->len > sizeof(c->buf) - 1)
    {
        printf("dump http context c->len > sizeof(c->buf) - 1 ! \n");
    }

    char * p = c->buf;
    int i = 0;
    while (1)
    {
        char * s = strstr(p + i, HTTP_LR);
        if (s == NULL)
        {
            break;
        }

        if (strncmp(s, HTTP_LR_LR, 4) == 0)
        {
            printf("parse header fields end! \n");
            break;
        }

        s[0] = '\0';
        printf("lien %s \n", p + i);
        i += strlen(p + i) + 2;
        s[0] = '\r';
    }
    printf("client ip [%s] port [%d] \n", c->cip, c->cport);
}


struct http_context* get_http_context(int id)
{
    printf("get_http_context %d \n", id);

    if (id <= 0 || id > MAX_CLIENTS)
    {
        printf("error id <= 0 || id > MAX_CLIENTS !\n");
        return NULL;
    }
    struct http_context* c = H[id];
    if (c && c->id == id)
    {
        return c;
    }
    return NULL;
}


void add_http_context(struct http_context* c)
{
    printf("add_http_context %p \n", c);
    if (c->id <= 0 || c->id > MAX_CLIENTS)
    {
        printf("error add http context ! id \n");
    }

    if (H[c->id] != NULL)
    {
        printf("error add http context already exist! \n");
    }

    H[c->id] = c;
}

void remove_http_context(int id)
{
    printf("remove_http_context %d \n", id);
    if (id <= 0 || id > MAX_CLIENTS)
    {
        printf("error remove http context! \n");
    }

    if (H[id]->id == id)
    {
        struct http_context* c = H[id];
        free(c);
        H[id] = NULL;
    }
}

void handle_message(struct socket_server*ss, struct socket_message* msg)
{
    printf("http server recv : %d %s \n", msg->ud, msg->data);

    struct http_context* c = get_http_context(msg->id);
    if (c == NULL)
    {
        printf("handle message c == NULL ! \n");
    }

    memcpy(c->buf + c->len, msg->data, msg->ud);
    c->len += msg->ud;

    if (strstr(c->buf, HTTP_LR_LR) != NULL)
    {
        // maybe end!
        dump_http_context(c);
        char buf[] = "hello world !";
        char *p = strdup(buf);
        int64_t ret = socket_server_send(ss, msg->id, p, sizeof(buf));
        if (ret < 0)
        {
            printf("handle message  replay error ! \n");
        }
    }
    else
    {
        printf("http context not compleate! \n");
    }

}


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
            handle_message(ss, &result);
            free(result.data);
            break;
        case SOCKET_CLOSE:
            printf("close(%lu) [id=%d]\n",result.opaque,result.id);
            remove_http_context(result.id);
            break;
        case SOCKET_OPEN:
            printf("open(%lu) [id=%d] %s\n",result.opaque,result.id,result.data);
            break;
        case SOCKET_ERROR:
            printf("error(%lu) [id=%d]\n",result.opaque,result.id);
            break;
        case SOCKET_ACCEPT:
            printf("accept(%lu) [id=%d %s] from [%d]\n",result.opaque, result.ud, result.data, result.id);
            struct http_context* c = (struct http_context*) malloc(sizeof(struct http_context));
            memset(c, 0, sizeof(struct http_context));
            c->id = result.ud;
            char* cpdata = strdup(result.data);
            char* p = strchr(cpdata, ':');
            if (p)
            {
                p[0] = '\0';
                strcpy(c->cip, cpdata);
                c->cport = atoi(p+1);
                p[0] = ':';
            }
            free(cpdata);
            add_http_context(c);
            socket_server_start(ss, 1000, result.ud);
            break;
        }
    }
}



static void
http_server(struct socket_server *ss, const char* addr, int port)
{

    pthread_t pid;
    pthread_create(&pid, NULL, _poll, ss);

    printf("socket server listen 127.0.0.1:8888\n");
    int l = socket_server_listen(ss, 200, addr, port, 32);
    socket_server_nodelay(ss, l);
    printf("listening %d\n",l);
    socket_server_start(ss,201,l);
    pthread_join(pid, NULL);
}



int
main()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);

    struct socket_server * ss = socket_server_create();
    http_server(ss, "127.0.0.1", 8080);
    socket_server_release(ss);

    return 0;
}
