#ifndef poll_socket_select_h
#define poll_socket_select_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_SELECT_NUM (2 << 16)

#define E_SOCKET_NONE	(0)
#define E_SOCKET_READ	(1<<0)
#define E_SOCKET_WRITE	(1<<1)

struct s_socket_t
{
	int fd;
	int mask;
	void *ud;
};

struct s_select_t
{
	int event_fds;	// Highest fd in fd set
	fd_set writeset_in;
	fd_set readset_in;
	fd_set errorset_in;
	fd_set writeset_out;
	fd_set readset_out;
	fd_set errorset_out;
	struct s_socket_t* st[MAX_SELECT_NUM]; // s_socket_t data
};

struct s_select_t* SELECT[MAX_SELECT_NUM] = {NULL};


static void 
update_event_fds(struct s_select_t* sel){

	for (int i = MAX_SELECT_NUM -1; i > 0; i--) {
		if (sel->st[i] != E_SOCKET_NONE && sel->st[i]->fd > 0) {
			sel->event_fds = i;
			break;
		}
	}

}

static bool 
sp_invalid(int efd) {
	return efd == -1;
}

static int
sp_create() {
	int idx = -1;
	for (int i = 0; i < MAX_SELECT_NUM; ++i)
	{
		if (SELECT[i] == NULL) {
			struct s_select_t* sel = (struct s_select_t*)malloc(sizeof(struct s_select_t));
			FD_ZERO(&sel->readset_in);
			FD_ZERO(&sel->writeset_in);
			FD_ZERO(&sel->readset_out);
			FD_ZERO(&sel->writeset_out);
			FD_ZERO(&sel->errorset_in);
			FD_ZERO(&sel->errorset_out);
			sel->event_fds = 0;

			for (int i = 0; i < MAX_SELECT_NUM; ++i) {
				sel->st[i] = (struct s_socket_t*) malloc(sizeof(struct s_socket_t));
				//memset(sel->st[i], 0, sizeof(struct s_socket_t));
				sel->st[i]->fd = 0;
				sel->st[i]->mask = E_SOCKET_NONE;
				sel->st[i]->ud = NULL;
			}
			SELECT[i] = sel;
			idx = i;
			break;
		}
	}
	return idx;
}

static void
sp_release(int efd) {
	if (efd>= 0 && efd < MAX_SELECT_NUM && SELECT[efd] != NULL) {
		struct s_select_t* sel = SELECT[efd];
		for (int i = 0; i < MAX_SELECT_NUM; ++i) {
			free(sel->st[i]);
		}
		free(sel);
		SELECT[efd] = NULL;
	}
}

static int 
sp_add(int efd, int sock, void *ud) {
	if (efd >= 0 && efd < MAX_SELECT_NUM && SELECT[efd] != NULL && sock >= 0 && sock < MAX_SELECT_NUM) {
		struct s_select_t* sel = SELECT[efd];
		sel->event_fds = sock > sel->event_fds ? sock : sel->event_fds;
		FD_SET(sock, &sel->readset_in);
		FD_SET(sock, &sel->errorset_in);
		sel->st[sock]->fd = sock;
		sel->st[sock]->mask = sel->st[sock]->mask | E_SOCKET_READ;
		sel->st[sock]->ud = ud;
	} else {
		return -1;
	}
	return 0;
}

static void 
sp_del(int efd, int sock) {
	if (efd >= 0 && efd < MAX_SELECT_NUM && SELECT[efd] != NULL && sock >= 0 && sock < MAX_SELECT_NUM) {
		struct s_select_t* sel = SELECT[efd];
		FD_CLR(sock, &sel->readset_in);
		FD_CLR(sock, &sel->writeset_in);
		FD_CLR(sock, &sel->errorset_in);
		sel->st[sock]->fd = 0;
		sel->st[sock]->mask = E_SOCKET_NONE;
		sel->st[sock]->ud = NULL;
		update_event_fds(sel);
	}
}

static void 
sp_write(int efd, int sock, void *ud, bool enable) {
	if (efd >= 0 && efd < MAX_SELECT_NUM && SELECT[efd] != NULL && sock >= 0 && sock < MAX_SELECT_NUM) {
		struct s_select_t* sel = SELECT[efd];
		sel->st[sock]->fd = sock;
		sel->st[sock]->mask = sel->st[sock]->mask | (enable ? E_SOCKET_WRITE : 0);
		sel->st[sock]->ud = ud;
		FD_SET(sock, &sel->errorset_in);
		if (enable) {
			sel->event_fds = sock > sel->event_fds ? sock : sel->event_fds;
			FD_SET(sock, &sel->writeset_in);
		} else {
			FD_CLR(sock, &sel->writeset_in);
		}
	}
}

static int 
sp_wait(int efd, struct event *e, int max) {
	if (efd >= 0 && efd < MAX_SELECT_NUM && SELECT[efd] != NULL) {
		struct  s_select_t* sel = SELECT[efd];
		int nfds = sel->event_fds + 1;

		memcpy(&sel->readset_out, &sel->readset_in, sizeof(sel->readset_in));
		memcpy(&sel->writeset_out, &sel->writeset_in, sizeof(sel->writeset_in));
		memcpy(&sel->errorset_out, &sel->errorset_in, sizeof(sel->errorset_in));


		int n = select(nfds, &sel->readset_out, &sel->writeset_out, &sel->errorset_out, NULL);
		if (n > 0) {
			int i = 0;
			max = max > nfds ?  nfds : max;
			int idx = 0;
			for (i = 0; i < max; i++) {
				int event_r = 0;
				int event_w = 0;

				e[i].read = 0;
				e[i].write = 0;
				e[i].s = NULL;
				if (FD_ISSET(i, &sel->readset_out)) {
					e[i].read = 1;
					e[i].s = sel->st[i]->ud;
					event_r = 1;
				}

				if (FD_ISSET(i, &sel->writeset_out)) {
					e[i].write = 1;
					e[i].s = sel->st[i]->ud;
					event_w = 1;
				}

				if (FD_ISSET(i, &sel->errorset_out)) {
					e[i].read = 1;
					e[i].write = 1;
					e[i].s = sel->st[i]->ud;
					event_r = 1;
					event_w = 1;
				}

				if (event_r || event_w) {
					idx = i;
				}
			}
			return idx + 1;
		}
		//return n;
	}
	return -1;
}

static void
sp_nonblocking(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	if ( -1 == flag ) {
		return;
	}

	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

#endif
