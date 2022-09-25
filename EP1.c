#include <stdio.h>

int main() {
    
    int n1, n2, n3, n4, n, modo;
    int ehPrimo , soma, fim, i;

    printf("modo: ");
    scanf("%d", &modo);
    
    if(modo == 1) {
        scanf("%d %d %d %d %d", &n1, &n2, &n3, &n4, &n);
            
        printf("n1: \n");
        printf("n2: \n");
        printf("n3: \n");
        printf("n4: \n");
        printf("n: \n");
        
        if(n == n1*n1 + n2*n2 + n3*n3 + n4*n4)
            printf("verdadeiro");
        else
            printf("falso");
    }
    if(modo == 2) {
        printf("n: ");
        scanf("%d", &n);

        n1 = 2;
        n2 = 3;
        n3 = 5;
        n4 = 7;
        fim = 0;

        while(fim == 0) {

            soma = n1*n1 + n2*n2 + n3*n3 + n4*n4;

            if(soma > n) {
                printf("falso");
                fim = 1;
            }
            else if(soma == n) {
                printf("%d %d %d %d\n", n1, n2, n3, n4);
                fim = 1;
            }

            else {

                n1 = n2;
                n2 = n3;
                n3 = n4;
                n4 = n4 + 2;
                ehPrimo = 0;

                while(ehPrimo == 0) {

                    for(i = 3; i < n4; i = i + 2) {

                        if(n4%i == 0) {
                            n4 = n4 + 2;
                            i = 3;
                        }
                    }
                    ehPrimo = 1;
                }
            }
        }

    }
    return 0;
}