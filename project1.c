/* iaed-23 - ist1107028 - project1 */
/*
 * Ficheiro:  project1.c
 * Autor:  Inês Duarte Rodrigues Almeida Paredes
 * Descrição: Programa que corresponde a um sistema de gestão de carreiras de
              transportes públicos.
*/


/*  Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Constantes  */
#define MAX_ARGS 5              /* número máximo de argumentos */
#define NOMEMAXCARREIRA 21      /* dimensão do nome da carreira */
#define NOMEMAXPARAGEM 51       /* dimensão do nome da paragem */
#define NOMEMAX 55              /* dimensão máxima dos nomes */

#define NUMMAXCARREIRA 200      /* número máximo de carreiras */
#define NUMMAXPARAGEM 10000     /* número máximo de paragens */

#define NAO_EXISTE -2           /* código de erro */

#define TRUE 1                  /* verdadeiro */
#define FALSE 0			        /* falso */      

/* Tipos de Dados */

typedef struct{
    char nome[NOMEMAXPARAGEM];
    double latitude, longitude;
    int n_carreiras;
} Paragem;

typedef struct{
    char nome[NOMEMAXCARREIRA];
    Paragem lst_paragens[NUMMAXPARAGEM];
    int n_paragens;
    double custo, duracao;
} Carreira;

/*  Variaveis globais   */
int _n_carreiras_max = 0;                 /* número de carreiras introduzidas */
int _n_paragens_max = 0;                  /* número de paragens introduzidas */

Carreira _lst_carreiras[NUMMAXCARREIRA];                /* vetor de carreiras */
Paragem _lst_paragens_total[NUMMAXPARAGEM];             /* vetor de paragens */


/*                                Funções                                     */


                           /* Funcoes Leitura */
/*============================================================================*/

/*  Recebe um input, divide em argumentos, consequentemente os armazena num 
    vetor de argumentos e retorna o número de argumentos                      */
int separa_inputs(char inputs[], char args[][NOMEMAX], int max_args) {
    int i = 0, j = 0, dentro_aspas = FALSE, l;

    for (l = 0; inputs[l] != '\0' && i < max_args; l++) {
        char c = inputs[l];
        if (c == '"') {
            dentro_aspas = !dentro_aspas; 
            continue;
        }
        if (c == ' ' && !dentro_aspas) {
            if (j > 0) { 
                args[i][j] = '\0';
                i++;
                j = 0;           /*Passa para o próximo argumento*/
            }
        } else {
            args[i][j] = c;
            j++;
        }
    }
    if (j > 0) {
        args[i][j] = '\0'; /*Adiciona o último argumento na matriz args*/
        i++;
    }
    return i;
}


/*  Recebe uma string, armazena-a em um array de caracteres e retorna o tamanho
    da string.                                                               */

int leinput(char str[]) {
    int i = 0, c;

    while (((c = getchar()) != EOF && c != '\n'))
        str[i++] = c;
    str[i] = '\0';

    return i;
}
/*============================================================================*/


                            /*Funções de validação*/
/*============================================================================*/

/*  Recebe dois argumentos: custo e duração, e tem como objetivo validá-los. 
    Retorna TRUE se o argumento é válido e FALSE caso contrário.              */

int valida_custo_duracao(double custo, double duracao) {
    if (custo >= 0 && duracao >= 0)
        return TRUE;

    return FALSE;
}


/*  Avalia se um argumento passado como parâmetro é igual à palavra "inverso". 
    Retorna TRUE se o argumento é igual a "inverso" e FALSE caso contrário.   */

int avalia_arg(char args[]){
    int i, t_arg, t_inv;
    char c[] = "inverso";

    t_arg = strlen(args);
    t_inv = strlen(c);

    if (t_arg < 3 || t_arg > t_inv)
        return FALSE;

    else {
        for (i = 0; i < t_arg; i++)
            if (args[i] != c[i])
                return FALSE;
    }
    return TRUE;
}
/*============================================================================*/


                          /*Função de converção*/
/*============================================================================*/

/*  Converte uma string que contem um número em ponto flutuante para um valor 
    do tipo double.                                                           */

double conv_double(char str[]) {
    double num;

    num = strtod(str, NULL);
    return num;
}
/*============================================================================*/


                           /*Funções de procura*/
/*============================================================================*/

/*  Recebe o nome de uma carreira e retorna um inteiro que representa o índice 
    da carreira na lista de carreiras ou retorna NAO_EXISTE                   */

int encontraCarreira(char args[]) {
    int i;

    for (i = 0; i < _n_carreiras_max; i++)
        if (strcmp(_lst_carreiras[i].nome, args) == 0) {
            return i;
        }
    return NAO_EXISTE;
}


