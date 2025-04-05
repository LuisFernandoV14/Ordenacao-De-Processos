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

int main(){
    FILE *novoArquivo, *velhoArquivo;
    char parametroDeLinha, charDif, header[100];
    int indice = 0, nmrDeLinhas = 0;

    velhoArquivo = fopen("processo_043_202409032338.csv", "r");
    novoArquivo = fopen("arquivo de teste.csv", "w");

    if (velhoArquivo == NULL) {printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (novoArquivo == NULL) {printf("Erro ao abrir o arquivo \"teste.csv\"\n."); exit(1); }
    
    while((parametroDeLinha = fgetc(velhoArquivo)) != EOF) { if(parametroDeLinha == '\n') nmrDeLinhas++; } // percorre até o final do arquivo contando as linhas (\n)
    rewind(velhoArquivo); // volta para o início do arquivo
    fscanf(velhoArquivo, "%[^\n]\n", header); // armazena o cabeçalho

    PROCESSO *listaDeProcessos = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas); 

    fprintf(novoArquivo, "%s\n", header);

    while (indice < nmrDeLinhas) {
        
        // Inicializar arrays com zeros
        limparArrays(&listaDeProcessos[indice]);

        // Ler parte comum do registro
        fscanf(velhoArquivo, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,",
            &listaDeProcessos[indice].id,
            listaDeProcessos[indice].numero,
            &listaDeProcessos[indice].ano,
            &listaDeProcessos[indice].mes,
            &listaDeProcessos[indice].dia,
            &listaDeProcessos[indice].horas,
            &listaDeProcessos[indice].minutos,
            &listaDeProcessos[indice].segundos);

        // Verificar se o próximo caractere é aspas ou chave
        charDif = fgetc(velhoArquivo);
        moverCursor(velhoArquivo, -1);
        
        if (charDif == '{') { // id_classe começa com {
            char buffer[100];
            // Verificar se é um único ID de classe ou múltiplos
            fscanf(velhoArquivo, "{%[^}]}", buffer);
            
            // Processar o buffer para extrair os valores
            char *token = strtok(buffer, ",");
            int i = 0;
            while (token != NULL && i < 5) {
                listaDeProcessos[indice].id_classe[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            
            // Ler o próximo caractere após }
            charDif = fgetc(velhoArquivo);
            moverCursor(velhoArquivo, -1);
            
            // Verificar se o próximo caractere é vírgula
            if (charDif == ',') {
                fgetc(velhoArquivo); // consumir a vírgula
                
                // Verificar o tipo de id_assunto
                charDif = fgetc(velhoArquivo);
                moverCursor(velhoArquivo, -1);
                
                if (charDif == '{') { // Formato: {id_assunto}
                    fscanf(velhoArquivo, "{%[^}]},%d\n", buffer, &listaDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        listaDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    listaDeProcessos[indice].tipoDeLinha = 1;


                } else if (charDif == '"') { // Formato: "{id_assunto}"
                    fscanf(velhoArquivo, "\"{%[^}]}\",%d\n", buffer, &listaDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        listaDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    listaDeProcessos[indice].tipoDeLinha = 2;

                }
            }
        } else if (charDif == '"') { // id_classe começa com "
            char buffer[100];
            fscanf(velhoArquivo, "\"{%[^}]}\"", buffer);
            
            // Processar o buffer para extrair os valores de id_classe
            char *token = strtok(buffer, ",");
            int i = 0;
            while (token != NULL && i < 5) {
                listaDeProcessos[indice].id_classe[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            
            // Ler o próximo caractere após "
            charDif = fgetc(velhoArquivo);
            
            // Verificar se o próximo caractere é vírgula
            if (charDif == ',') {
                // Verificar o tipo de id_assunto
                charDif = fgetc(velhoArquivo);
                moverCursor(velhoArquivo, -1);
                
                if (charDif == '{') { // Formato: ,{id_assunto}
                    fscanf(velhoArquivo, "{%[^}]},%d\n", buffer, &listaDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        listaDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    listaDeProcessos[indice].tipoDeLinha = 3;

                } else if (charDif == '"') { // Formato: ,"{id_assunto}"
                    fscanf(velhoArquivo, "\"{%[^}]}\",%d\n", buffer, &listaDeProcessos[indice].ano_eleicao);
                    
                    // Processar o buffer para extrair os valores
                    token = strtok(buffer, ",");
                    i = 0;
                    while (token != NULL && i < 5) {
                        listaDeProcessos[indice].id_assunto[i] = atoi(token);
                        token = strtok(NULL, ",");
                        i++;
                    }

                    listaDeProcessos[indice].tipoDeLinha = 4;

                }
            }
        }
        
        imprimirProcesso(novoArquivo, listaDeProcessos[indice]);
        
        indice++;
    }

    free(listaDeProcessos);
    fclose(velhoArquivo);
    fclose(novoArquivo);
    
    return 0;
}

void moverCursor(FILE *arquivo, size_t BytesVoltados) { 
    fseek(arquivo, BytesVoltados, SEEK_CUR);
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
