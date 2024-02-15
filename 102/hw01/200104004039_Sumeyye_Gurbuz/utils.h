#ifndef _UTIL_H_
#define _UTIL_H_

int find_divisible(int x, int y, int z);

int find_nth_divisible(int n, int f_i, int z);

int part1(int x, int y, int z);

int validate_identity_number(char identity_number[]);

int create_customer(char identity_number[], int password);

int part2();

int check_login(char identity_number[], int password);

int withdrawable_amount(float cash_amount);

int part3();

#endif
