#include <stdio.h>
#include <stdlib.h>
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
    int horas;
    int minutos;
    float segundos;        
    
    // identificação específica
    int id_classe[5];
    int id_assunto[5];
    
    // data geral
    int ano_eleicao;

    // (existem processos que tem mais de 1 valor em id classe, id assunto ou nos dois)
    int tipoDeLinha;
    
} PROCESSO;

void moverCursor(FILE *arquivo, size_t BytesVoltados);
void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso);
void limparArrays(PROCESSO *processo);
void atribuirProcessosAStruct(FILE *arquivoBase, PROCESSO *vetorDeProcessos, int numeroDeLinhas);
void copiarProcesso(PROCESSO *p1, PROCESSO *p2);
void contarIDs_classeUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas);


int main(){
    FILE *novoArquivo, *velhoArquivo, *arquivoAuxiliar;
    char parametroDeLinha, charDif, header[100];
    int nmrDeLinhas = 0; 

    velhoArquivo = fopen("processo_043_202409032338.csv", "r");
    novoArquivo = fopen("arquivo de teste.csv", "w");
    arquivoAuxiliar = fopen("arquivo-Auxiliar.csv", "w");

    if (velhoArquivo == NULL) {printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (novoArquivo == NULL) {printf("Erro ao abrir o arquivo \"teste.csv\".\n"); exit(1); }
    if (arquivoAuxiliar == NULL) {printf("Erro ao abrir o arquivo \"arquivo-Auxiliar.csv\".\n"); exit(1); }
    
    // percorre até o final do arquivo contando as linhas (\n)
    while((parametroDeLinha = fgetc(velhoArquivo)) != EOF) { if(parametroDeLinha == '\n') nmrDeLinhas++; } 

    // volta para o início do arquivo
    rewind(velhoArquivo); 
    // armazena o cabeçalho
    fscanf(velhoArquivo, "%[^\n]\n", header); 

    // Declara dinamicamente dois vetores, um para todos os processos e um para todos "id_classe" únicos
    PROCESSO *listaDeProcessos = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas); 
    int *vetorDeIDs_Unicos = (int *) calloc (nmrDeLinhas, sizeof(int));


    fprintf(novoArquivo, "%s\n", header);


    // Bota todos os processos em uma struct própria
    atribuirProcessosAStruct(velhoArquivo, listaDeProcessos, nmrDeLinhas);
    // Escreve todos os processos em um arquivo novo 
    for (int i = 0; i < nmrDeLinhas; i++) {imprimirProcesso(novoArquivo, listaDeProcessos[i]);}

    // Preenche o vetor de "id_classe" unicos e imprime em um arquivo auxiliar
    contarIDs_classeUnicos(listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas, arquivoAuxiliar);
    printf("%d", vetorDeIDs_Unicos[0]);
    for (int i = 1; vetorDeIDs_Unicos[i] > 0; i ++) { printf(", %d", vetorDeIDs_Unicos[i]);}

    imprimirProcessoPorIdClasse(arquivoAuxiliar, listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas);


    // Evita vazamento de memória
    free(listaDeProcessos); free(vetorDeIDs_Unicos);
    fclose(arquivoAuxiliar); fclose(velhoArquivo); fclose(novoArquivo);
    
    return 0;
}

void moverCursor(FILE *arquivo, size_t BytesVoltados) { 
    fseek(arquivo, BytesVoltados, SEEK_CUR);
}

