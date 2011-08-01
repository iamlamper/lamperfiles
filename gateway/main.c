#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "define.h"
#include "net.h"
#include "tty.h"

typedef struct msgbuf 
{
  long mtype;
  char mtext[MSG_DATA_SZ];
} msg_t;

sig_atomic_t time_up = 0;		/* whether the time interval use up */


/**
 *@breif
 *@return
 */
int main(int argc, char **argv)
{
  int tty_fd = -1;
  int msgq_id = -1;
  pid_t chld_pid = -1;
  char *buf = NULL;
  msg_t *msg = NULL;
  int nread = -1;
  int rt_code = 0;				/* default, successfully return */

  /* create a message queue */
  msgq_id = msgget(IPC_PRIVATE, IPC_CREAT); 
  if (msgq_id == -1)
	{
	  fprintf(stderr, "msgget error, errno:%d ", errno);
	  rt_code = -1;
	  goto QUIT;
	}

  /* child process, message queue --> socket*/
  chld_pid = fork();
  switch (chld_pid)
	{
	case -1:
	  fprintf(stderr, "fork error, errno:%d ", errno);
	  rt_code = -1;
	  goto QUIT;
	  break;
	case 0:
	  run_child(msgq_id);				/* child process */
	  break;
	default:
	  break;					/* parent process, continue */
	}

  /* parent, continue with here*/
  buf = (char *)calloc(TTY_RX_BUF_SZ, sizeof(char));
  msg = (msg_t *)calloc(1, sizeof(msg_t));
  if (buf == NULL || msg == NULL)
	{
	  fprintf(stderr, "Memory allocation error.");
	  rt_code = -1;
	  goto QUIT;
	}

  /* open a tty device */
  tty_fd = open_tty(0);	  /* /dev/ttyS0 */
  if (tty_fd == -1)
	{
	  rt_code = -1;
	  goto QUIT;
	}

  while (1)
	{
	  bzero(&(msg->mtext), sizeof(msg->mtext));
	  bzero(buf, sizeof(buf));
	  msg->mtype = 1;			/* not used, currently*/

	  nread = read(tty_fd, buf, sizeof(buf));
	  switch (nread)
		{
		case -1:
		  fprintf(stderr,"read error, errno:%d", errno);
		  rt_code = -1;
		  goto QUIT;
		  break;
		case 0:
		  continue;
		  break;
		default :
		  memcpy(&(msg->mtext), buf, sizeof(msg->mtext)); /* DON'T use strlen(buf) */
		  break;
		}

	  /* put into msg queue */
	  if (msgsnd(msgq_id, msg, strlen(msg->mtext), 0) == -1)
		{
		  fprintf(stderr, "msgsnd error, errno:%d", errno);
		  rt_code = -1;
		  goto QUIT;
		} 
	}
 QUIT:
  if (buf == NULL)
	free(buf);
  if (msg == NULL)
	free(msg);
  return rt_code;
}


/** local function, alarm signal handler */
void _alrm_hdlr(int signo)
{
  if (signo == SIGALRM)
	time_up = 1;					/* set this flag */
  return ;
}

/**
 *@brief
 *@param
 *@return
 */
int run_child(int msgq_id)
{
  unsigned interval;					/* send msg to server each interval seconds. */
  char sock_buf[SOCK_TX_BUF_SZ]; /* socket rw buffer */
  unsigned buf_end = 0;
  msg_t  msg;
  int rt_code = 0;				/* successfully return, by default */
  int len = -1;
  int sock_fd = -1;				/* socket fd */
  struct itimerval itv;
  char http_buf[512 + SOCK_TX_BUF_SZ];
  char url[1024];				/* 1K should be enough for us */
  char host[1024];

  /* set timer */
  interval = 6;					/* 6 seconds, by default */
  if (signal(SIGALRM, _alrm_hdlr) == SIG_ERR)
	{
	  fprintf(stderr, "signal error, errno:%d", errno);
	  rt_code = -1;
	  goto QUIT;
	}
  itv.it_interval.tv_sec = interval;
  itv.it_interval.tv_usec = 0;
  memcpy(&itv.it_value, &itv.it_interval, sizeof(itv.it_interval));
  if (setitimer(ITIMER_REAL, &itv, NULL) == -1)					/* set the timer */
	{
	  fprintf(stderr, "setitimer error, errno:%d", errno);
	  rt_code = -1;
	  goto QUIT;
	}

  /* child process main loop */
  bzero(&sock_buf, sizeof(sock_buf));
  while (1)
	{
	  /* time cycle begin */
	  if (buf_end == 0)
		{
		  snprintf(sock_buf, 2, "["); /* [.....  */
		  buf_end ++;
		}

	  /* time cycle end, send msg */
	  if (time_up == 1)
		{
		  snprintf(&sock_buf[buf_end], 2, "]"); /* should have:buf_end <= BufferSize-2 */
		  /* pack http msg */
		  if (pack_msg(http_buf, sizeof(http_buf), sock_buf, sizeof(sock_buf), url, host) == -1)
			{
			  /* print or log errors, but don't quit!!! */
			  fprintf(stderr, "pack_msg error\n");
			  buf_end = 0;
			  time_up = 0;
			  continue;
			}

		  /* http request */
		  if (http_req(sock_fd, http_buf, strlen(http_buf)) == -1)
			{
			  fprintf(stderr, "http_req error\n");
			  buf_end = 0;
			  time_up = 0;
			  continue;
			}
		  
		  /* http response */
		  bzero(http_buf, sizeof(http_buf));
		  if (wait_resp(sock_fd, http_buf, sizeof(http_buf)) == -1)
			{
			  fprintf(stderr, "wait_resp error\n");
			  /* suppose it succeed */
			  buf_end = 0;
			  time_up = 0;
			  continue;
			}
		  else 
			{
			  /* parse response */
			  if (0)
				{				/* ...... */
				  /* if not successful, log it.*/
				}
			}
		}

	  bzero(&msg, sizeof(msg));
	  /* read msg */
	  if (msgrcv(msgq_id, &msg, sizeof(msg), 0, 0) == -1)
		{
		  fprintf(stderr, "msgrcv error, errno:%d", errno);
		  rt_code = -1;
		  goto QUIT;
		}
	  if (buf_end + strlen(msg.mtext) + 3 <= sizeof(sock_buf) - 1) /* 本来是<=size-2,但是因为第一组值前可以少一个逗号号，所以多出一个空间 */
		{
		  if (buf_end != 1)
			{
			  snprintf(&sock_buf[buf_end], 2, ",");
			  buf_end ++;
			}
		  len = snprintf(&sock_buf[buf_end], sizeof(sock_buf) - buf_end, "[%s]", msg.mtext);
		  buf_end += len;
		}
	  else						/* no enough buffer space, ignore this record, send buffer ASAP */
		{
		  time_up = 1;
		}
	}

 QUIT:
  return rt_code;
}
