#ifndef _INCLUDE_H
#define _INCLUDE_H

#define DEBUG    					/* DEBUG | RELEASE */
#define USE_FILE                    /* USE_TTY | USE_FILE  ,Use file as virtual tty device at first */
#define SOCK_TX_BUF_SZ 1024			/* size of socket send buffer */
#define SOCK_RX_BUF_SZ 1024			/* size of socket recv buffer */
#define TTY_RX_BUF_SZ  128			/* size of tty read buf */
#define MSG_DATA_SZ TTY_RX_BUF_SZ	/* size of message queue data, no less than TTY_RX_BUF_SZ */
#endif
