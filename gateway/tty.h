
#ifndef TTY_H
#define TTY_H

extern int tty_speed_arr[];
extern int tty_name_arr[];
extern int open_tty(int tty_num);
extern int set_tty(int fd, int i_rate, int o_rate, int databits, int stopbits, int parity);

#endif
