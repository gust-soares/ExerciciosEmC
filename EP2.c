#include <stdio.h>
#include <stdlib.h> // para 'malloc(int)' na funcao 'polinomioToStringF'
#include <string.h> // para 'strlen' nas funcoes 'ftoa' e 'polinomioToStringF', 'strcat' na funcao 'polinomioToStringF'
#include <math.h>   // para 'pow' na funcao 'ftoa(...)'

#define MAX 50 // tamanho maximo do polinomio

// ======================================================================
// FUNCOES OBRIGATORIAS PRONTAS
// Estas funcoes devem ser utilizadas para simplificar a codificacao em C,
// use-as sem efetuar qualquer alteracao nelas.
// ======================================================================

// ======================================================================
// Usar para lista raizes (para nao gerar problema na avaliacao)
void listaCoeficientes (float p[], int np) {
  int i;
  printf("[ ");
  for (i=0; i<np; i++)
    printf("%f, ", p[i]);
  printf(" ]\n");
  }

// ======================================================================
// Devolve '+' se coef nao e' negativo e existe termo anterior ao termo dele no polinomio.
// Devolve nada em caso contrario. Usado para determinar se o '+' deve aparecer antes
// de coef na string que representa o polinomio.
// nTermAnte -- expoente de x no termo anterior ao termo do coef
// coef -- coeficiente de um termo do polinomio
char sig (int nTermAnte, float coef) {
  if (nTermAnte > 0 && coef >= 0)
    return '+';
  else
    return 0; // nada!
  }

// Inverter caracteres em "string" 'str' de comprimento 'tam'.
// Para invocar, supondo "char pal[MAX]; int t;", use: inverterString(pal, t);
// str[] -- vetor a ser iniciado com 0.0
// tam   -- numero de elementos de vetor
void inverterString (char *str, int tam) {
  int i = 0, j = tam - 1, temp;
  if (str[0]=='-') // tem sinal => ignore posicao 0 (evita que "-201" torne "102-")
    i = 1;
  while (i < j) {
    temp = str[i];
    str[i] = str[j];
    str[j] = temp;
    i++;
    j--;
    }
  }

// Converte inteiro em "string": dado inteiro 'x' para uma "string" 'str[]' com os digitos em 'x'.
// x -- e' o inteiro a ser convertido em "string"
// str[] -- a "string" que recebera' cada digito de 'x' (na ordem correta)
// d -- e' o numero de digitos a serem usado, se 'd' maior que digitos em x, '0' sao adicionados 'a esquerda
// Devolve numero de caracteres em 'str[]'.
int intToStr (int x, char str[], int d) {
  int i = 0;
  if (x<0) { str[i++] = '-'; x = -x; }
  if (x==0) str[i++] = '0';
  while (x) {
    str[i++] = (x % 10) + '0';
    x = x / 10;
    }
  // Se numero de digitos exigido e' maior, adicione zeros '0' no inicio
  while (i < d)
    str[i++] = '0';
  str[i] = '\0';
  inverterString(str, i);
  return i;
  }// int intToStr(int x, char str[], int d)

// Converte numero "flutuante" em "string" correspondente.
// Baseado em: https://www.geeksforgeeks.org/convert-floating-point-number-string
// Para invocar, supondo "float f; char pal[MAX]; int t;", use: ftoa(f, pal, t)
// floatn -- "flutuante" a ser convertido
// res -- "string" que recebera' sinal, cada digito e eventual '.' do "flutuante"
// precisao -- numero de digitos apos '.' ("ponto decimal"), se 0, entao gera como inteiro
void ftoa (float floatn, char *res, int precisao) {
  int ipart = (int)floatn; // pegar parte inteira
  float fpart = floatn - (float)ipart; // pegar parte flutuante
  int i, negativo = 0, tam;
  i = intToStr(ipart, res, 0); // converte a parte inteira para "string" 'res' ('i' recebe numero de digitos)
  if (precisao != 0) { // verifica se tem digitos apos '.'
    res[i] = '.'; // adiciona ponto '.'
    // Tornar a parte decimal em equivalente inteiro (condirando a 'precisao').
    // Se ".100213" e 'precisao'=5, entao 'fpart' recebe 10021
    fpart = fpart * pow(10, precisao);
    if (fpart<0) { // tratar negativo
      fpart = -fpart; // evita "0.-xxx"
      negativo = 1;
      }
    intToStr((int)fpart, res + i + 1, precisao);
    }
  if (negativo == 1 && res[0]!='-') { // evita que 2/3 produza "0.6666" -> "-0.6666" (adiciona '-')
    tam = strlen(res);
    for (i=tam; i>0; i--) res[i] = res[i-1];
    res[0] = '-';
    res[tam] = '\0';
    }
  } // void ftoa(float floatn, char* res, int precisao)

