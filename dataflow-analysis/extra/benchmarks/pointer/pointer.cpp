#include <stdio.h>

int main() {
    int * x;
	int * z;
	int * a;
	int y = 5;
	x = &y;
	z = x;
	if (*x <= 5)
		a = x;
	else
		a = z;
	while(*x > y) {
        while(*x < 0) {
		    *x = y+1;
        }
	}
	return *x;
}
