#include <stdio.h>

int main() {
    int i = 1;
	int j = i + 33;
	if (i <= 5)
		i = 0;
	while(i > j) {
        while(i < 0) {
		    i = i+1;
        }
	}
	return i;
}
