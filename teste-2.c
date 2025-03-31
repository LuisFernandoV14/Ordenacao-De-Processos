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
        float segundos;        
    
    // identificação específica
    char id_classe[25];
    char id_assunto[25];
    
    // data geral
    int ano_eleicao;
    
    // id;numero ; data_ajuizamento ; id_classe ; id_assunto ; ano_eleicao

} processo;

// Declaração das funções ----------------------------------------------------------------------------------
void atribuirStruct(processo *arr, FILE *arquivoBase, FILE *arquivoAlvo, int indice);
void escreverProcessos(FILE *teste, processo *arr, int indice,  int tipo);
// Declaração das funções ----------------------------------------------------------------------------------

int main() {

    // Declaração Inicial das variáveis //
    int nmrDeLinhas = 0;
    FILE *arquivo, *teste;
    char lixo[100], counter;
    int indice = 0;

    arquivo = fopen("processo_043_202409032338.csv", "r");
    teste = fopen("arquivo de teste.csv", "w" );


    if (arquivo == NULL) { printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (teste == NULL) { printf("Erro ao abrir o arquivo \"teste.csv\'\n".); exit(1); }


    // lê e descarta a primeira linha:
    fgets(lixo, sizeof(lixo), arquivo); 

    // percorre até o final do arquivo, contando o numero de linhas:
    while ((counter = fgetc(arquivo)) != EOF) {if (counter == '\n') {nmrDeLinhas++;}} 
    
    // volta para o inicio do arquivo
    rewind(arquivo);
    
    // Declaração do vetor que vai guardar todos os processos
    processo *listaDeProcessos = (processo *) malloc(sizeof(processo) * nmrDeLinhas);
    if (listaDeProcessos == NULL) { printf("Erro na alocação de memória.\n"); exit(1);}
    


    // Escreve o cabeçalho no arquivo de teste
    fprintf(teste, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
    // Atribui todos os 18392 processos a um vetor de Struct Processo e escreve todos eles no arquivo teste 
    atribuirStruct(listaDeProcessos, arquivo, teste, indice);

    // Impede o vazamento de memória
    free(listaDeProcessos);
    fclose(arquivo);
    fclose(teste);

    return 0;

}

void atribuirStruct(processo *arr, FILE *arquivoBase, FILE *arquivoAlvo, int indice) {

    int tipo;
    char linha[100];

    while (
        fscanf(arquivoBase, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,%[^,],%[^,],%d",
        &arr[indice].id,
        arr[indice].numero,
        &arr[indice].ano,
        &arr[indice].mes,
        &arr[indice].dia,
        &arr[indice].hora,
        &arr[indice].minutos,
        &arr[indice].segundos,
        arr[indice].id_classe,
        arr[indice].id_assunto,
        &arr[indice].ano_eleicao) == 12 || fgets(linha, sizeof(linha), arquivoBase) != NULL)  {
        
        tipo = 1;

        if(indice == 0){indice +=1 ;continue;}

        // printf("(teste) 1º Caracter: %c \n", arr[indice].id_classe[0]);

        // Na lista de processos, tem certos "id_classe" que estáo entre aspas, porque são números com vírgula, se isso acontecer esse if escaneia corretamente
        if(arr[indice].id_classe[0] == '\"') {

            char counter;
            int linhaDiferente = 0;

            rewind(arquivoBase);
            while((counter = fgetc(arquivoBase)) != EOF) {
                if(counter == '\n') {

                    linhaDiferente++;

                    if ((linhaDiferente == indice)) {

                        fscanf(arquivoBase, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,\"%[^\"]\",{%s},%d",
                        &arr[indice].id,
                        arr[indice].numero,
                        &arr[indice].ano,
                        &arr[indice].mes,
                        &arr[indice].dia,
                        &arr[indice].hora,
                        &arr[indice].minutos,
                        &arr[indice].segundos,
                        arr[indice].id_classe,
                        arr[indice].id_assunto,
                        &arr[indice].ano_eleicao);
            
                        // printf("(TESTE) 1º caracter: %c Numero: %s Linha: %d\n", arr[indice].id_classe[0], arr[indice].id_classe, indice);
                        tipo = 2;

                        break;
                    }
                }
            }
        
        }
        
        escreverProcessos(arquivoAlvo, arr, indice, tipo);  
        indice += 1;
    }
}

void escreverProcessos(FILE *teste, processo *arr, int indice,  int tipo){
    
    if (tipo == 1){
    
    fprintf(teste, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,%s,%s,%d\n",
        arr[indice].id,
        arr[indice].numero,
        arr[indice].ano,
        arr[indice].mes,
        arr[indice].dia,
        arr[indice].hora,
        arr[indice].minutos,
        arr[indice].segundos,
        arr[indice].id_classe,
        arr[indice].id_assunto,
        arr[indice].ano_eleicao);
    }

    else if (tipo == 2 ){

    fprintf(teste, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,\"%s\",%s,%d\n",
        arr[indice].id,
        arr[indice].numero,
        arr[indice].ano,
        arr[indice].mes,
        arr[indice].dia,
        arr[indice].hora,
        arr[indice].minutos,
        arr[indice].segundos,
        arr[indice].id_classe,
        arr[indice].id_assunto,
        arr[indice].ano_eleicao);
    }
}
