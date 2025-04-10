#ifndef PROCESSO_h
#define PROCESSO_h

#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Processo
{

    // identificação
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

    // identificaçãoo específica
    int id_classe[5];
    int id_assunto[5];

    // data geral
    int ano_eleicao;

    // (existem processos que tem mais de 1 valor em id classe, id assunto ou nos dois)
    int tipoDeLinha;

} PROCESSO;

int escolherOrdem(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, FILE *arquivoAlvo, int nmrDeLinhas, int escolha);
void copiarProcesso(PROCESSO *destino, PROCESSO *origem);
void contarIDs_ClasseUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas);
void contarIDs_AssuntoUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void limparNumerosRepetidos(int *vetorDeIDs_Unicos);
void ordenarVetorPorIdCrescente(PROCESSO *listaDeProcessos, int nmrDeProcesso);
void moverCursor(FILE *arquivo, size_t BytesVoltados);
void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso);
void limparArrays(PROCESSO *processo);
void atribuirProcessosAStruct(FILE *velhoArquivo, PROCESSO *listaDeProcessos, int nmrDeLinhas);
void listarProcessosComMaisDeUmAssunto(PROCESSO *lista, int totalProcessos, FILE *arquivoNovo);


int escolherOrdem(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, FILE *arquivoAlvo, int nmrDeLinhas, int escolha)
{

    switch (escolha)
    {

    case 1: // Imprimir na ordem original
        fprintf(arquivoAlvo, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
        for (int i = 0; i < nmrDeLinhas; i++)
        {
            imprimirProcesso(arquivoAlvo, listaDeProcessos[i]);
        }
        return 0;
        break;

    case 2: // Organizar por id classe
        contarIDs_ClasseUnicos(listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas, arquivoAlvo);
        imprimirProcessoPorIdClasse(arquivoAlvo, listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas);
        return 0;
        break;

    case 3: // Contar quantos id assuntos tem no arquivo
        contarIDs_AssuntoUnicos(listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas, arquivoAlvo);
        return 0;
        break;

    case 4: // Organizar por id, do menor para o maior
    {
        PROCESSO *listaDeProcessosOrdenada = (PROCESSO *)malloc(sizeof(PROCESSO) * nmrDeLinhas);
        for (int i = 0; i < nmrDeLinhas; i++)
        {
            copiarProcesso(&listaDeProcessosOrdenada[i], &listaDeProcessos[i]);
        }
        // ordenarVetorPorIdCrescente(listaDeProcessosOrdenada);
        for (int i = 0; i < nmrDeLinhas; i++)
        {
            imprimirProcesso(arquivoAlvo, listaDeProcessosOrdenada[i]);
        }
        return 0;
    }
    break;

    default:
        return 1;
        break;
    }
}

void copiarProcesso(PROCESSO *destino, PROCESSO *origem)
{
    int i = 0, j = 0;

    destino->id = origem->id;
    strcpy(destino->numero, origem->numero);
    destino->mes = origem->mes;
    destino->ano = origem->ano;
    destino->dia = origem->dia;
    destino->horas = origem->horas;
    destino->minutos = origem->minutos;
    destino->segundos = origem->segundos;
    destino->ano_eleicao = origem->ano_eleicao;
    destino->tipoDeLinha = origem->tipoDeLinha;

    while (origem->id_classe[i] > 0) {
        destino->id_classe[i] = origem->id_classe[i];
        i++;
    }
    destino->id_classe[i] = 0; // Finalizar com 0

    while (origem->id_assunto[j] > 0) {
        destino->id_assunto[j] = origem->id_assunto[j];
        j++;
    }
    destino->id_assunto[j] = 0; // Finalizar com 0
}

void contarIDs_ClasseUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar)
{

    static int limiteInteracoes = 0;
    int indice = 0, k, l, seraSeRepete, enderecoVetor;

    while (indice < nmrDeLinhas)
    {

        k = 0, l = 0, seraSeRepete = 1, enderecoVetor = 0;

        while (vetorDeProcessos[indice].id_classe[k] > 0)
        {

            while (l < limiteInteracoes)
            {

                if (vetorDeProcessos[indice].id_classe[k] == vetorDeIDs_Unicos[enderecoVetor])
                {

                    seraSeRepete = 0;
                }

                enderecoVetor++;
                l++;
            }

            if (seraSeRepete > 0)
            {

                // fprintf(auxiliar, "Linha: %d, ID: %d\n", indice + 1,vetorDeProcessos[indice].id_classe[k]);
                vetorDeIDs_Unicos[limiteInteracoes] = vetorDeProcessos[indice].id_classe[k];
                limiteInteracoes = limiteInteracoes + 1;
            }

            k++;
        }

        indice++;
    }
}

