#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

// "ah pq a função "voltarLinha" tem um -2?" Sei lá, foi o numero que tava dando certo, sem o -2 ele começava a ler do terceiro caracter da linha. Eu imagino que o 2 adicionais ai sejam o \n da linha e a discrepância do csv nao começar a contar em 0

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
    int id_classe[2];
    int id_assunto[2];
    
    // data geral
    int ano_eleicao;
    
    // id;numero ; data_ajuizamento ; id_classe ; id_assunto ; ano_eleicao

} processo;

void voltarCursor(FILE *arquivo, size_t BytesVoltados);
void imprimirProcesso(FILE *arquivoAlvo, processo processoImpresso, int tipoDeLinha);

int main() {

    setlocale(LC_NUMERIC, "pt_BR"); // Define a formatação numérica para o Brasil

    FILE *new, *og; processo pLinha;
    char cabecalho[100], primeiraLinha[100], counter, leituraDif;
    int tipo = 0, nmrDeLinhas = 0;

    og = fopen("processo_043_202409032338.csv", "r");
    new = fopen("arquivo de teste.csv", "w" );

    if (og == NULL) { printf("Erro ao abrir o arquivo \"processo_043_202409032338.csv\".\n"); exit(1); }
    if (new == NULL) { printf("Erro ao abrir o arquivo \"teste.csv\"\n."); exit(1); }
    
    while((counter = fgetc(og)) != EOF) { if(counter == '\n') nmrDeLinhas++; } // percorre até o final do arquivo contando as linhas (\n)
    rewind(og); // volta para o início do arquivo
    fscanf(og, "%[^\n]\n", cabecalho); // armazena o cabeçalho
    

    printf("%d\n", nmrDeLinhas);
    printf("%s\n", cabecalho);
    
    //fprintf(new,"%s\n",cabecalho);

    for (int i = 0; i < nmrDeLinhas; i++) {
        
        fscanf(og, "%[^\n]\n", primeiraLinha);
        voltarCursor(og, -strlen(primeiraLinha) - 2); 

        switch (primeiraLinha[57]){
            case '{':
            fscanf(og, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,{%d},",
                &pLinha.id,
                pLinha.numero,
                &pLinha.ano,
                &pLinha.mes,
                &pLinha.dia,
                &pLinha.horas,
                &pLinha.minutos,
                &pLinha.segundos,
                &pLinha.id_classe[0]);

                leituraDif = fgetc(og);
                voltarCursor(og, -1);

                switch (leituraDif) {
                    case '{':
                    fscanf(og,"{%d},%d\n", 
                        &pLinha.id_assunto[0],
                        &pLinha.ano_eleicao); 
                        tipo = 1;
                    break;

                    case'\"':
                    fscanf(og,"\"{%d,%d}\",%d",
                        &pLinha.id_assunto[0],
                        &pLinha.id_assunto[1],
                        &pLinha.ano_eleicao);
                        tipo = 3;
                    break;
                }


                imprimirProcesso(new, pLinha, tipo);

            break;

            case '\"':
            fscanf(og, "%d,\"%[^\"]\",%d-%d-%d %d:%d:%f,\"{%d,%d}\",{%d},%d\n",
                &pLinha.id,
                pLinha.numero,
                &pLinha.ano,
                &pLinha.mes,
                &pLinha.dia,
                &pLinha.horas,
                &pLinha.minutos,
                &pLinha.segundos,
                &pLinha.id_classe[0],
                &pLinha.id_classe[1],
                &pLinha.id_assunto,
                &pLinha.ano_eleicao);

                leituraDif = fgetc(og);
                voltarCursor(og, -1);

                switch (leituraDif) {
                    case '{':
                    fscanf(og,"{%d},%d\n", 
                        &pLinha.id_assunto[0],
                        &pLinha.ano_eleicao); 
                        tipo = 2;
                    break;

                    case'\"':
                    fscanf(og,"\"{%d,%d}\",%d",
                        &pLinha.id_assunto[0],
                        &pLinha.id_assunto[1],
                        &pLinha.ano_eleicao);
                        tipo = 4;
                    break;
                }



                imprimirProcesso(new, pLinha, tipo);
            break;
            
            }
    }


    //405287812,"06000824620216070000",2021-07-01 16:33:15.000,{12377},{11778},2020

}

void voltarCursor(FILE *arquivo, size_t BytesVoltados) { fseek(arquivo, BytesVoltados, SEEK_CUR);}

void imprimirProcesso(FILE *arquivoAlvo, processo processoImpresso, int tipoDeLinha) {

    switch (tipoDeLinha){
        case 1:
        fprintf(arquivoAlvo, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,{%d},{%d},%d\n",
            processoImpresso.id,
            processoImpresso.numero,
            processoImpresso.ano,
            processoImpresso.mes,
            processoImpresso.dia,
            processoImpresso.horas,
            processoImpresso.minutos,
            processoImpresso.segundos,
            processoImpresso.id_classe[0],
            processoImpresso.id_assunto[0],
            processoImpresso.ano_eleicao); 
        break;

        case 2:
        fprintf(arquivoAlvo, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,\"{%d,%d}\",{%d},%d\n",
            processoImpresso.id,
            processoImpresso.numero,
            processoImpresso.ano,
            processoImpresso.mes,
            processoImpresso.dia,
            processoImpresso.horas,
            processoImpresso.minutos,
            processoImpresso.segundos,
            processoImpresso.id_classe[0],
            processoImpresso.id_classe[1],
            processoImpresso.id_assunto[0],
            processoImpresso.ano_eleicao); 
        break;

        case 3:
        fprintf(arquivoAlvo, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,{%d}\"{%d,%d}\",%d\n",
            processoImpresso.id,
            processoImpresso.numero,
            processoImpresso.ano,
            processoImpresso.mes,
            processoImpresso.dia,
            processoImpresso.horas,
            processoImpresso.minutos,
            processoImpresso.segundos,
            processoImpresso.id_classe[0],
            processoImpresso.id_assunto[0],
            processoImpresso.id_assunto[1],
            processoImpresso.ano_eleicao); 
        break;

        case 4:
        fprintf(arquivoAlvo, "%d,\"%s\",%d-%d-%d %d:%d:%.3f,\"{%d,%d}\",\"{%d,%d}\",%d\n",
            processoImpresso.id,
            processoImpresso.numero,
            processoImpresso.ano,
            processoImpresso.mes,
            processoImpresso.dia,
            processoImpresso.horas,
            processoImpresso.minutos,
            processoImpresso.segundos,
            processoImpresso.id_classe[0],
            processoImpresso.id_classe[1],
            processoImpresso.id_assunto[0],
            processoImpresso.id_assunto[1],
            processoImpresso.ano_eleicao); 
        break;



    } 


}
