#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define pausarLeitura system("pause")
#ifdef _WIN32
#define limparTela system("cls")
#else
#define limparTela system("clear")
#endif

typedef struct Processo
{

    // identificação
    int id;
    char numero[50];

    // data que o processo foi aberto
    int mes;
    int ano;
    int dia;
    // horário que o processo foi aberto
    int horas;
    int minutos;
    float segundos;

    // identificaçãoo específica
    int id_classe[5];
    int id_assunto[5];

    // data geral
    int ano_eleicao;

    // (existem processos que tem mais de 1 valor em id classe, id assunto ou nos dois)
    int tipoDeLinha;

} PROCESSO;

int escolherOrdem(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, FILE *novoArquivo, int nmrDeLinhas, int escolha);
int contardias(PROCESSO processo);
int limparNumerosRepetidos(int *vetorDeIDs_Unicos);
void imprimirOrdemOriginal(FILE *novoArquivo, int nmrDeLinhas, PROCESSO *listaDeProcessos);
void copiarProcesso(PROCESSO *destino, PROCESSO *origem);
void contarIDs_ClasseUnicos(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *novoArquivo);
void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas);
void contarIDs_AssuntoUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void ordenarVetorPorIdCrescente(PROCESSO *listaDeProcessos, int nmrDeProcesso, FILE *novoArquivo);
void moverCursor(FILE *arquivo, size_t BytesVoltados);
void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso);
void limparArrays(PROCESSO *processo);
void atribuirProcessosAStruct(FILE *velhoArquivo, PROCESSO *listaDeProcessos, int nmrDeLinhas);
void listarProcessosComMaisDeUmAssunto(PROCESSO *lista, int totalProcessos, FILE *arquivoNovo);
void imprimirdias(PROCESSO *lista, int tamanho, FILE *arquivoAlvo);
void organizarPorData(PROCESSO *lista, int nmrDeLinhas, FILE *arquivoAlvo);
time_t converterparaTempot(PROCESSO processo);

#endif

