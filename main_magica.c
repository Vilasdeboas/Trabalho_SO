#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matriz.h"
#include <stdint.h>

#define M 2
#define N 2
#define T 2
#define V 4

//int M=0, N=0; 
//int T; /*Em ordem Linhas, Colunas, Threads*/
//float V;    /*Valor a ser buscado*/
FILE *dados;
char nome[21];

	typedef struct{
		
		float matriz[M][N];
		int auxiliar[M][N];
		int lin;
		int col;
		
	}thread_data;


void* busca(void *t){/*Metodo de busca do valor na matriz*/

	int i = 0, j=0;

	thread_data *td = (thread_data*) t;

	for(i=0;i<M;i++){
		for(j=0;j<N;j++){
			td->auxiliar[i][j]=td->matriz[i][j];
			printf("Matriz na Posicao BUSCA [%d][%d] = %f\n",i,j,td->matriz[i][j]);
		}
	}

	int id = *((int *) t); //Índice Numérico da THREAD
	
	printf("Entrando no metodo de busca da THREAD: %d\n", id);
	
	printf("ID THREAD: %d\n\n",id);
	
	while(id<M){
		/*printf("ID[WHILE]: %d\n\n",id);
		printf("Valor encontrado: %d (WHILE)\n", matriz[id][i]);
		printf("Posição: [%d][%d] (WHILE)\n",id,i);*/
		
		for(i = 0; i<N ; i++){
			
			/*printf("Valor encontrado: %d (FOR)\n", matriz[id][i]);
			printf("Posição: [%d][%d] (FOR)\n",id,i);*/
			printf("Var i = %d\n",i);
			
			if(td->matriz[id][i]==V){
				
				td->auxiliar[id][i] = 1;
				printf("Valor encontrado: %d (IF)\n", td->auxiliar[id][i]);
				printf("Posicao: [%d][%d] (IF)\n",id,i);
			
			}
		
		id = id + T;
		printf("ID Pos Incremento: %d\n\n",id);
						
		}	
	}
	
	pthread_exit(0);
	
}

int main(){
	
	
	//scanf("%d %d %d %f" , &M,&N,&T,&V);
	
	thread_data td;

	td.lin = M;
	td.col = N;
	
	scanf("%s", nome); //Recebe o nome do arquivo

	dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
	if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return 1;
	}

	int linha, coluna;

    for(linha = 0; linha < M; linha++){
        for(coluna = 0; coluna < N; coluna++){
                fscanf(dados, "%f", &td.matriz[linha][coluna]);
                //td.matriz[linha][coluna];
                printf("PREENCHE MATRIZ ---> [%d][%d] = %.2f\n",linha,coluna,td.matriz[linha][coluna]);
        }
    }
	
	int i_t, j_t;
	//Printa o Valor da Matriz - Inicio
	for(i_t=0;i_t<M;i_t++){
		for(j_t=0;j_t<N;j_t++){
			//auxiliar[i][j]=matriz[i][j];
			printf("Matriz na Posicao MAIN [%d][%d] = %f\n",i_t,j_t,td.matriz[i_t][j_t]);
		}
	}
	//Fim
	
	//criaThread(T);
	
	pthread_t *thread = NULL;
        int *argumentos = NULL;
        int c=0;//contador

	int rc;

        /*[Andre] Alocacao dinamica do vetor com os identificadores dos threads*/
        thread = malloc(sizeof(pthread_t) * T);
        if (thread == NULL)
        {
            perror("Probemas na criacao do vetor de identificadores de threads\n");
            exit(1);
        }
        
        /*[Andre] Alocacao dinamica do vetor com os argumentos dos threads*/
        argumentos = malloc(sizeof(int) * T);
        if (argumentos == NULL)
        {
            perror("Probemas na criacao do vetor de argumentos de threads\n");
            exit(1);
        }

	for(c = 0; c < T; c++){
		printf("Criando a thread %d\n",c);
                argumentos[c] = c; 

		rc = pthread_create(&thread[c], NULL, busca,(void*) &td);
		if (rc){
			printf("ERROR: return code from pthread_create is %d\n", rc);
			exit(-1);
		} 

	}

        /*[Andre] Adicionado para sincronizar as threads.*/
        for (c=0; c < T; c++) 
            pthread_join(thread[c], NULL);

        /*[Andre] Liberando as alocacoes dinamicas*/
        free(thread);
        free(argumentos);
	
	int x = 0;
	int y = 0;
	while(x < M){
		while(y< N){
			if (td.auxiliar[x][y] == 1){
				printf("Valor encontrado na posicao %d, %d", x, y);
			}
			y = y + 1;
		}
		x = x + 1;
	}

	
	fclose(dados);	
	pthread_exit(NULL);
}

