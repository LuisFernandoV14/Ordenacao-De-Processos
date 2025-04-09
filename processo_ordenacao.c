#include "processo_ordenacao.h"
#include <string.h>

void ordenar_por_id(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (processos[i].id > processos[j].id) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}