/*  Recebe o nome de uma paragem e retorna um inteiro que representa o índice da 
    paragem na lista de paragens ou retorna NAO_EXISTE.                       */

int encontraParagem(char args[]) {
    int i;

    for (i = 0; i <= _n_paragens_max; i++)
        if (strcmp(_lst_paragens_total[i].nome, args) == 0) {
            return i;
        }
    return NAO_EXISTE;
}


/*  Encontra uma paragem em uma determinada carreira e retorna o índice da 
    paragem. Ela recebe como argumentos o nome da paragem a ser encontrada e o 
    índice da carreira onde a paragem será procurada.                         */

int encontraParagem_Carreira(char args[], int j) {
    int i;

    for (i = 0; i < _lst_carreiras[j].n_paragens; i++)
        if (strcmp(_lst_carreiras[j].lst_paragens[i].nome, args) == 0) {
            return i;
        }
    return NAO_EXISTE;
}
/*============================================================================*/


                            /*Funções auxiliares*/
/*============================================================================*/

/*  Adiciona o custo e a duração a uma determinada carreira. A função recebe 
    três argumentos: o índice da carreira em uma lista de carreiras, e dois 
    arrays de caracteres que contêm os valores do custo e da duração.         */

void add_custo_duracao(int i, char arg1[], char arg2[]) {
    _lst_carreiras[i].custo += conv_double(arg1);
    _lst_carreiras[i].duracao += conv_double(arg2);
}


/*============================================================================*/
/*  Estas funções são responsáveis por adicionar uma nova ligação entre duas 
    paragens em uma determinada carreira.                                     */
/*============================================================================*/

/*  Recebe o índice da carreira e um array de argumentos que corresponde ao nome
    das paragens.                                                             */

void add_primeira_ligacao(int i, char args[][NOMEMAX]) {
    strcpy(_lst_carreiras[i].lst_paragens[0].nome, args[1]);
    strcpy(_lst_carreiras[i].lst_paragens[1].nome, args[2]);

    _lst_carreiras[i].n_paragens+=2;
    _lst_paragens_total[encontraParagem(args[1])].n_carreiras++;
    _lst_paragens_total[encontraParagem(args[2])].n_carreiras++;

    add_custo_duracao(i, args[3], args[4]);
}


/*  Recebe o índice da carreira e um array de argumentos que corresponde ao nome
    das paragens.                                                             */

void add_ligacao_inicio(int i, char args[][NOMEMAX]) {
    int l;

    for (l = _lst_carreiras[i].n_paragens - 1; l >= 0; l--) 
        strcpy(_lst_carreiras[i].lst_paragens[l + 1].nome, 
                _lst_carreiras[i].lst_paragens[l].nome);

    strcpy(_lst_carreiras[i].lst_paragens[0].nome, args[1]);
    _lst_carreiras[i].n_paragens++;
    _lst_paragens_total[encontraParagem(args[1])].n_carreiras++;

    add_custo_duracao(i, args[3], args[4]);
}


/*  Recebe o índice da carreira e um array de argumentos que corresponde ao nome 
    das paragens e o índice da primeira paragem.                              */

void add_ligacao_fim(int i, char args[][NOMEMAX], int idx1){
    strcpy(_lst_carreiras[i].lst_paragens[idx1+1].nome, args[2]);

    _lst_carreiras[i].n_paragens++;
    _lst_paragens_total[encontraParagem(args[2])].n_carreiras++;

    add_custo_duracao(i, args[3], args[4]);
}


/*  Recebe o índice da carreira e um array de argumentos que corresponde ao nome
    das paragens e os índices da primeira e segunda paragem.                  */

void add_circular(int i, char args[][NOMEMAX], int idx1, int idx2){
    if (idx2 == 0){
            strcpy(_lst_carreiras[i].lst_paragens[idx1+1].nome, args[2]);
            _lst_carreiras[i].n_paragens++;
            add_custo_duracao(i, args[3], args[4]);
        }
}
/*============================================================================*/


/*============================================================================*/
/*  Recebe uma matriz de strings como argumento, onde a primeira string é o nome 
    da carreira, a segunda é o nome da paragem de início da nova ligação e a 
    terceira é o nome da paragem de fim.                                      */

