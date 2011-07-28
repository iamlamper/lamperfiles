#ifndef TTY_H
#define TTY_H

int tty_speed_arr[];
int tty_name_arr[];
extern int open_tty(int tty_num, int i_rate, int o_rate);
extern int set_tty(int fd, int i_rate, int o_rate, int databits, int stopbits, int parity);

#endif
