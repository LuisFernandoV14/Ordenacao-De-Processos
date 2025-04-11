# Ordenacao-De-Processos

Esse é um trabalho realizado no terceiro semestre do curso Engenharia de Software, da disciplina Estrutura de Dados, ministrada pelo professor Marcelo Eustáquio, da Universidade Católica de Brasília.

O objetivo é ordenar um arquivo csv, com 18.391 processos judiciais diferentes.

Cada processo segue o seguinte formato:
id;numero;data_ajuizamento;id_classe;id_assunto;ano_eleicao

Usando a linguagem de programação C, fomos capazes de ordenar o arquivo com os seguintes parâmetros:
  1. Ordenar, em ordem crescente, o conjunto de dados a partir do atributo “id”; 
  2. Ordenar, em ordem decrescente, o conjunto de dados a partir do atributo “data_ajuizamento”; 
  3. Contar quantos processos estão vinculados a um determinado “id_classe”; 
  4. Identificar quantos “id_assuntos” constam nos processos presentes na base de dados;  
  5. Listar todos os processos que estão vinculados a mais de um assunto; e 
  6. Indicar a quantos dias um processo está em tramitação na justiça.

Também é parte do projeto fazê-lo seguindo o modelo TAD de abstração de dados.
Por isso, separamos um arquivo unico em C em três arquivos diferentes: main.c, processo.c e processo.h

Essa foi uma grande jornada de aprendizado, evoluímos como programadores e como profissionais.

Programadores que fizeram isso possível:
 * Luís Fernando Ventura Ferreira;
 * Luis Gustavo Lima Fernandes;
 * Pedro Eduardo Da Costa Nascimento;
 * Paulo Vinícius Sousa Lima
   
E é claro, o professor Marcelo Eustaquio Soares De Lima Junior.
