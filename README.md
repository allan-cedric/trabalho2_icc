## Trabalho 2 de Introdução à Computação Científica - CI1164 - UFPR

### Autores
*   Allan Cedric G. B. Alves da Silva - GRR20190351
*   Gabriel N. H. do Nascimento - GRR20190361

### Arquivos do projeto
*   `utils.h/utils.c`: Biblioteca que implementa algumas funções auxiliares
*   `matrix.h/matrix.c`: Biblioteca que implementa estruturas e funções para matrizes
*   `lin_system.h/lin_system.c`: Bibliotecas que implementa estruturas e funções para sistemas lineares, utiliza-se da lib anterior
*   `interpol.h/interpol.c`: Biblioteca que implementa funções geradoras de coeficientes para os problemas de **Interpolação** e **Ajuste de curvas**.
*   `main_optimized.c`: Programa principal otimizado para análise de desempenho, integração com LIKWID
*   `main_unoptimized.c`: Programa principal não otimizado para análise de desempenho, integração com LIKWID
*   `main.c`: Programa principal definitivo e otimizado para usar
*   `analise.py`: Script gerador de tabelas no formato `tsv` para análise de desempenho
*   `gera_plot.py`: Script gerador de gráficos para análise de desempenho, usa as tabelas geradas pelo script anterior
*   `performance.sh`: Script que realiza a análise de desempenho
*   `Resultados/`: Diretório contendo as tabelas e gráficos gerados pela análise de desempenho
*   `RELATORIO-acgbas19-gnhn19.pdf`: Relatório definitivo discorrendo sobre a análise de desempenho realizada

### Execução (Análise de desempenho)
*   `make`
*   `./performance.sh`

### Execução (Sem análise de desempenho)
*   `make`
*   `./main < {entrada}`