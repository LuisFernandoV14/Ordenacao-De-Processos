#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Processo {

    //identificaÃ§Ã£o
    int id;
    char numero[50];

    // data que o processo foi aberto
    int mes;
    int ano;
    int dia;
    // horÃ¡rio que o processo foi aberto
    int horas;
    int minutos;
    float segundos;        
    
    // identificaÃ§Ã£o especÃ­fica
    int id_classe[5];
    int id_assunto[5];
    
    // data geral
    int ano_eleicao;

    // (existem processos que tem mais de 1 valor em id classe, id assunto ou nos dois)
    int tipoDeLinha;
    
} PROCESSO;

int escolherOrdem(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, FILE *arquivoAlvo, int nmrDeLinhas, int escolha);
void moverCursor(FILE *arquivo, size_t BytesVoltados);
void imprimirProcesso(FILE *arquivoAlvo, PROCESSO processoImpresso);
void limparArrays(PROCESSO *processo);
void atribuirProcessosAStruct(FILE *arquivoBase, PROCESSO *vetorDeProcessos, int numeroDeLinhas);
void copiarProcesso(PROCESSO *p1, PROCESSO *p2);
void contarIDs_ClasseUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas);
void contarIDs_AssuntoUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar);
void limparNumerosRepetidos(int *vetorDeIDs_Unicos);


int main(){

    setlocale(LC_ALL, "Portuguese");

    FILE *novoArquivo, *velhoArquivo, *arquivoAuxiliar;
    char parametroDeLinha, charDif, header[100];
    int nmrDeLinhas = 0; 

    velhoArquivo = fopen("processo_043_202409032338.csv", "r");
    novoArquivo = fopen("arquivo de teste.csv", "w");
    arquivoAuxiliar = fopen("arquivo-Auxiliar.csv", "w");

    if (velhoArquivo == NULL) {printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (novoArquivo == NULL) {printf("Erro ao abrir o arquivo \"teste.csv\".\n"); exit(1); }
    if (arquivoAuxiliar == NULL) {printf("Erro ao abrir o arquivo \"arquivo-Auxiliar.csv\".\n"); exit(1); }
    
    // percorre atÃ© o final do arquivo contando as linhas (\n)
    while((parametroDeLinha = fgetc(velhoArquivo)) != EOF) { if(parametroDeLinha == '\n') nmrDeLinhas++; } 

    // volta para o inÃ­cio do arquivo
    rewind(velhoArquivo); 
    // armazena o cabeÃ§alho
    fscanf(velhoArquivo, "%[^\n]\n", header); 

    // Declara dinamicamente dois vetores, um para todos os processos e um para todos "id_classe" Ãºnicos
    PROCESSO *listaDeProcessos = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas); 
    int *vetorDeIDs_Unicos = (int *) calloc (nmrDeLinhas, sizeof(int));

    // Bota todos os processos em uma struct prÃ³pria
    atribuirProcessosAStruct(velhoArquivo, listaDeProcessos, nmrDeLinhas);
    

    int erro; int escolha; char option = 'y';
    do {

        printf("\n\nEm qual ordem você deseja organizar o arquivo?\n\n\t 1 - Ordem Original\n\t 2 - Organizar por Id Classe\n\t 3 - Contar quantos Id Assuntos unico aparecem\nR:");
        scanf("%d", &escolha);
        getchar();

        erro = escolherOrdem(listaDeProcessos, vetorDeIDs_Unicos, arquivoAuxiliar, nmrDeLinhas, escolha);

        if (erro) {printf("\n\nOpção Inválida.");}

        printf("\n\nDeseja trocar a ordem novamente? (y/n) \nR:");
        fflush(stdin);
        scanf("%c", &option);
        getchar();

    } while( option = 'y');

    // Evita vazamento de memÃ³ria
    free(listaDeProcessos); free(vetorDeIDs_Unicos);
    fclose(arquivoAuxiliar); fclose(velhoArquivo); fclose(novoArquivo);
    
    return 0;
}


