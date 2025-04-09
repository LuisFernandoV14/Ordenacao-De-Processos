#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void carregar_processos(Processo processos[], int *qtd) {
    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    char linha[256];
    fgets(linha, 256, arquivo);
    *qtd = 0;
    while (fgets(linha, 256, arquivo) && *qtd < MAX_PROCESSOS) {
        Processo p;
        sscanf(linha, "%d,%49[^,],%19[^,],%d,{%d}", &p.id, p.numero, p.data_ajuizamento, &p.id_classe, &p.id_assuntos[0]);
        p.num_assuntos = 1;
        processos[(*qtd)++] = p;
    }
    fclose(arquivo);
}

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

void salvar_csv(Processo processos[], int qtd, const char *filename) {
    FILE *arquivo = fopen(filename, "w");
    fprintf(arquivo, "id,numero,data_ajuizamento,id_classe,id_assuntos\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d,%s,%s,%d,{%d}\n", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe, processos[i].id_assuntos[0]);
    }
    fclose(arquivo);
}
