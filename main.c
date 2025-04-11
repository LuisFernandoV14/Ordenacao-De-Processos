#include "processo.h"

int main(){
    
    // Declaracao de Variaveis -------------------------------------------
    FILE *novoArquivo, *velhoArquivo;
    char parametroDeLinha, charDif, header[100];
    int indice = 0, nmrDeLinhas = 0;

    velhoArquivo = fopen("processo_043_202409032338.csv", "r");

    if (velhoArquivo == NULL) {printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    // --------------------------------------------------------------------

    
    // Preparacao do Arquivo ----------------------------------------------
    while((parametroDeLinha = fgetc(velhoArquivo)) != EOF) { if(parametroDeLinha == '\n') nmrDeLinhas++; } 
    rewind(velhoArquivo);
    fscanf(velhoArquivo, "%[^\n]\n", header);

    PROCESSO *listaDeProcessos = (PROCESSO *) malloc (sizeof(PROCESSO) * nmrDeLinhas);
    int *vetorDeIDs_Unicos = (int *) calloc (nmrDeLinhas, sizeof(int));
    
    atribuirProcessosAStruct(velhoArquivo, listaDeProcessos, nmrDeLinhas);
    // --------------------------------------------------------------------
    

    // Programa -----------------------------------------------------------
    char repetir; int operacao, error;

    limparTela;

    printf("Bem vindo ao programa para organizar o arquivo judicial \"processo_043_202409032338.csv\" \n\n");

    printf("Disclaimer: Como o arquivo tem muitas linhas e processos, as opcoes de ordenar vao demorar um pouco para atualizar\no arquivo, normalmente as mensagens de conclusao vem antes do arquivo ser atualizado. \n\n");

	pausarLeitura; limparTela;

    do{
	    novoArquivo = fopen("Processos Judiciais Organizados.csv", "w");
	    if (novoArquivo == NULL) {printf("Erro ao abrir o arquivo \"Processos Judiciais Organizados.csv\"\n."); exit(1); }
	    
        printf("Em qual ordem voce deseja organizar o arquivo? Digite apenas o numero\n");

        printf("\n\t1 - Organizados pelo numero de ID (ordem crescente)\n\t2 - Organizados pela data de ajuizamento (ordem descrescente)\n\t");
        printf("3 - Organizados por ID da Classe \n\t4 - Constar quantos Assuntos diferentes existem no arquivo usando o numero de ID\n\t");
        printf("5 - Listar todos os processos com mais de um Assunto \n\t6 - Constar a quanto tempo os processos estao em tramitacao na justica\n\t");
        printf("7 - Voltar o arquivo para a ordem original \n\n R: ");

        scanf("%d", &operacao);

        error = escolherOrdem(listaDeProcessos, vetorDeIDs_Unicos, novoArquivo, nmrDeLinhas, operacao);

        if (error) {
            printf("\n\nEscolha invalida. Por favor digite apenas um numero de 1 a 7.");
            printf("\n\n");
			pausarLeitura; limparTela;
            continue;
        }

        printf("\n\n");
        pausarLeitura; limparTela;

        fflush(stdin);
        printf("Deseja reorganizar o arquivo? Responda com 'y' ou 'n'\n\nR: ");
        scanf("%c", &repetir);

		printf("\n\n");
        pausarLeitura; limparTela;
        
        fclose(novoArquivo);

    } while (repetir == 'y');

    printf("Finalizando programa...\n\n");
    printf("Obrigado!");
    pausarLeitura; limparTela;

    // --------------------------------------------------------------------


    // Evitar vazamento de memoria ----------------------------------------
    free(listaDeProcessos); free(vetorDeIDs_Unicos);
    fclose(velhoArquivo);
    // --------------------------------------------------------------------
    
    return 0;

}