int escolherOrdem(PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, FILE *arquivoAlvo, int nmrDeLinhas, int escolha) {

    switch (escolha) {
    
    case 1: // Imprimir na ordem original
        fprintf(arquivoAlvo, "id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
        for (int i = 0; i < nmrDeLinhas; i++) {imprimirProcesso(arquivoAlvo, listaDeProcessos[i]);}
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

    default:
        return 1;
    break;
    
    }

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

        // Verificar se o prÃ³ximo caractere Ã© aspas ou chave
        charDif = fgetc(arquivoBase);
        moverCursor(arquivoBase, -1);
        
        if (charDif == '{') { // id_classe comeÃ§a com {
            char buffer[100];
            // Verificar se Ã© um Ãºnico ID de classe ou mÃºltiplos
            fscanf(arquivoBase, "{%[^}]}", buffer);
            
            // Processar o buffer para extrair os valores
            char *token = strtok(buffer, ",");
            int i = 0;
            while (token != NULL && i < 5) {
                vetorDeProcessos[indice].id_classe[i] = atoi(token);
                token = strtok(NULL, ",");
                i++;
            }
            
            // Ler o prÃ³ximo caractere apÃ³s }
            charDif = fgetc(arquivoBase);
            moverCursor(arquivoBase, -1);
            
            // Verificar se o prÃ³ximo caractere Ã© vÃ­rgula
            if (charDif == ',') {
                fgetc(arquivoBase); // consumir a vÃ­rgula
                
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
        } else if (charDif == '"') { // id_classe comeÃ§a com "
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
            
            // Ler o prÃ³ximo caractere apÃ³s "
            charDif = fgetc(arquivoBase);
            
            // Verificar se o prÃ³ximo caractere Ã© vÃ­rgula
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
    
    // Parte comum da formataÃ§Ã£o para todos os tipos de linha
    fprintf(arquivoAlvo, "%d,\"%s\",%04d-%02d-%02d %02d:%02d:%06.3f,",
        processoImpresso.id,
        processoImpresso.numero,
        processoImpresso.ano,
        processoImpresso.mes,
        processoImpresso.dia,
        processoImpresso.horas,
        processoImpresso.minutos,
        processoImpresso.segundos);
    
    // FormataÃ§Ã£o especÃ­fica para cada tipo de linha
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
            
            // Imprimir mÃºltiplos id_assunto
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
            
            // Imprimir mÃºltiplos id_classe
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
            
            // Imprimir mÃºltiplos id_classe
            i = 0;
            while (processoImpresso.id_classe[i+1] > 0) {
                fprintf(arquivoAlvo, "%d,", processoImpresso.id_classe[i]);
                i++;
            }
            fprintf(arquivoAlvo, "%d}\",\"{", processoImpresso.id_classe[i]);
            
            // Imprimir mÃºltiplos id_assunto
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

void contarIDs_ClasseUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar) {

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

void contarIDs_AssuntoUnicos(PROCESSO *vetorDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas, FILE *auxiliar) {

    static int limiteInteracoes = 0;
    int indice = 0, k, l, seraSeRepete, enderecoVetor;


        while (indice < nmrDeLinhas){

            k = 0, l = 0, seraSeRepete = 1, enderecoVetor = 0;

            while( vetorDeProcessos[indice].id_assunto[k] > 0 ) {

                while( l < limiteInteracoes) {

                    if(  vetorDeProcessos[indice].id_assunto[k] == vetorDeIDs_Unicos[enderecoVetor]) {

                        seraSeRepete = 0;

                    }

                    enderecoVetor++;
                    l++;
                }

                if (seraSeRepete > 0) {


                    //fprintf(auxiliar, "Linha: %d, ID: %d\n", indice + 1,vetorDeProcessos[indice].id_classe[k]);
                    vetorDeIDs_Unicos[limiteInteracoes] = vetorDeProcessos[indice].id_assunto[k];
                    limiteInteracoes = limiteInteracoes + 1;


                }

                k++;

            }

            indice++;

        }

        int tamanhoDoVetor = 0;

        limparNumerosRepetidos(vetorDeIDs_Unicos);
        while (vetorDeIDs_Unicos[tamanhoDoVetor] > 0) {tamanhoDoVetor++;}

        fprintf(auxiliar, "Existem %d nÃºmeros de \"id_assunto\" Ãºnicos, sÃ£o eles: \n\n{%d}", tamanhoDoVetor, vetorDeIDs_Unicos[0]);
        for(int i = 1; vetorDeIDs_Unicos[i] > 0; i++ ) {
            if (i % 4 == 0) {

                fprintf(auxiliar, "\n");
                fprintf(auxiliar,"{%d}", vetorDeIDs_Unicos[i]);
                continue;
            }
            
            fprintf(auxiliar,"\t, {%d}", vetorDeIDs_Unicos[i]);
        }

}

void limparNumerosRepetidos(int *vetorDeIDs_Unicos) {

    int tamanhoDoVetor = 0;

    while (vetorDeIDs_Unicos[tamanhoDoVetor] > 0) {tamanhoDoVetor++;}

    printf("%d ", tamanhoDoVetor);

    for (int i = 0; i < tamanhoDoVetor; i++) {
        int repetido = vetorDeIDs_Unicos[i]; 

        if (repetido == 0) continue;

        for (int j = i + 1; j < tamanhoDoVetor; j++) {

            if (repetido == vetorDeIDs_Unicos[j]) {
              
                vetorDeIDs_Unicos[j] = vetorDeIDs_Unicos[tamanhoDoVetor - 1];
                vetorDeIDs_Unicos[tamanhoDoVetor - 1] = 0;
                tamanhoDoVetor--;
                j--;

            }
        }
    }
}

void imprimirProcessoPorIdClasse(FILE *arquivoAlvo, PROCESSO *listaDeProcessos, int *vetorDeIDs_Unicos, int nmrDeLinhas) {
    int j = 0, contadorDeProcessos = 0;;
    
    limparNumerosRepetidos(vetorDeIDs_Unicos);

    while (vetorDeIDs_Unicos[j] > 0) {
        int id_atual = vetorDeIDs_Unicos[j];
        contadorDeProcessos = 0;

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
        fprintf(arquivoAlvo, "--------------------------------------------------------------------------------\n\n");
    }
}
