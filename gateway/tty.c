#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include "tty.h"

int tty_speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
          B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int tty_name_arr[] = {38400,  19200,  9600,  4800,  2400,  1200,  300, 38400,  
          19200,  9600, 4800, 2400, 1200,  300, };

/**
 *@brief 打开并设置一个串口,串口设置为RAW模式,其他串口参数设置均按默认
 *@param int tty_num 串口号，第一个串口为0, 第二个为1,依此类推
 *@return int 成功返回文件描述符，失败返回-1
 */
int open_tty(int tty_num)
{
  char tty_name[11];			/* /dev/ttyS[0,1,2,....] */
  int tty_fd;
  struct termios ts;
  sprintf((char *)tty_name, "/dev/ttyS%d", tty_num);
  tty_fd = open(tty_name, O_RDWR);
  if (tty_fd == -1)
	{
	  fprintf(stderr, "open tty error, errno:%d", errno);
	  return -1;
	}
  if (tcgetattr(tty_fd, &ts) == 0)
	{
	  cfmakeraw(&ts);
	  if (tcsetattr(tty_fd, TCSANOW, &ts) == -1)
		{
		  fprintf(stderr, "tcsetattr error, errno:%d", errno);
		  return -1;
		}

	  /* test 
	  printf("\ndefault vmin %d, default vtime %d\n", ts.c_cc[VMIN], ts.c_cc[VTIME]);
	  ts.c_cc[VMIN] = 8;
	  ts.c_cc[VTIME] = 100;
	  tcsetattr(tty_fd, TCSANOW, &ts);
	  tcgetattr(tty_fd, &ts);
	  printf("\nNew default vmin %d, default vtime %d\n", ts.c_cc[VMIN], ts.c_cc[VTIME]);
	  test */
	}
  else 
	{
	  fprintf(stderr, "tcgetattr error, errno:%d", errno);
	  return -1;
	}
  
  return tty_fd;
}

/**
 *@brief 修改串口的波特率、奇偶检验等参数
 *@param int fd 打开的串口的文件描述符
 *@param int i_rate 波特率 (man termios.h)
 *@param int o_rate 波特率
 *@param int databits 数据位 取值7或8
 *@param int stopbits 停止位 取值为1或2
 *@param int parity 检验类型 N，E，O，S
 *@return   
 */
int set_tty(int fd, int i_rate, int o_rate, int databits, int stopbits, int parity)
{
  struct termios tty_opt;			/* tty option */
  short i;

  if (tcgetattr(fd, &tty_opt) != 0)
	{
	  fprintf(stderr, "tcgetarr error, errno:%d", errno);
	  return -1;
	}

  tty_opt.c_cflag &= ~CSIZE;
  switch (databits)
	{
	case 7:
	  tty_opt.c_cflag |= CS7;
	  break;
	case 8:
	  tty_opt.c_cflag |= CS8;
	  break;
	default:
	  fprintf(stderr, "data size error, data size =%d", databits);
	}

  switch (parity)
	{
	case 'n':
	case 'N':
	  tty_opt.c_cflag &= ~PARENB;
	  tty_opt.c_iflag &= ~INPCK;
	  break;
	case 'o':
	case 'O':
	  tty_opt.c_cflag |= (PARODD | PARENB);
	  tty_opt.c_iflag |= INPCK;
	  break;
	case 'e':
	case 'E':
	  tty_opt.c_cflag |= PARENB;
	  tty_opt.c_cflag &= ~PARODD;
	  tty_opt.c_iflag |= INPCK;
	case 's':
	case 'S':
	  tty_opt.c_cflag &= ~PARENB;
	default:
	  fprintf(stderr, "Unsupported parity");
	  return -1;
	}

  switch(stopbits)
	{
	case 1:
	  tty_opt.c_cflag &= ~CSTOPB;
	  break;
	case 2:
	  tty_opt.c_cflag |= CSTOPB;
	  break;
	default:
	  fprintf(stderr, "unsupported stop bits");
	  return -1;
	}

  for (i=0; i<sizeof(tty_speed_arr)/sizeof(int); i++)
	{
	  if (i_rate == tty_name_arr[i])
		{
		  if (cfsetispeed(&tty_opt, tty_speed_arr[i]) == -1)
			{
			  fprintf(stderr, "cfsetispeed error, errno:%d", errno);
			  return -1;
			}
		}
	  if (o_rate == tty_name_arr[i])
		{
		if (cfsetospeed(&tty_opt, tty_speed_arr[i]) == -1)
		  {
			fprintf(stderr, "cfsetospeed error, errno:%d", errno);
			return -1;
		  }
		}
	}

	if (parity != 'n' && parity != 'N')
	  {
		tty_opt.c_iflag != INPCK;
		tcflush(fd, TCIFLUSH);
	  tty_opt.c_cc[VTIME] = 150; /* set time delay 15 seconds */
	  tty_opt.c_cc[VMIN] = 0;
	  if (tcsetattr(fd, TCSANOW, &tty_opt) != 0)
		{
		  fprintf(stderr, "tcsetattr error,errno %d", errno);
		  return -1;
		}
	}
  return 0;						/* success */
}   
    
