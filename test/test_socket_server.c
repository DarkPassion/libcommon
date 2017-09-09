#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

const int client_id = 300;
const int httpc_id = 301;

void handle_message(struct socket_server*ss, struct socket_message* msg)
{
    int64_t ret = 0;
    if (msg->opaque == client_id) {
        printf("socket client recv msg [%d %s]\n", msg->ud, msg->data);

        printf("socket client send msg! \n");
    } else {
        printf("socket server recv msg [%d %s]\n", msg->ud, msg->data);

        printf("socket server send msg ! \n");
    }

#if 0
    ret = socket_server_send(ss, msg->id, msg->data, msg->ud);
    if (ret < 0) {
        printf("handle message  replay error ! \n");
    }
#endif
}

static void *
_poll(void * ud) {
	struct socket_server *ss = ud;
	struct socket_message result;
	for (;;) {
		int type = socket_server_poll(ss, &result, NULL);
		// DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
		switch (type) {
		case SOCKET_EXIT:
			return NULL;
		case SOCKET_DATA:
			printf("message(%lu) [id=%d] size=%d\n",result.opaque,result.id, result.ud);
            handle_message(ss, &result);
			//free(result.data);
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
test_http(struct socket_server* ss)
{
    pthread_t pid;
    pthread_create(&pid, NULL, _poll, ss);

    printf("socket server test http client!\n");

    int httpc = socket_server_connect(ss, httpc_id, "baidu.com", 80);

    char buf[] = "GET / HTTP/1.1\r\nHost: www.baidu.com\r\nCache-Control: max-age=0\r\nConnection: keep-alive\r\nAccept: text/html\r\nUser-Agent: Mozilla/5.0\r\n\r\n";
    char* sbuf = strdup(buf);
    int64_t ret = socket_server_send(ss, httpc, sbuf, sizeof(buf));
    if (ret < 0) {
        printf("test httpc socket server send fail! \n");
    }

    pthread_join(pid, NULL);

}


static void
test(struct socket_server *ss) {
	pthread_t pid;
	pthread_create(&pid, NULL, _poll, ss);

    printf("socket server listen 127.0.0.1:8888\n");
	int l = socket_server_listen(ss,200,"127.0.0.1",8888,32);
    socket_server_nodelay(ss, l);
	printf("listening %d\n",l);
	socket_server_start(ss,201,l);
    int client = socket_server_connect(ss, client_id, "127.0.0.1", 8888);

    int index = 0;
    while(index<1) {
        index++;
        char buf[] = "hello world!";
        char* sbuf = strdup(buf);
        printf("client send buff %d %s\n", client, buf);
        int64_t ret = socket_server_send(ss, client, sbuf, sizeof(buf));

        if (ret < 0) {
            printf("socket server send buff error ! %zd %d", ret, index);
        }
        usleep(500 * 1000);
    }

	//sleep(5);
	//socket_server_exit(ss);
	pthread_join(pid, NULL);
}



int
main() {
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	struct socket_server * ss = socket_server_create();
	//test(ss);
    test_http(ss);
	socket_server_release(ss);

	return 0;
}
