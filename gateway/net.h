#ifndef NET_H
#define NET_H

extern int create_connection(const char *server, const char *port);
extern int http_req(int sockfd, const char *message, int msg_size);
extern int pack_msg(char *, int, const char *, int, const char*, const char*);
extern int wait_resp(int, char *, int );
#endif
