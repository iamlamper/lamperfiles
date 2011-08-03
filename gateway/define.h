#ifndef _INCLUDE_H
#define _INCLUDE_H

#define DEBUG    					/* DEBUG | RELEASE */
#define USE_FILE                    /* USE_TTY | USE_FILE  ,Use file as virtual tty device at first */
#define SOCK_TX_BUF_SZ 1024			/* size of socket send buffer */
#define SOCK_RX_BUF_SZ 1024			/* size of socket recv buffer */
#define TTY_RX_BUF_SZ  (RECORD_SZ + 1)			/* size of tty read buf */
#define MSG_DATA_SZ TTY_RX_BUF_SZ	/* size of message queue data, no less than TTY_RX_BUF_SZ */

#define ID_LEN 3				/* 节点ID的长度 */
#define TIME_LEN 5				/* 时间戳的长度 */
#define VALUE_LEN 7				/* 电量数值的长度 */
#define RECORD_SZ (ID_LEN + 2*TIME_LEN + VALUE_LEN) /* 每条电量记录的长度 */
#endif