void contarIDs_AssuntoUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar)
{

    static int limiteInteracoes = 0;
    int indice = 0, k, l, seraSeRepete, enderecoVetor;

    while (indice < nmrDeLinhas)
    {

        k = 0, l = 0, seraSeRepete = 1, enderecoVetor = 0;

        while (vetorDeProcessos[indice].id_assunto[k] > 0)
        {

            while (l < limiteInteracoes)
            {

                if (vetorDeProcessos[indice].id_assunto[k] == vetorDeIDs_Unicos[enderecoVetor])
                {

                    seraSeRepete = 0;
                }

                enderecoVetor++;
                l++;
            }

            if (seraSeRepete > 0)
            {

                // fprintf(auxiliar, "Linha: %d, ID: %d\n", indice + 1,vetorDeProcessos[indice].id_classe[k]);
                vetorDeIDs_Unicos[limiteInteracoes] = vetorDeProcessos[indice].id_assunto[k];
                limiteInteracoes = limiteInteracoes + 1;
            }

            k++;
        }

        indice++;
    }

    int tamanhoDoVetor = 0;

    limparNumerosRepetidos(vetorDeIDs_Unicos);
    while (vetorDeIDs_Unicos[tamanhoDoVetor] > 0)
    {
        tamanhoDoVetor++;
    }

    fprintf(auxiliar, "Existem %d numeros de \"id_assunto\" Unicos, sao eles: \n\n{%d}", tamanhoDoVetor, vetorDeIDs_Unicos[0]);
    for (int i = 1; vetorDeIDs_Unicos[i] > 0; i++)
    {
        if (i % 5 == 0)
        {

            fprintf(auxiliar, "\n");
            fprintf(auxiliar, "{%d}", vetorDeIDs_Unicos[i]);
            continue;
        }

        fprintf(auxiliar, "\t, {%d}", vetorDeIDs_Unicos[i]);
    }
}

void limparNumerosRepetidos(int *vetorDeIDs_Unicos)
{

    int tamanhoDoVetor = 0;

    while (vetorDeIDs_Unicos[tamanhoDoVetor] > 0)
    {
        tamanhoDoVetor++;
    }

    for (int i = 0; i < tamanhoDoVetor; i++)
    {
        int repetido = vetorDeIDs_Unicos[i];

        if (repetido == 0)
            continue;

        for (int j = i + 1; j < tamanhoDoVetor; j++)
        {

            if (repetido == vetorDeIDs_Unicos[j])
            {

                vetorDeIDs_Unicos[j] = vetorDeIDs_Unicos[tamanhoDoVetor - 1];
                vetorDeIDs_Unicos[tamanhoDoVetor - 1] = 0;
                tamanhoDoVetor--;
                j--;
            }
        }
    }
}

void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas)
{
    int j = 0, contadorDeProcessos = 0;
    ;

    limparNumerosRepetidos(vetorDeIDs_Unicos);

    while (vetorDeIDs_Unicos[j] > 0)
    {
        int id_atual = vetorDeIDs_Unicos[j];
        contadorDeProcessos = 0;

        fprintf(arquivoAlvo, "--------------------------------------------------------------------------------\n");
        fprintf(arquivoAlvo, "\n\tProcessos com ID_Classe {%d}:\t\n\n", id_atual);

        for (int i = 0; i < nmrDeLinhas; i++)
        {
            int encontrado = 0;

            for (int k = 0; listaDeProcessos[i].id_classe[k] > 0; k++)
            {
                if (listaDeProcessos[i].id_classe[k] == id_atual)
                {
                    imprimirProcesso(arquivoAlvo, listaDeProcessos[i]);
                    contadorDeProcessos++;
                    encontrado = 1;
                    break;
                }
            }
        }
        j++;

        fprintf(arquivoAlvo, "\n\tQuantidade de processos com ID_Classe {%d}: %d\t\n\n", id_atual, contadorDeProcessos);
        fprintf(arquivoAlvo, "--------------------------------------------------------------------------------\n\n");
    }
}

