#include <stdio.h>
int sum(int a, int b) {
    int c = a + b;
    return c;
}

int main() {
    sum(1, 2);
    sum(1, 2);
    printf("%d\n", sum(1, 2));
    return 0;
}