void add_ligacoes(char args[][NOMEMAX]) {
    
    int i, idx1, idx2;

    i = encontraCarreira(args[0]);
    idx1 = encontraParagem_Carreira(args[1], i); /*Indice da paragem origem*/
    idx2 = encontraParagem_Carreira(args[2], i); /*Indice da paragem destino*/

    if (idx1 == NAO_EXISTE && _lst_carreiras[i].n_paragens == 0){
        add_primeira_ligacao(i, args);

    } else if (idx1 == NAO_EXISTE && idx2 != NAO_EXISTE){
        add_ligacao_inicio(i, args);

    } else if (idx1 == _lst_carreiras[i].n_paragens-1 && idx2 == NAO_EXISTE){
        add_ligacao_fim(i, args, idx1);

    } else if (idx1 != NAO_EXISTE && idx2 != NAO_EXISTE){
        add_circular(i, args, idx1, idx2);
        
    } else 
        printf("link cannot be associated with bus line.\n");
}
/*============================================================================*/


                            /*Funções construtoras*/
/*============================================================================*/

/*  Recebe um nome, cria uma nova carreira e adiciona à lista de carreiras.   */

void cria_carreira(char args[]) {
    strcpy(_lst_carreiras[_n_carreiras_max].nome, args);
    _lst_carreiras[_n_carreiras_max].n_paragens = 0;
    _n_carreiras_max++;
}


/*  Recebe um array com o nome, a latitude e longitude, e cria uma nova paragem.*/

void cria_paragem(char args[][NOMEMAX]) {
    /*Cria uma nova paragem*/
    Paragem nova_paragem;
    strcpy(nova_paragem.nome, args[0]);
    nova_paragem.latitude = conv_double(args[1]);
    nova_paragem.longitude = conv_double(args[2]);
    nova_paragem.n_carreiras = 0;

    /*Adiciona à lista de paragens*/
    _lst_paragens_total[_n_paragens_max] = nova_paragem;
    _n_paragens_max++;
}
/*============================================================================*/


                         /*Funções de representação*/
/*============================================================================*/

/*  Recebe um argumento do tipo Paragem e faz print do nome.      */

void nome_paragem(Paragem local){
    if (strcmp(local.nome, ""))
        printf("%s ", local.nome);
}

/*  Lista o nome, a paragem de origem, a paragem de destino, o custo total e a 
    duração total de todas as carreiras que existem em lista de carreiras.    */

void listar_carreiras() {
    int i;

    for (i = 0; i < _n_carreiras_max; i++){
        printf("%s ", _lst_carreiras[i].nome);

        nome_paragem(_lst_carreiras[i].lst_paragens[0]);
        nome_paragem(_lst_carreiras[i].lst_paragens[_lst_carreiras[i].n_paragens-1]);

        printf("%d %.2f %.2f\n", _lst_carreiras[i].n_paragens, 
                _lst_carreiras[i].custo, _lst_carreiras[i].duracao);
    }
}

/*  Recebe o nome da carreira e lista todas as paragens da carreira.          */

void listar_uma_carreira(char args[]) {
    int i, j;
    i = encontraCarreira(args);

    for (j = 0; j < _lst_carreiras[i].n_paragens-1; j++)
        printf("%s, ", _lst_carreiras[i].lst_paragens[j].nome);
    printf("%s\n", _lst_carreiras[i].lst_paragens[j].nome);
}


/*  Lista o nome, a latitude, a longitude e o número de carreiras de todas as 
    paragens.                                                                 */

void listar_paragens() {
    int i;

    for (i = 0; i < _n_paragens_max; i++)
        printf("%s: %16.12f %16.12f %d\n", _lst_paragens_total[i].nome,
        _lst_paragens_total[i].latitude, _lst_paragens_total[i].longitude, 
        _lst_paragens_total[i].n_carreiras);
}


/*  Recebe o nome da paragem e lista a latitude e longitude correspondentes.  */

void listar_uma_paragem(char args[]) {
    int j;
    j = encontraParagem(args);

    printf("%16.12f %16.12f\n", _lst_paragens_total[j].latitude, 
                _lst_paragens_total[j].longitude);
}
/*============================================================================*/


/*============================================================================*/
/*  Recebe um nome de uma paragem e uma matriz, e conta o número de vezes que 
    esta aparece em cada carreira e armazenar os nomes das carreiras que possuem 
    essa paragem na matriz.                                                   */

int n_frequencias(char args[], char lst[][NOMEMAXCARREIRA]){
    int idx[NUMMAXCARREIRA] = {0};
    int k, l, m, n = 0;

    for (k = 0; k < _n_carreiras_max; k++) {
        for (l = 0; l < _lst_carreiras[k].n_paragens; l++)
            if (strcmp(_lst_carreiras[k].lst_paragens[l].nome, args) == 0)
                idx[k]++;
    }

    for (m = 0; m < _n_carreiras_max; m++) {
        if (idx[m] > 0) {
            strcpy(lst[n], _lst_carreiras[m].nome);
            n++;
        }
    }
    return n;
}
/*============================================================================*/


                            /*Funções de ordenação*/
/*============================================================================*/

