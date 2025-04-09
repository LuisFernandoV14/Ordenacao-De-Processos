#ifndef PROCESSO_ANALISE_H
#define PROCESSO_ANALISE_H

#include "processo.h"

int contar_por_classe(Processo processos[], int qtd, int classe);
int contar_assuntos(Processo processos[], int qtd);
void listar_multiplos_assuntos(Processo processos[], int qtd);
void calcular_tramitacao(Processo processos[], int qtd);

#endif