// Devolve uma string que representa o polinomio em um formato legivel.
// Atencao, cuidado com polinomios como "p(x)=-x^2+1" que apos somar 1 torna-se "2" (aqui deve chegar p={1} apenas).
// Para invocar, supondo "float coef[MAX]; int n;", use: printf("%s\n", polinomioToStringF(coef,n));
// p -- o vetor dos coeficientes do polinomio
// np -- numero de coeficientes do polinomio
char *polinomioToStringF (float p[], int np) {
  char *expressao = malloc(sizeof(char)*100), str[] = "                         ", sinal;
  int m, tam = 0; //expressao = ""
  float val;
  np -= 1; //L x^2 + x - 2
  expressao[0] = '\0';
  tam = 0;
  for (m=0; m<np-1; m++) { // m in range(n-1):
    if (np-m<0 || np-m>np) { printf("polinomioToStringF: Erro: np-m=%d (m=%d)\n", np-m,m); return expressao; } else
    val = p[np-m];
    sinal = sig(m,val);
    if (sinal!=0) { expressao[tam++] = sinal; expressao[tam] = '\0'; }
    ftoa(val, str, 4);
    tam += strlen(str);
    strcat(expressao, str); // expressao <- expressao "+" str
    expressao[tam++] = 'x';
    expressao[tam++] = '^';
    expressao[tam] = '\0';
    intToStr(np-m, str, 0);
    strcat(expressao, str);
    tam += strlen(str);
    }
  if (np>0 && p[1] != 0) {
    sinal = sig(np-1,p[1]);
    if (sinal!=0) { tam = strlen(expressao); expressao[tam++] = sinal; expressao[tam] = '\0'; }
    ftoa(p[1], str, 4);
    tam += strlen(str);
    strcat(expressao, str);
    expressao[tam++] = 'x';
    expressao[tam] = '\0';
    }
  if (p[0] != 0) {
    sinal = sig(np,p[0]);
    if (sinal!=0) { expressao[tam++] = sinal; expressao[tam] = '\0'; }
    ftoa(p[0], str, 4);
    tam += strlen(str);
    strcat(expressao, str);
    }
  return expressao;
  }

// Pode ser usada para conseguir gerar racionais a partir de ponto flutuante.
// Dado um numero flutuante parteFlut, encontra a potencia 'pot' para gerar a
// parte inteira correspondente em 'flutInt'.
// Para invocar, supondo "float pf; int p,f;", use: encontraInteiro(pf, &p, &f);
// Exemplo: parteFlut=1.020300 => pot=10000, floatInt=10203, ou seja,
//          pot*0.020300 = 10000*0.020300 = 203.00 = floatInt-pot
void encontraInteiro (float parteFlut, int *pot, int *flutInt) {
  int potA = 1;
  if (parteFlut<0) parteFlut = -parteFlut;
  while (parteFlut-(int)parteFlut > 0) {
    potA *= 10;
    parteFlut *= 10;
    }
  *flutInt = (int)parteFlut;
  *pot = potA;
  }

// ======================================================================
// FIM DO BLOCO DE FUNCOES OBRIGATORIAS PRONTAS
// ======================================================================


// ======================================================================
// FUNCOES ADICIONAIS
// Implemente neste bloco as funcoes adicionais as obrigatorias do EP2.
// ======================================================================

// digite aqui suas funcoes auxiliares...
void lePolinomio(float p[], int grau) {
    int  i;

    printf("Digite os %d coeficientes: ", grau+1);
    for(i = 0; i < grau+1; i++) {
        scanf("%f", &p[i]);
    }
}
void impressaoOpcao2(float r,int *num, int*den) {
     

    if((int)*den == 1 && r > 0)
        printf("%d\n", (int)*num);
    if((int)*den ==1 && r <0) {
        *num = -(int)*num;
        printf("%d\n", (int)*num);
    }
    if(r > 0 && (int)*den != 1) {
        printf("%d/%d\n", (int)*num, (int)*den);
    }
    if(r < 0 && (int)*den != 1) {
        *num = -(int)*num;
        printf("%d/%d\n", (int)*num, (int)*den);
    }
}


// ======================================================================
// FIM DO BLOCO DE FUNCOES ADICIONAIS
// ======================================================================


// ======================================================================
// FUNCOES OBRIGATORIAS
// Implemente  neste bloco as funcoes obrigatorias do EP2.
// NAO modifique os nomes e parametros dessas funcoes.
// ======================================================================

// Dado o vetor p[] descrevendo um polinomio, avalia-lo em um ponto x dado.
// O valor da avaliacao deve ser devolvido como 'float'.
// p[] -- coeficientes do polinomio p(x)
// x -- valor no qual deve ser avaliado p(x)
// np -- grau do polinomio p(x)
float avaliaPolinomio (float p[], float x, int np) { // fazer um for na main pra testar 6 valores
    int i;
    float valor;

    valor = p[np];

    for(i = np-1; i >= 0; i--) {
        valor = valor*x + p[i];
    }
    return valor;
  }

