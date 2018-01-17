/*
 * clock.h
 *
 * Created: 16.01.2018 14:00:51
 *  Author: Matthias Hinrichs
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

void clock_display(int row, int col);
void clock_hour_inc(void);
void clock_hour_dec(void);
void clock_min_inc(void);
void clock_min_dec(void);

#endif /* CLOCK_H_ */