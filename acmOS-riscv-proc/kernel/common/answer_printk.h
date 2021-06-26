//
// Created by Wenxin Zheng on 2021/4/21.
//

#ifndef ACMOS_SPR21_ANSWER_PRINTK_H
#define ACMOS_SPR21_ANSWER_PRINTK_H

static void printk_write_string(const char *str) {
    // Homework 1: YOUR CODE HERE
    // this function print string by the const char pointer
    // I think 3 lines of codes are enough, do you think so?
    // It's easy for you, right?
    for(int i = 0; str[i] != '\0'; i++)uart_putc(str[i]);
}


static void printk_write_num(int base, unsigned long long n, int neg) {
    // Homework 1: YOUR CODE HERE
    // this function print number `n` in the base of `base` (base > 1)
    // you may need to call `printk_write_string`
    // you do not need to print prefix like "0x", "0"...
    // Remember the most significant digit is printed first.
    // It's easy for you, right?
    if(neg)uart_putc('-');
    unsigned long long m, _n = n;
    for(m = 1; n / m >= base; m *= base);
    for(; m; _n %= m, m /= base)
        uart_putc(_n / m < 10 ? (int)(_n / m) + 48 : (int)(_n / m) + 87);
}

#endif  // ACMOS_SPR21_ANSWER_PRINTK_H