#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Processo {
    
    //identificação
    int id;
    char numero[50];

    // data que o processo foi aberto
    int mes;
    int ano;
    int dia;
        // horário que o processo foi aberto
        int hora;
        int minutos;
        int segundos;
        int milisegundos;
        
    
    // identificação específica
    int id_classe;
    int id_assunto;
    
    // data geral
    int ano_eleicao;
    
    // id;numero ; data_ajuizamento ; id_classe ; id_assunto ; ano_eleicao

} processo;

int main() {

    int nmrDeLinhas = 0;
    FILE *arquivo, *teste;
    char linha[200], lixo[100], counter;
    int indice = 0;

    arquivo = fopen("processo_043_202409032338.csv", "r");
    teste = fopen("arquivo de teste.csv", "w" );

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
 
    fgets(lixo, sizeof(lixo), arquivo); // Lê e descarta a primeira linha

    while ((counter = fgetc(arquivo)) != EOF) {if (counter == '\n') {nmrDeLinhas++;}}
    rewind(arquivo);

    processo *listaDeProcessos = (processo *) malloc(sizeof(processo) * nmrDeLinhas);
    
    if (listaDeProcessos == NULL) {
        printf("Erro na alocação de memória.\n");
        exit(1);
    }
    
    fprintf(teste, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");

    while (
        fscanf(arquivo, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%d.%d,{%d},{%d},%d",
        &listaDeProcessos[indice].id,
        listaDeProcessos[indice].numero,
        &listaDeProcessos[indice].ano,
        &listaDeProcessos[indice].mes,
        &listaDeProcessos[indice].dia,
        &listaDeProcessos[indice].hora,
        &listaDeProcessos[indice].minutos,
        &listaDeProcessos[indice].segundos,
        &listaDeProcessos[indice].milisegundos,
        &listaDeProcessos[indice].id_classe,
        &listaDeProcessos[indice].id_assunto,
        &listaDeProcessos[indice].ano_eleicao) == 12 || fgets(linha, sizeof(linha), arquivo) != NULL)  {

        // -------------------------------------------------------------------------------------------------

        if(indice == 0){indice +=1 ;continue;}
        
        fprintf(teste, "%d,\"%s\",%d-%d-%d %d:%d:%d.%d,{%d},{%d},%d\n",
        listaDeProcessos[indice].id,
        listaDeProcessos[indice].numero,
        listaDeProcessos[indice].ano,
        listaDeProcessos[indice].mes,
        listaDeProcessos[indice].dia,
        listaDeProcessos[indice].hora,
        listaDeProcessos[indice].minutos,
        listaDeProcessos[indice].segundos,
        listaDeProcessos[indice].milisegundos,
        listaDeProcessos[indice].id_classe,
        listaDeProcessos[indice].id_assunto,
        listaDeProcessos[indice].ano_eleicao);
        
        indice += 1;
    }

    free(listaDeProcessos);
    fclose(arquivo);
    fclose(teste);

    return 0;

}