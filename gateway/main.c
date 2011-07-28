#include <stdio.h>
#include <string.h>
#include <syslog.h>

#include "define.h"
#include "net.h"

int main(int argc, char **argv)
{
	int conn;
	char *message = "GET / HTTP /1.1\r\n\r\n";
	char buf[SOCK_RX_BUF_SZ];

	memset(buf, 0, SOCK_RX_BUF_SZ);
	if (argc == 3)
	  conn = create_connection(argv[1] , argv[2]);
	else
	  conn = create_connection("127.0.0.1", "8000");
	syslog(LOG_INFO, "connection established");

	if (conn != -1)
	  {
		if (send_message(conn, message, strlen(message)) != strlen(message))
		  return -1;
		syslog(LOG_INFO, "sent message:%s", message);		
		while (read(conn, buf, SOCK_RX_BUF_SZ) > 0)
		  printf("%s", buf);
		syslog(LOG_INFO, "read message:%s", buf);
	  }
	getchar();
	return 0;
}
