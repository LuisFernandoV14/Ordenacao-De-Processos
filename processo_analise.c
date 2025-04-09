#include "processo_analise.h"
#include <stdio.h>
#include <time.h>

int contar_por_classe(Processo processos[], int qtd, int classe) {
    int count = 0;
    for (int i = 0; i < qtd; i++) {
        if (processos[i].id_classe == classe) {
            count++;
        }
    }
    return count;
}

int contar_assuntos(Processo processos[], int qtd) {
    int unique[100] = {0}, count = 0;
    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            if (!unique[processos[i].id_assuntos[j]]) {
                unique[processos[i].id_assuntos[j]] = 1;
                count++;
            }
        }
    }
    return count;
}

void listar_multiplos_assuntos(Processo processos[], int qtd) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < qtd; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("%s\n", processos[i].numero);
        }
    }
}

void calcular_tramitacao(Processo processos[], int qtd) {
    time_t t_atual;
    time(&t_atual);
    for (int i = 0; i < qtd; i++) {
        struct tm tm_inicio = {0};
        sscanf(processos[i].data_ajuizamento, "%d-%d-%d", &tm_inicio.tm_year, &tm_inicio.tm_mon, &tm_inicio.tm_mday);
        tm_inicio.tm_year -= 1900;
        tm_inicio.tm_mon -= 1;
        time_t t_inicio = mktime(&tm_inicio);
        int dias = (int)difftime(t_atual, t_inicio) / (60 * 60 * 24);
        printf("%s: %d dias\n", processos[i].numero, dias);
    }
}