void ordenarVetorPorIdCrescente(PROCESSO *listaDeProcessos, int nmrDeProcessos) 
{
    PROCESSO aux;

    for (int i = 0; i < nmrDeProcessos - 1; i++) {
        for (int j = i + 1; j < nmrDeProcessos; j++) {
            if (listaDeProcessos[i].id > listaDeProcessos[j].id) {
                copiarProcesso(&aux, &listaDeProcessos[i]);
                copiarProcesso(&listaDeProcessos[i], &listaDeProcessos[j]);
                copiarProcesso(&listaDeProcessos[j], &aux);
            }
        }
    }
}

void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso)
{
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
    switch (processoImpresso.tipoDeLinha)
    {
    case 1: // Formato simples: {id_classe},{id_assunto},ano
        fprintf(arquivoAlvo, "{%d},{%d},%d\n",
                processoImpresso.id_classe[0],
                processoImpresso.id_assunto[0],
                processoImpresso.ano_eleicao);
        break;

    case 2: // Formato: {id_classe},"{id_assunto1,id_assunto2,...}",ano
        fprintf(arquivoAlvo, "{%d},\"{", processoImpresso.id_classe[0]);

        // Imprimir múltiplos id_assunto
        i = 0;
        while (processoImpresso.id_assunto[i + 1] > 0)
        {
            fprintf(arquivoAlvo, "%d,", processoImpresso.id_assunto[i]);
            i++;
        }
        fprintf(arquivoAlvo, "%d}\",%d\n",
                processoImpresso.id_assunto[i],
                processoImpresso.ano_eleicao);
        break;

    case 3: // Formato: "{id_classe1,id_classe2,...}",{id_assunto},ano
        fprintf(arquivoAlvo, "\"{");

        // Imprimir múltiplos id_classe
        i = 0;
        while (processoImpresso.id_classe[i + 1] > 0)
        {
            fprintf(arquivoAlvo, "%d,", processoImpresso.id_classe[i]);
            i++;
        }
        fprintf(arquivoAlvo, "%d}\",{%d},%d\n",
                processoImpresso.id_classe[i],
                processoImpresso.id_assunto[0],
                processoImpresso.ano_eleicao);
        break;

    case 4: // Formato: "{id_classe1,id_classe2,...}","{id_assunto1,id_assunto2,...}",ano
        fprintf(arquivoAlvo, "\"{");

        // Imprimir múltiplos id_classe
        i = 0;
        while (processoImpresso.id_classe[i + 1] > 0)
        {
            fprintf(arquivoAlvo, "%d,", processoImpresso.id_classe[i]);
            i++;
        }
        fprintf(arquivoAlvo, "%d}\",\"{", processoImpresso.id_classe[i]);

        // Imprimir múltiplos id_assunto
        i = 0;
        while (processoImpresso.id_assunto[i + 1] > 0)
        {
            fprintf(arquivoAlvo, "%d,", processoImpresso.id_assunto[i]);
            i++;   
        }
        fprintf(arquivoAlvo, "%d}\",%d\n",
                processoImpresso.id_assunto[i],
                processoImpresso.ano_eleicao);
        break;
    }
}

void moverCursor(FILE *arquivo, size_t BytesVoltados)
{
    fseek(arquivo, BytesVoltados, SEEK_CUR);
}

void limparArrays(PROCESSO *processo)
{
    for (int i = 0; i < 5; i++)
    {
        processo->id_classe[i] = 0;
        processo->id_assunto[i] = 0;
    }
}

void listarProcessosComMaisDeUmAssunto(PROCESSO *lista, int totalProcessos, FILE *arquivoNovo)
{
    fprintf(arquivoNovo,"Processos com mais de um assunto:\n\n");

    for (int i = 0; i < totalProcessos; i++)
    {
        int contador = 0;
        for (int j = 0; j < 5; j++)
        {
            if (lista[i].id_assunto[j] != 0)
                contador++;
        }

        if (contador > 1)
        {
            fprintf(arquivoNovo,"Linha: %d : ", i+1); 
            imprimirProcesso(arquivoNovo, lista[i]);
        }
    }
}

void atribuirProcessosAStruct(FILE *velhoArquivo, PROCESSO *listaDeProcessos, int nmrDeLinhas){
    
    int indice = 0; char charDif;

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
        
        indice++;
    }

}


#endif
