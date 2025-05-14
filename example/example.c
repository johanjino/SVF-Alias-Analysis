// void swap(char **p, char **q){
//   char* t = *p; 
//        *p = *q; 
//        *q = t;
// }
// int main(){
//       char a1, b1; 
//       char *a = &a1;
//       char *b = &b1;
//       swap(&a,&b);
// }

#include <stdio.h>
#include <stdlib.h>

int main() {
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));

    if (rand() % 2) {
        a = b;
    }

    *a = 42;   // Load/Store 1
    *b = 100;  // Load/Store 2

    printf("%d %d\n", *a, *b);
    return 0;
}