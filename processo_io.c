#include "processo_io.h"
#include <stdlib.h>

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

void salvar_csv(Processo processos[], int qtd, const char *filename) {
    FILE *arquivo = fopen(filename, "w");
    fprintf(arquivo, "id,numero,data_ajuizamento,id_classe,id_assuntos\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d,%s,%s,%d,{%d}\n", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe, processos[i].id_assuntos[0]);
    }
    fclose(arquivo);
}
