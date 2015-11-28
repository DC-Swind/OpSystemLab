#include "x86.h"
#include "device.h"
#include "protect.h"
/*
void test(const char *format,int *d,int *c){
	const char *p = format;
	int addr = (int) p;
	while(addr>0){
		putchar('0'+addr%10);
		addr /= 10;
	}
	putchar('\n');
	addr = (int) &d;
	while(addr>0){
		putchar('0'+addr%10);
		addr /= 10;
	}
	putchar('\n');
	while (*d){
		int n = *d;
		while(n>0){
			putchar('0'+n%10);
			n /= 10;
		}
		d++;
	}
	putchar('\n');
	int addr1 = (int)&format;
	while (addr1 >0){
		putchar('0'+addr1%10);
		addr1 /= 10;
	}
	putchar('\n');
	int addr3 = (int)&c;
	while (addr3 >0){
		putchar('0'+addr3%10);
		addr3 /= 10;
	}
	

	
}
*/
int output_d(int num){
	int count;
	if(num >0) count = output_d(num/10); else return 0;
	putchar('0'+num%10);
	return count+1;
}
int output_x(int num){
	int count;
	if(num >0) count = output_x(num/16); else return 0;
	int temp = num%16;
	if (temp < 10) putchar('0'+temp);else
	if (temp == 10) putchar('a');else
	if (temp == 11) putchar('b');else
	if (temp == 12) putchar('c');else
	if (temp == 13) putchar('d');else
	if (temp == 14) putchar('e');else
	if (temp == 15) putchar('f');
	return count+1;
}
int printf(const char *format, ...){
	const char *p = format;
	void *base = (void *)&format;
	int count = 0;
	//int addr = (int)&p;
	//output_d(addr);
	//putchar('\n');
	while(*p){
		if (*p == '%'){
			p++;
			if(*p == 'd'){
				base+=4;
				int num = *(int*)base;
				if (num < 0){
					putchar('-');
					num = 0-num;
					count++;
				}
				if (num == 0){
					putchar('0');
					count++;
				}else count += output_d(num);
			}
			if(*p == 'x'){
				base+=4;
				int num = *(int*)base;
				if (num < 0){
					putchar('-');
					num = 0-num;
					count++;
				}
				if (num == 0){
			 		putchar('0');
					count++;
				}else count += output_x(num);
			}
			if(*p == 's'){
				base+=4;
				char *s = (char *)((unsigned long)*(int *)base);
				while(*s){
					putchar(*s);
					s++;
					count++;
				}
			}
			if(*p == 'c'){
				base+=4;
				char c = *(char *)base;
				putchar(c);
				count++;
			}
			if(*p == '%'){
				putchar('%');
				count++;
			}
		}else{ putchar(*p); count++;}
		p++;
	}
        return count;
}
