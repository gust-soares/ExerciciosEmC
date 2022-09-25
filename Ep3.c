#include <stdio.h>
#include <math.h> // ceil(.), roundf(.)

#define MAX 1000

// ---
// Inicio - NAO alterar estas funcoes

// Funcoes de apoio para uniformizar tratamentos de dados para a turma C
int max_listaI (int L[], int n) {
  int i, m_l = 0;
  for (i=0; i<n; i++) // e in L:
    if (L[i]>m_l) // e > m_l:
      m_l = L[i]; // e
  return m_l;
  }
double max_listaD (double L[], int n) {
  int i;
  double m_l = 0;
  for (i=0; i<n; i++) // e in L:
    if (L[i]>m_l) // e > m_l:
      m_l = L[i]; // e
  return m_l;
  }

void imprimeLista (double vet[], int n) {
  int i;
  for (i=0; i<n; i++) printf("%.4lf ", vet[i]);
   printf("\n");
   }

void leituraParam1 (int *N, float *Beta, float *Gama, int *Tmax) {
  printf("Digite N: \n");    scanf("%d", N);
  printf("Digite Beta: \n"); scanf("%f", Beta);
  printf("Digite Gama: \n"); scanf("%f", Gama);
  printf("Digite Tmax: \n"); scanf("%d", Tmax);
  }

void leituraParam2 (int *N, float *Gama, int *Tmax, float *Beta_MIN, float *Beta_MAX, float *Beta_delta) {
  printf("Digite N: \n");          scanf("%d", N);
  printf("Digite Gama: \n");       scanf("%f", Gama);
  printf("Digite Tmax: \n");       scanf("%d", Tmax);
  printf("Digite Beta_MIN: \n");   scanf("%f", Beta_MIN);
  printf("Digite Beta_MAX: \n");   scanf("%f", Beta_MAX);
  printf("Digite Beta_delta: \n"); scanf("%f", Beta_delta);
  }

// Leia e imprima arquivo PGM (tipo=0) ou PPM (tipo=1)
void imprimaArquivo (char nome_de_arquivo[], int tipo) {
  FILE *file = fopen(nome_de_arquivo, "r");
  char tipoImg[3] = "P2";
  int i, j, x, y, val, r, g, b;
  fscanf(file, "%s", tipoImg); tipoImg[2] = '\0'; printf("%s\n", tipoImg);
  fscanf(file, "%d %d", &x, &y); printf("%d %d\n", x, y);
  fscanf(file, "%d", &val); printf("%d\n", val);
  for (i=0; i<y; i++) {
    for (j=0; j<x; j++) {
      if (tipo==0) { // PGM
         fscanf(file, "%d", &val);
         printf("%d ", val);
         }
      else { // (tipo==1) PPM
         fscanf(file, "%d %d %d", &r, &g, &b);
         printf("%d %d %d ", r, g, b);
         }
      }
     printf("\n");
     }
   fclose(file); // Importante fechar arquivo! (senao leituras posteriores poder gerar "melecas")
   }

// Final - NAO alterar esta funcao
// ---

// Leitura de N, Gama, Tmax, Beta_MIN, Beta_MAX e Beta_delta (nesta ordem) via arquivo
void leituraParamViaArquivo (char nomeArquivo[], int *N, float *Gama, int *Tmax, float *Beta_MIN, float *Beta_MAX, float *Beta_delta) {
    printf("Digite nome do arquivo:\n");
    scanf("%s", nomeArquivo);
    
    FILE *arq = fopen(nomeArquivo, "r");
    fscanf(arq, "%d", N);
    fscanf(arq, "%f", Gama);
    fscanf(arq, "%d", Tmax);
    fscanf(arq, "%f", Beta_MIN);
    fscanf(arq, "%f", Beta_MAX);
    fscanf(arq, "%f", Beta_delta);
    
    fclose(arq);
  }


