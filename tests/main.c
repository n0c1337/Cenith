#include <stdio.h>

extern int test(void);

int main(void) {
    printf("%d\n", test());
}