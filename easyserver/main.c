#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 500

int main(int argc, char **argv)
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;
  struct sockaddr peer_addr;
  socklen_t peer_addr_len;
  ssize_t nread;
  char buf[BUF_SIZE];
  
  if (argc != 2) 
	{
	  fprintf(stderr, "Usage: %s port\n", argv[0]);
	  exit(EXIT_FAILURE);
	}

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family		= AF_UNSPEC;
  hints.ai_socktype		= SOCK_STREAM;
  hints.ai_flags		= AI_PASSIVE;
  hints.ai_protocol		= 0;
  hints.ai_canonname	= NULL;
  hints.ai_addr			= NULL;
  hints.ai_next			= NULL;
  
  s = getaddrinfo(NULL, argv[1], &hints, &result);
  if (s != 0)
	{
	  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
	  exit(EXIT_FAILURE);
	}

  for (rp = result; rp != NULL; rp = rp->ai_next)
	{
	  sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	  if (sfd == -1)
		continue;
	  if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
		if (listen(sfd, 128) == 0)
		  break;					/* ok */
	  close(sfd);				/* fail */
	}
  syslog(LOG_INFO, "listening on socket:%d", sfd);
  if (rp == NULL)
	{
	  fprintf(stderr, "Could not bind\n");
	  exit(EXIT_FAILURE);
	}
  freeaddrinfo(result);

  for (;;)
	{
	  /*	  s = accept(sfd, &peer_addr, &peer_addr_len); */
	  s = accept(sfd, NULL, NULL);
	  syslog(LOG_INFO, "accept fd %d:", s);
	  if (s == -1)
		{
		  fprintf(stderr, "Errorno: %d", errno);
		}
	  syslog(LOG_INFO, "waiting to read....");
	  while ((nread = read(s, buf, BUF_SIZE)) > 0)
		{
		  syslog(LOG_INFO, "nread = %d", nread);
		  syslog(LOG_INFO, "%s", buf);
		  write(s, buf, nread);
		  /* close s*/
		}

	}
}
	  