void SIR (double S[], double I[], double R[], int N, float Beta, float Gama, int Tmax) {
    //double S;
    int i;

    S[0] = N - 1;
    R[0] = 0;
    I[0] = 1;

    for(i = 0; i < Tmax-1; i++) {
        I[i+1] = I[i] + ((Beta*(S[i]*I[i]))/N) - Gama*I[i];
        R[i+1] = R[i] + Gama*I[i];
        S[i+1] = S[i] - ((Beta*(S[i]*I[i]))/N);
    }
  }


int critic_SIR (double c_SIR[], int N, float Gama, int Tmax, float Beta_MIN, float Beta_MAX, float Beta_delta) {
    int i = 0;
    float novoBeta;
    double S[MAX], I[MAX], R[MAX];

    novoBeta = Beta_MIN;
    while(novoBeta <= Beta_MAX) {
        novoBeta = Beta_MIN + i*Beta_delta;
        SIR (S ,I ,R ,N ,novoBeta ,Gama ,Tmax);
        c_SIR[i] = max_listaD (I, Tmax);
        i++;
    }
    return i-1;
}


// Gerar o grafico simples PGM e imprimi-lo (tela ou arquivo: imp=1 => grava arquivo "graf_simples.pgm")
void gera_grafico_simples (int Mat[][MAX], double L[], int X_MAX, int *Y_MAX, int imp) {
  int i, j, Y, m, n, y_max = (int)roundf(max_listaD(L, X_MAX)) + 1; // para uniformizar tratamento (em outros truncamentos usar tambem '(int)roundf(.)'
  int Y_MIN=0, X_MIN=0;
  int M[MAX][MAX];
  
  m = y_max - Y_MIN;
  n = X_MAX - X_MIN + 1;
  if(imp == 0) {
     printf("[");
     for(i = 0; i <= m; i++) {
       printf("[");
          for(j = 0; j < n-1; j++) {
            if(roundf(L[j]) == m-i) {   
                M[i][j] = 255;
                printf("255");
            }
            else {
                M[i][j] = 0;
                printf("0");
            }
            if(j == n-2)    
                printf("]");
            else
                printf(" ,");
            }
         printf(", ");
        }
      printf("]");
    }
  if(imp == 1) {
     for(i = 0; i <= m; i++) {
          for(j = 0; j < n-1; j++) {
            if(roundf(L[j]) == m-i) {   
                M[i][j] = 255;
            }
            else 
                M[i][j] = 0;
            }
        }
     FILE* arq = fopen("graf_simples.pgm", "w");
     fprintf(arq,"P2\n");
     fprintf(arq,"%d %d\n", n-1, m+1);
     fprintf(arq, "255\n");
     for(i = 0; i < m+1; i++) {
         for(j = 0; j < n-1; j++) {
             if(j == n-1)  
               fprintf(arq, "%d\n", M[i][j]);
            else
              fprintf(arq, "%d ", M[i][j]);
        }
    }
    fclose(arq);
  }
}


// Gerar o grafico composto PPM e imprimi-lo (tela ou arquivo: imp=1 => grava arquivo "graf_simples.pgm")
void gera_grafico_composto (int Mat[][MAX], double S[], double I[], double R[], int X_MAX, int imp) {
  int i,j, Y_MAX;
  int vet[3];
  vet[0] = (int)roundf(max_listaD(S, X_MAX)); // para uniformizar
  vet[1] = (int)roundf(max_listaD(I, X_MAX));
  vet[2] = (int)roundf(max_listaD(R, X_MAX));
  Y_MAX = max_listaI(vet, 3) + 1;
  // Implementar
  // Atencao 'a ordem da linhas finais - pode ser necessario usar matriz auxiliar e depois inverter ordem das linhas
  }


