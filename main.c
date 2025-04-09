#include "processo.h"
#include <stdio.h>

int main() {
    Processo processos[MAX_PROCESSOS];
    int qtd;
    carregar_processos(processos, &qtd);

    // 1. Ordenar por ID (crescente)
    printf("Ordenando por ID...\n");
    ordenar_por_id(processos, qtd);
    salvar_csv(processos, qtd, "ordenado_por_id.csv");

    // 2. Ordenar por Data de Ajuizamento (decrescente)
    printf("Ordenando por Data de Ajuizamento...\n");
    ordenar_por_data(processos, qtd);
    salvar_csv(processos, qtd, "ordenado_por_data.csv");

    // 3. Contar processos por Classe
    printf("Contando processos por Classe...\n");
    int classe = 1; // Exemplo de classe
    printf("Classe %d tem %d processos\n", classe, contar_por_classe(processos, qtd, classe));

    // 4. Contar Assuntos Distintos
    printf("Total de assuntos distintos: %d\n", contar_assuntos(processos, qtd));

    // 5. Listar Processos com Múltiplos Assuntos
    printf("Listando Processos com Múltiplos Assuntos...\n");
    listar_multiplos_assuntos(processos, qtd);

    // 6. Calcular Tempo de Tramitação
    printf("Calculando Tempo de Tramitação...\n");
    calcular_tramitacao(processos, qtd);

    return 0;
}
