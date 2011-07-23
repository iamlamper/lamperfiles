
#include <stdio.h>
#include <sys/socket.h>
#include <assert.h>

#include "define.h"
#include "net.h"


/**
 *@brief 创建一个Socket，并与指定服务器的指定端口建立一个TCP连接
 *@param server char* 要连接的服务器地址，可以使用IP地址，也可以使用域名
 *@param port char* 服务器端口号。
 *@return socket描述符，失败则返回-1
*/
int create_connection(const char *server, const char *port)
{
  int skt;
  int temp;
  struct addrinfo hints;
  struct addrinfo *result;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family		= AF_UNSPEC;
  hints.ai_socktype		= SOCK_STREAM;
  hints.ai_flags		= 0;
  hints.ai_protocol		= 0;
  
  temp = getaddrinfo(server, port, &hints, &result); /* should return 0, if success */
  if (temp != 0)
	{
#ifdef DEBUG
	  fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(temp));
#endif
	  return -1;
	}

  /* getaddrinfo()返回一个地址链表，我们就用返回的第一个地址 */
  skt = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (skt == -1)
	{
#ifdef DEBUG
	  fprintf(stderr, "socket() failed.");
#endif
	  close(skt);
	  return -1;
	}

  if (connect(skt, result->ai_addr, result_addrlen) == -1)
	{
#ifdef DEBUG
	  fprintf(stderr, "connect() failed.");
#endif
	  close(skt);
	  return -1;
	}
  freeaddrinfo(result);
  return skt;
}

/**
 *@brief 向指定的TCP端口发送消息
 *@param 
 *@param
 *@return
 */
int send_message(int sockfd, char *message, int msg_size)
{
  
}