// Dado o vetor p2[] descrevendo um polinomio, fazer copia dele em p1[]
// p1[] -- coeficientes do polinomio, que recebera' copia do polinomio p2(x)
// p2[] -- coeficientes do polinomio p2(x)
// np1, np2 -- grau dos polinomios p1(x) e p2(x)
void copiaPolinomio (float p1[], float p2[], int np1, int np2) { // p1 <- p2
    int i;
    float aux[MAX];
    if(np1<np2) {
        for(i = 0; i < np1+1; i++) {
            p1[i] = p2[i];
            p2[i] = p1[i] + 1;
        }
    }
    else {
        for(i = 0; i < np2+1; i++) {
            p1[i] = p2[i];
            p2[i] = p1[i] + 1;
        }
    }

}


// Dados inteiros N e D apontados por *b e *a, obter a forma reduzida de N/D
// Para invocar, supondo "int n,d", use: reduzRacional(&n, &d);
// *b -- apontador para numerador
// *a -- apontador para denominador
void reduzRacional (int *b, int *a) {
  int i;

    if((int)*b > (int)*a) {
        for(i = 2; i <= (int)*a; i++) {
            while((int)*b % i == 0 && (int)*a % i == 0) {
                *b = (int)*b/i;
                *a = (int)*a/i;
            }
        }
    }
    if((int)*a > (int)*b) {
        for(i = 2; i <= (int)*b; i++) {
            while((int)*b % i == 0 && (int)*a % i == 0) {
                *b = (int)*b/i;
                *a = (int)*a/i;
            }
        }
    }
}

// Computa 2 inteiros que representam a forma racional reduzida (menor possivel)
// da raiz r do polinomio, i.e., *num/*den sao primos entre si. Usar a funcao 'encontraInteiro'.
// Para invocar, supondo "float r, int n,d", use: racionalReduzido(r, &n, &d);
// r -- uma raiz do polinomio
// *num, *den -- devolver resultado nos apotados, com a forma racional reduzida de r
void racionalReduzido (float r, int *num, int *den) {
   
    encontraInteiro(r, den, num);
    
    if(r > 0 && (int)*den != 1) {
        reduzRacional(den, num);
    }
    if(r < 0 && (int)*den != 1) {
        reduzRacional(den, num);
    }
    impressaoOpcao2(r, num, den);
}

// Devolve 1 (Verdadeiro) se b eh raiz do polinomio representado pelo vetor p e 0 (Falso) em caso contrario.
// p[] -- coeficientes do polinomio p(x)
// x -- valor no qual deve ser avaliado p(x)
// np -- grau do polinomio p(x)
int polinomioComRaiz (float p[], float x, int np) { // se p(b)=0, entao devolva 1; senao devolva 0
    if(avaliaPolinomio(p, x, np) == 0)  
        return 1;
    else
        return 0;
}

// Computa o vetor que representa o polinomio quociente da divisao
// p(x)/(x-b), onde p(x) e' o polinomio cujos coeficientes estao no
// vetor p e raiz e' uma raiz de p(x).
// p[] -- coeficientes do polinomio p(x) a ser dividido
// q[] -- coeficientes do polinomio que recebera' o polinomio quociente q(x)
// raiz -- a raiz a ser usada como divisor
// np, nq -- grau dos polinomios p(x) e q(x)
int polinomioQuociente (float p[], float q[], float raiz, int np, int nq) {
  int k;
    float resto, aux[np+1];
    raiz = -raiz;
    if(np == 0) {
        return -1;
    }
    else {
        for(k = 0; k <= np; k++) {
            aux[k] = p[k];
            q[k] = 0;
        }
        for(k = np-1; k >= 0; k--) {
            q[k] = aux[k+1];
            aux[k] = aux[k] - raiz*q[k];

        }
        resto = aux[0];

    }
} // int polinomioQuociente(float p[], float q[], float raiz, int np, int nq)


// Computa o vetor canonico de raizes inteiras do polinomio p(x),
// devolvendo o numero de suas raizes
// p -- o vetor dos coeficientes do polinomio
// raizes -- o vetor com as raizes do polinomio
// np -- grau do polinomio p(x)
int listaCanonicaDeRaizes (float p[], float raizes[], int np) {
    int i, j=0;

    for(i = 0; i < MAX; i++) {
        if(polinomioComRaiz(p, i, np) == 1) {
            raizes[j] = i;
            j++;
        }
    }

    if(j == 0) {
        return 0;
    }

} // int listaCanonicaDeRaizes(float p[], float raizes[], int np)


