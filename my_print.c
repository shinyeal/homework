/*************************************************************************
> File Name: my_print.c
> Author: 
> Mail: 
> Created Time: 2018年09月20日 星期四 20时31分18秒
************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <inttypes.h>

int64_t my_printf(const char *frm, ...) {
    int cnt = 0;
    va_list arg;
    va_start(arg, frm);
    for (int i = 0, cnt = 0; frm[i]; i++, cnt++) {
        switch (frm[i]) {
            case '%' : {
                i++;
                switch(frm[i]) {
                    case 'd' : {
                        int64_t temp = va_arg(arg, int), x = 0;
                        if(temp < 0) {
                            putchar('-');
                        }
                        int count = 0;
                        temp = fabs(temp);
                        while (temp) {
                            x = x * 10 + temp % 10;
                            if(x == 0) {
                                count++;
                            }
                            temp /= 10;
                        }
                        while (x) {
                            putchar(x % 10 + '0');
                            x /= 10;
                        } 
                        for(int i = 1; i <= count; i++) {
                            putchar('0');
                        }
                        break;
                    }
                    default : 
                    fprintf(stderr, "error : unknow %%%c\n", frm[i]); 
                    exit(1);

                }
            } break;
            default:
            putchar(frm[i]);
        }
    }
    return cnt;
}

    int main() {
        int n = 123;
        my_printf("hello world\n");
        my_printf("n = %d\n", n);
        my_printf("n = %d\n", 12000);
        my_printf("n = %d\n", -567);
        my_printf("n = %d\n", INT32_MIN);
        my_printf("n = %d\n", INT32_MAX);
        return 0;

    }