void atribuirProcessosAStruct(FILE *arquivoBase, PROCESSO *vetorDeProcessos, int numeroDeLinhas) {
    
int indice = 0;
char charDif;

while (indice < numeroDeLinhas) {

        // Inicializar arrays com zeros
        limparArrays(&vetorDeProcessos[indice]);

        // Ler parte comum do registro
        fscanf(arquivoBase, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,",
            &vetorDeProcessos[indice].id,
            vetorDeProcessos[indice].numero,
            &vetorDeProcessos[indice].ano,
            &vetorDeProcessos[indice].mes,
            &vetorDeProcessos[indice].dia,
            &vetorDeProcessos[indice].horas,
            &vetorDeProcessos[indice].minutos,
            &vetorDeProcessos[indice].segundos);

        // Verificar se o próximo caractere é aspas ou chave
        charDif = fgetc(arquivoBase);
        moverCursor(arquivoBase, -1);
        
        if (charDif == '{') { // id_classe começa com {
            char buffer[100];
            // Verificar se é um único ID de classe ou múltiplos
            fscanf(arquivoBase, "{%[^}]}", buffer);
            
            // Processar o buffer para extrair os valores
            char *token = strtok(buffer, ",");
            int i = 0;
            while (token != NULL && i < 5) {
                vetorDeProcessos[indice].id_classe[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            
            // Ler o próximo caractere após }
            charDif = fgetc(arquivoBase);
            moverCursor(arquivoBase, -1);
            
            // Verificar se o próximo caractere é vírgula
            if (charDif == ',') {
                fgetc(arquivoBase); // consumir a vírgula
                
                // Verificar o tipo de id_assunto
                charDif = fgetc(arquivoBase);
                moverCursor(arquivoBase, -1);
                
                if (charDif == '{') { // Formato: {id_assunto}
                    fscanf(arquivoBase, "{%[^}]},%d\n", buffer, &vetorDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        vetorDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    vetorDeProcessos[indice].tipoDeLinha = 1;


                } else if (charDif == '"') { // Formato: "{id_assunto}"
                    fscanf(arquivoBase, "\"{%[^}]}\",%d\n", buffer, &vetorDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        vetorDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    vetorDeProcessos[indice].tipoDeLinha = 2;

                }
            }
        } else if (charDif == '"') { // id_classe começa com "
            char buffer[100];
            fscanf(arquivoBase, "\"{%[^}]}\"", buffer);
            
            // Processar o buffer para extrair os valores de id_classe
            char *token = strtok(buffer, ",");
            int i = 0;
            while (token != NULL && i < 5) {
                vetorDeProcessos[indice].id_classe[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            
            // Ler o próximo caractere após "
            charDif = fgetc(arquivoBase);
            
            // Verificar se o próximo caractere é vírgula
            if (charDif == ',') {
                // Verificar o tipo de id_assunto
                charDif = fgetc(arquivoBase);
                moverCursor(arquivoBase, -1);
                
                if (charDif == '{') { // Formato: ,{id_assunto}
                    fscanf(arquivoBase, "{%[^}]},%d\n", buffer, &vetorDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        vetorDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    vetorDeProcessos[indice].tipoDeLinha = 3;

                } else if (charDif == '"') { // Formato: ,"{id_assunto}"
                    fscanf(arquivoBase, "\"{%[^}]}\",%d\n", buffer, &vetorDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        vetorDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    vetorDeProcessos[indice].tipoDeLinha = 4;

                }
            }
        }
        
        indice++;

    }

}

void limparArrays(PROCESSO *processo) {
    for (int i = 0; i < 5; i++) {
        processo->id_classe[i] = 0;
        processo->id_assunto[i] = 0;
    }
}

void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso) {

    int i = 0;
    
    // Parte comum da formatação para todos os tipos de linha
    fprintf(arquivoAlvo, "%d,\"%s\",%04d-%02d-%02d %02d:%02d:%06.3f,",
        processoImpresso.id,
        processoImpresso.numero,
        processoImpresso.ano,
        processoImpresso.mes,
        processoImpresso.dia,
        processoImpresso.horas,
        processoImpresso.minutos,
        processoImpresso.segundos);
    
    // Formatação específica para cada tipo de linha
    switch (processoImpresso.tipoDeLinha) {
        case 1: // Formato simples: {id_classe},{id_assunto},ano
            fprintf(arquivoAlvo, "{%d},{%d},%d\n", 
                processoImpresso.id_classe[0],
                processoImpresso.id_assunto[0], 
                processoImpresso.ano_eleicao);

                limparArrays(&processoImpresso);
            break;
            
        case 2: // Formato: {id_classe},"{id_assunto1,id_assunto2,...}",ano
            fprintf(arquivoAlvo, "{%d},\"{", processoImpresso.id_classe[0]);
            
            // Imprimir múltiplos id_assunto
            i = 0;
            while (processoImpresso.id_assunto[i+1] > 0) {
                fprintf(arquivoAlvo, "%d,", processoImpresso.id_assunto[i]);
                i++;
            }
            fprintf(arquivoAlvo, "%d}\",%d\n", 
                processoImpresso.id_assunto[i],
                processoImpresso.ano_eleicao);

                limparArrays(&processoImpresso);
            break;
            
        case 3: // Formato: "{id_classe1,id_classe2,...}",{id_assunto},ano
            fprintf(arquivoAlvo, "\"{");
            
            // Imprimir múltiplos id_classe
            i = 0;
            while (processoImpresso.id_classe[i+1] > 0) {
                fprintf(arquivoAlvo, "%d,", processoImpresso.id_classe[i]);
                i++;
            }
            fprintf(arquivoAlvo, "%d}\",{%d},%d\n", 
                processoImpresso.id_classe[i],
                processoImpresso.id_assunto[0], 
                processoImpresso.ano_eleicao);

                limparArrays(&processoImpresso);
            break;
            
        case 4: // Formato: "{id_classe1,id_classe2,...}","{id_assunto1,id_assunto2,...}",ano
            fprintf(arquivoAlvo, "\"{");
            
            // Imprimir múltiplos id_classe
            i = 0;
            while (processoImpresso.id_classe[i+1] > 0) {
                fprintf(arquivoAlvo, "%d,", processoImpresso.id_classe[i]);
                i++;
            }
            fprintf(arquivoAlvo, "%d}\",\"{", processoImpresso.id_classe[i]);
            
            // Imprimir múltiplos id_assunto
            i = 0;
            while (processoImpresso.id_assunto[i+1] > 0) {
                fprintf(arquivoAlvo, "%d,", processoImpresso.id_assunto[i]);
                i++;
            }
            fprintf(arquivoAlvo, "%d}\",%d\n", 
                processoImpresso.id_assunto[i],
                processoImpresso.ano_eleicao);

                limparArrays(&processoImpresso);
            break;
    }
}

void copiarProcesso(PROCESSO *p1, PROCESSO *p2) {

    int i = 0, j = 0;
    
    (*p2).id = (*p1).id;
    strcpy((*p2).numero, (*p1).numero);
    (*p2).mes = (*p1).mes;
    (*p2).ano = (*p1).ano;
    (*p2).dia = (*p1).dia;
    (*p2).horas = (*p1).horas;
    (*p2).minutos = (*p1).minutos;
    (*p2).segundos = (*p1).segundos;
    (*p2).ano_eleicao = (*p1).ano_eleicao;
    (*p2).tipoDeLinha = (*p1).tipoDeLinha;

    
    while ((*p1).id_classe[i] > 0 ) {

        (*p2).id_classe[i] = (*p1).id_classe[i];
        i++;
    }

    while ((*p1).id_assunto[j] > 0 ) {

        (*p2).id_assunto[j] = (*p1).id_assunto[j];
        j++;
    }

}

void contarIDs_classeUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar) {

    static int limiteInteracoes = 0;
    int indice = 0, k, l, seraSeRepete, enderecoVetor;


        while (indice < nmrDeLinhas){

            k = 0, l = 0, seraSeRepete = 1, enderecoVetor = 0;

            while( vetorDeProcessos[indice].id_classe[k] > 0 ) {

                while( l < limiteInteracoes) {

                    if(  vetorDeProcessos[indice].id_classe[k] == vetorDeIDs_Unicos[enderecoVetor]) {

                        seraSeRepete = 0;

                    }

                    enderecoVetor++;
                    l++;
                }

                if (seraSeRepete > 0) {


                    //fprintf(auxiliar, "Linha: %d, ID: %d\n", indice + 1,vetorDeProcessos[indice].id_classe[k]);
                    vetorDeIDs_Unicos[limiteInteracoes] = vetorDeProcessos[indice].id_classe[k];
                    limiteInteracoes = limiteInteracoes + 1;


                }

                k++;

            }

            indice++;

        }



}

void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas) {
    int j = 0, contadorDeProcessos = 0;;
    
    while (vetorDeIDs_Unicos[j] > 0) {
        int id_atual = vetorDeIDs_Unicos[j];
        contadorDeProcessos = 0;
        
        if(j == 13) {j++; continue;}
        
        fprintf(arquivoAlvo, "--------------------------------------------------------------------------------\n");
        fprintf(arquivoAlvo, "\n\tProcessos com ID_Classe {%d}:\t\n\n", id_atual);
        
        for (int i = 0; i < nmrDeLinhas; i++) {
            int encontrado = 0;
            
            for (int k = 0; listaDeProcessos[i].id_classe[k] > 0; k++) {
                if (listaDeProcessos[i].id_classe[k] == id_atual) {
                    imprimirProcesso(arquivoAlvo, listaDeProcessos[i]);
                    contadorDeProcessos++;
                    encontrado = 1;
                    break;
                }
            }
        }
        j++;

        fprintf(arquivoAlvo, "\n\tQuantidade de processos com ID_Classe {%d}: %d\t\n\n", id_atual, contadorDeProcessos);
        fprintf(arquivoAlvo, "--------------------------------------------------------------------------------\n");
    }
}