// Computa o vetor que representa o polinomio quociente da divisao
// p(x)/(ax-b) e o resto dessa divisao, onde p(x) e o polinomio
// cujos coeficientes estao no vetor p e b/a e uma raiz de p(x).
// p[] -- o vetor dos coeficientes do polinomio a ser dividido
// q[] -- coeficientes do polinomio que quociente q(x) (recebera' o polinomio quociente)
// b -- numerador da raiz a ser usada como divisor
// a -- denominador da raiz a ser usada como divisor
// np -- grau do polinomio p(x)
float polinomioQuocienteRacional (float p[], float q[], int b, int a, int np) {
 int k;
    float raiz, resto, aux[np+1];
    b = -b;
    if(np == 0) {
        return -1;
    }
    else {
        for(k = 0; k <= np; k++) {
            aux[k] = p[k];
            q[k] = 0;
        }
        for(k = np-1; k >= 0; k--) {
            q[k] = aux[k+1]/a;
            aux[k] = aux[k] - b*q[k];
     
        }
        resto = aux[0];
   
        return resto;
    }
}

// ======================================================================
// FIM DO BLOCO DE FUNCOES OBRIGATORIAS
// ======================================================================


// ======================================================================
// INICIO DO BLOCO DE FUNCOES PARA EXECUTAR CADA OPCAO
// ======================================================================

// Implementar aqui as 7 funcoes, uma para cada opcao (0 ate' 6).
// Dada as caracteristica da funcoes para opcoes, as impressoes devem estar nelas.

// ======================================================================
// FIM DO BLOCO DE FUNCOES PARA EXECUTAR CADA OPCAO
// ======================================================================


// ======================================================================
// FUNCAO MAIN
// Escreva dentro da funcao main() o codigo que quiser para testar suas demais funcoes.
// Somente dentro da funcao main() e funcoes de opcao voce pode usar as funcoes printf ou scanf.
// O codigo da funcao main() NAO sera avaliado.
// ======================================================================
int main (void) {
  // Declare aqui as variaveis importantes.
  // Implemente a leitura de opcao (0 ate' 6) e as chamadas para as funcoes
  // que implentou para executar cada opcao.
  int opcao, grau, i;
  float p[MAX], valor, p1[MAX], p2[MAX], raiz, q[MAX], raizes[MAX];
  int x, y;
    
  printf("Digite opcao:\n");
  scanf("%d", &opcao);

  if(opcao == 0) {
    printf("Digite o grau: ");
    scanf("%d", &grau);
    lePolinomio(p, grau);
    printf("Digite os 6 valores: ");
    for(i = 0; i < 6; i++) {
        scanf("%f", &valor);
        printf("p(%f) = %f\n", valor, avaliaPolinomio(p, valor, grau));
    }
  }
  if(opcao == 1) {
    printf("Digite o grau: ");
    scanf("%d", &grau);
    lePolinomio(p2, grau);
    copiaPolinomio (p1, p2, grau, grau);
    printf("p+1 = %s\n", polinomioToStringF(p2, grau+1));
    printf("p = %s", polinomioToStringF(p1, grau+1));
  }
  if(opcao == 2) {
      printf("Digite os 5 valores: ");
      for(i = 0; i < 5; i++) {
          scanf("%f", &valor);
          racionalReduzido(valor, &x, &y);
          
      }
  }
  if(opcao == 3) {
      printf("Digite o grau:\n");
      scanf("%d", &grau);
      lePolinomio(p, grau);
      printf("Digite os 8 valores: ");
      for(i = 0; i < 8; i++) {
          scanf("%f", &valor);
          printf("p(%f) = %d\n", valor, polinomioComRaiz(p, valor, grau));
      }
  }
  if(opcao == 4) {
    printf("Digite o grau: ");
    scanf("%d", &grau);
    lePolinomio(p, grau);
    printf("Digite uma raiz r para fatorar por (x-r): ");
    scanf("%f", &raiz);
    printf("%f: ", raiz);
    polinomioQuociente (p, q, raiz, grau, grau-1);
    printf("%s",  polinomioToStringF(q, grau));

  }
  if(opcao == 5) {
      printf("Digite o grau: ");
      scanf("%d", &grau);
      lePolinomio(p, grau);
      
      if(listaCanonicaDeRaizes (p, raizes, grau)==0){
          printf("Sem raizes");
      }
  }
  if(opcao == 6) {
    printf("Digite o grau: ");
    scanf("%d", &grau);
    lePolinomio(p, grau);
    printf("Digite a e b: ");
    scanf("%d %d", &x, &y);
    printf("r = %f : ", polinomioQuocienteRacional (p, q, y, x, grau));
    listaCoeficientes(q, grau);
  }




  return 0;
} 