// Opcoes
// 1: Calcular 'SIR' e imprimir os vetores S, I e R - leitura de teclado
// 2: Calcular 'critic_SIR' e imprimir o vetor c_SIR - leitura de teclado
// 3: Calcular 'critic_SIR' e imprimir o vetor c_SIR - leitura de arquivo
// 4: Calcular 'critic_SIR', testar matriz devolvida por 'gera_grafico_simples' - leitura de teclado
// 5: Calcular 'critic_SIR', testar arquivo PGM no disco por 'gera_grafico_simples' - leitura de teclado
// 6: Calcular 'SIR', testar matriz devolvida por 'gera_grafico_composto' - leitura de teclado
// 7: Calcular 'SIR', testar arquivo PPM no disco por 'gera_grafico_composto' - leitura de teclado

// Nao altere esta funcao
void main (void) {
  char Dados[100]; // para nome do arquivo
  int Opt; // opcao
  int N, Tmax, tam, y_max;
  float Gama, Beta, Beta_MIN, Beta_MAX, Beta_delta; // parametros dos modelos
  double S[MAX], I[MAX], R[MAX], c_SIR[MAX];
  int Mat[MAX][MAX];
  printf(" Digite modo do programa: \n");
  scanf("%d", &Opt);

  if (Opt == 1) { // saida - SIR; entrada - teclado
    leituraParam1(&N, &Beta, &Gama, &Tmax);
    SIR(S, I, R, N, Beta, Gama, Tmax);
    printf("S = "); imprimeLista(S, Tmax);
    printf("I = "); imprimeLista(I, Tmax);
    printf("R = "); imprimeLista(R, Tmax);
    }
  else
  if (Opt == 2) { // saida - critic_SIR; entrada - teclado
    leituraParam2(&N, &Gama, &Tmax, &Beta_MIN, &Beta_MAX, &Beta_delta);
    tam = critic_SIR(c_SIR, N, Gama, Tmax, Beta_MIN, Beta_MAX, Beta_delta);
    imprimeLista(c_SIR, tam);
    }
  else
  if (Opt == 3) { // saida - critic_SIR; entrada - arquivo
    leituraParamViaArquivo(Dados, &N, &Gama, &Tmax, &Beta_MIN, &Beta_MAX, &Beta_delta); // Dados: nome do arquivo
    tam = critic_SIR(c_SIR, N, Gama, Tmax, Beta_MIN, Beta_MAX, Beta_delta);
    imprimeLista(c_SIR, tam);
    }
  else
  if (Opt == 4) { // grafico simples - critic_SIR; entrada - teclado
    leituraParam2(&N, &Gama, &Tmax, &Beta_MIN, &Beta_MAX, &Beta_delta);
    tam = critic_SIR(c_SIR, N, Gama, Tmax, Beta_MIN, Beta_MAX, Beta_delta);
    gera_grafico_simples(Mat, c_SIR, tam, &y_max, 0);
    }
  else
  if (Opt == 5) { // PGM grafico simples - critic_SIR; entrada - teclado
    leituraParam2(&N, &Gama, &Tmax, &Beta_MIN, &Beta_MAX, &Beta_delta);
    tam = critic_SIR(c_SIR, N, Gama, Tmax, Beta_MIN, Beta_MAX, Beta_delta);
    gera_grafico_simples(Mat, c_SIR, tam, &y_max, 1); // PGM: m x n ; PPM: m x n => 3*m x n  (m=tam e n=max(c_SIR))
    imprimaArquivo("graf_simples.pgm", 0);
    }
  else
  if (Opt == 6) { // grafico composto - SIR; entrada - teclado
    leituraParam1(&N, &Beta, &Gama, &Tmax);
    SIR(S, I, R, N, Beta, Gama, Tmax);
    gera_grafico_composto(Mat, S, I, R, Tmax, 0); // (float Mat[][MAX], float S[], float I[], float R, int X_MAX)
    }
  else
  if (Opt == 7) { // PPM grafico composto - SIR; entrada - teclado
    leituraParam1(&N, &Beta, &Gama, &Tmax);
    SIR(S, I, R, N, Beta, Gama, Tmax);
    gera_grafico_composto(Mat, S, I, R, Tmax, 1); // (float Mat[][MAX], float S[], float I[], float R, int X_MAX)
    imprimaArquivo("graf_composto.ppm", 1);
    }

  }