/*  Recebe um nome de uma carreira e lista todas as paragens por ordem inversa.*/

void ordena_paragens_inv(char arg[]) {
    int i, j;
    j = encontraCarreira(arg);

    for (i = _lst_carreiras[j].n_paragens; i - 1 > 0; i--)
        printf("%s, ", _lst_carreiras[j].lst_paragens[i-1].nome);

    printf("%s\n", _lst_carreiras[j].lst_paragens[0].nome);
}


/*  Função insertion sort que recebe uma matriz de carreiras, o indice inicial 
    e o indice final, e ordena alfabeticamente.                               */

int menor(char a[], char b[]){
    if (strcmp(a, b) < 0)
        return TRUE;
    return FALSE;
}

void insertion_sort(char carr[][NOMEMAXCARREIRA], int esq, int dir) {
    int i,j;
    char temp[NOMEMAXCARREIRA];

    for (i = esq + 1; i <= dir; i++) {
        strcpy(temp, carr[i]);
        j = i-1;
        while (j >= esq && menor(temp, carr[j])) {
            strcpy(carr[j+1], carr[j]);
            j--;
        }
        strcpy(carr[j+1], temp);
    }
}
/*============================================================================*/


                                /*Comandos*/
/*============================================================================*/

/*  Função que adiciona e lista as carreiras    */

void comando_c() {
    char str[BUFSIZ];
    char args[MAX_ARGS][NOMEMAX];
    int num_args, indice, j;

    leinput(str);
    num_args = separa_inputs(str, args, MAX_ARGS);

    if (num_args == 0)
        listar_carreiras();

    else if (num_args == 1){
        indice = encontraCarreira(args[0]);
        if (indice == NAO_EXISTE)
            cria_carreira(args[0]);

        else if (_lst_carreiras[indice].n_paragens == 0){}
        
        else
            listar_uma_carreira(args[0]);

    } else if (num_args == 2){
        j = encontraCarreira(args[0]);
        
        if (!avalia_arg(args[1]))
            printf("incorrect sort option.\n");

        else if (_lst_carreiras[j].n_paragens != 0)
            ordena_paragens_inv(args[0]);
    }
}


/*  Função que adiciona e lista paragens    */

void comando_p() {
    char str[BUFSIZ];
    char args[MAX_ARGS][NOMEMAX];
    int num_args, tamanho;

    tamanho = leinput(str);
    num_args = separa_inputs(str, args, MAX_ARGS);

    if (num_args == 0 && tamanho == 0)
        listar_paragens();

    else if (num_args == 3){
        if (encontraParagem(args[0]) != NAO_EXISTE)
            printf("%s: stop already exists.\n", args[0]);
        else
            cria_paragem(args);
    
    } else if (num_args == 1){
        if (encontraParagem(args[0]) == NAO_EXISTE)
            printf("%s: no such stop.\n", args[0]);
        else
            listar_uma_paragem(args[0]);
    }
}

/*  Função que adiciona ligações    */

void comando_l(){
    char str[BUFSIZ];
    char args[MAX_ARGS][NOMEMAX];

    leinput(str);
    separa_inputs(str, args, MAX_ARGS);

    if (encontraCarreira(args[0]) == NAO_EXISTE)
        printf("%s: no such line.\n", args[0]);

    else if (encontraParagem(args[1]) == NAO_EXISTE)
        printf("%s: no such stop.\n", args[1]);
    
    else if (encontraParagem(args[2]) == NAO_EXISTE)
        printf("%s: no such stop.\n", args[2]);

    else if (!valida_custo_duracao(conv_double(args[3]), conv_double(args[4])))
        printf("negative cost or duration.\n");

    else 
        add_ligacoes(args);
}


/*  Função que lista as interseções entre carreiras    */

void comando_i() {
    int i, k, m;

    for (i = 0; i < _n_paragens_max; i++) {
        char carreiras[NUMMAXCARREIRA][NOMEMAXCARREIRA];
        k = n_frequencias(_lst_paragens_total[i].nome, carreiras);

        if (k > 1) {
            printf("%s %d: ", _lst_paragens_total[i].nome, 
                    _lst_paragens_total[i].n_carreiras);

            insertion_sort(carreiras, 0, k - 1);

            for (m = 0; m < k - 1; m++)
                printf("%s ", carreiras[m]);
            printf("%s\n", carreiras[m]);
        }
    }
}
/*============================================================================*/


int main() 
{
    int comando;

    while ((comando = getchar()) != EOF)
        switch(comando){
            case 'q': exit(0);
            case 'c': comando_c();
                break;
            case 'p': comando_p();
                break;
            case 'l': comando_l();
                break;
            case 'i': comando_i();
                break;
            default: break;
        }
    return 0;
}
