#include "PROCESSO.h"

int main(){
    
    // Declaracao de Variaveis -------------------------------------------
    FILE *novoArquivo, *velhoArquivo;
    char parametroDeLinha, charDif, header[100];
    int indice = 0, nmrDeLinhas = 0;

    velhoArquivo = fopen("processo_043_202409032338.csv", "r");
    novoArquivo = fopen("arquivo de teste.csv", "w");

    if (velhoArquivo == NULL) {printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (novoArquivo == NULL) {printf("Erro ao abrir o arquivo \"teste.csv\"\n."); exit(1); }
    // --------------------------------------------------------------------

    
    // Preparacao do Arquivo ----------------------------------------------
    while((parametroDeLinha = fgetc(velhoArquivo)) != EOF) { if(parametroDeLinha == '\n') nmrDeLinhas++; } // percorre até o final do arquivo contando as linhas (\n)
    rewind(velhoArquivo); // volta para o início do arquivo
    fscanf(velhoArquivo, "%[^\n]\n", header); // armazena o cabeçalho

    PROCESSO *listaDeProcessos = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas);
    int *vetorDeIDs_Unicos = (int *) calloc (nmrDeLinhas, sizeof(int));
    
    atribuirProcessosAStruct(velhoArquivo, listaDeProcessos, nmrDeLinhas);
    // --------------------------------------------------------------------
    
    /* 
    * Aqui começa as funcoes para organizar o arquivo, enquanto nao tem um do-while
    * Pra ficar repetindo ate o usuario cansar, todas ficarao em comentarios, caso 
    * Queira testar, tire os comentarios, uma funcao por vez
    */ 

    // Ordenar pela data (decrescente) ------------------------------------ 
    /* fprintf(novoArquivo, "%s\n", header);
    organizarPorData(listaDeProcessos, nmrDeLinhas, novoArquivo); */
    // --------------------------------------------------------------------
    

    // Contar a quanto tempo um processo esta em tramitacao --------------- 
    /* imprimirdias(listaDeProcessos, nmrDeLinhas, novoArquivo); */
    // --------------------------------------------------------------------
    

    // Listar processos com mais de um id assunto ------------------------- 
    /* listarProcessosComMaisDeUmAssunto(listaDeProcessos, nmrDeLinhas, novoArquivo); */
    // --------------------------------------------------------------------
    

    // Impressao dos processos organizados por id (crescente) -------------
    /* PROCESSO *listaDeProcessosOrdenada = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas + 1);
    
    for(int i = 0; i < nmrDeLinhas; i++) {copiarProcesso(&listaDeProcessosOrdenada[i], &listaDeProcessos[i]);}
    listaDeProcessosOrdenada[nmrDeLinhas].id = 0;
    
    ordenarVetorPorIdCrescente(listaDeProcessosOrdenada, nmrDeLinhas);
    fprintf(novoArquivo,"id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao\n");
    for (int i = 0; i < nmrDeLinhas; i++) {imprimirProcesso(novoArquivo, listaDeProcessosOrdenada[i]);} */
    // --------------------------------------------------------------------
    
    
    // Impressao dos processos na ordem original -------------------------- 
    /*fprintf(novoArquivo, "%s\n", header);
    for (int i = 0; i < nmrDeLinhas; i++) {imprimirProcesso(novoArquivo, listaDeProcessos[i]);} */
    // --------------------------------------------------------------------
    
    
    // Impressao dos processos organizados por id classe ------------------
    /* contarIDs_ClasseUnicos(listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas, novoArquivo);
    imprimirProcessoPorIdClasse(novoArquivo, listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas); */
    // --------------------------------------------------------------------
    
    
    // Contar quandos id assunto tem no arquivo e imprimi-los -------------
    /* contarIDs_AssuntoUnicos(listaDeProcessos, vetorDeIDs_Unicos, nmrDeLinhas, novoArquivo); */
    // --------------------------------------------------------------------    
    



    // Evitar vazamento de memoria ----------------------------------------
    free(listaDeProcessos); free(vetorDeIDs_Unicos);
    fclose(velhoArquivo); fclose(novoArquivo);
    // --------------------------------------------------------------------
    
    return 0;

}



