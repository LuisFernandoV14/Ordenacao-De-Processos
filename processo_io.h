#ifndef PROCESSO_IO_H
#define PROCESSO_IO_H

#include "processo.h"
#include <stdio.h>

void carregar_processos(Processo processos[], int *qtd);
void salvar_csv(Processo processos[], int qtd, const char *filename);

#endif
