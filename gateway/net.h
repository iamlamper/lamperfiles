#ifndef NET_H
#define NET_H

extern int create_connection(const char *server, const char *port);
extern int send_message(int sockfd, const char *message, int msg_size);

#endif
