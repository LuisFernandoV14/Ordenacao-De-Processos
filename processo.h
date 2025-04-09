#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_ASSUNTOS 10

typedef struct {
    int id;
    char numero[50];
    char data_ajuizamento[20];
    int id_classe;
    int id_assuntos[MAX_ASSUNTOS];
    int num_assuntos;
} Processo;

#include "processo_ordenacao.h"
#include "processo_analise.h"
#include "processo_io.h"

#endif
