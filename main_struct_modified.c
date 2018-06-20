#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

int M, N, T; /*Em ordem Linhas, Colunas, Threads*/
float V;    /*Valor a ser buscado*/
float **matriz;/*Matriz que será alocada dinamicamente*/
float **auxiliar;/*Matriz AUXILIAR que será alocada dinamicamente*/
FILE *dados;
char nome[21];


//obs reescrever para ler uma struct com os dados e um vetor com todos os elementos para achar
void* busca(void *t){/*Metodo de busca do valor na matriz*/

	int i = 0, j=0;

	int id = *((int *) t); //Índice Numérico da THREAD
	
	printf("Entrando no metodo de busca da THREAD: %d\n", id);

	printf("ID THREAD: %d\n\n",id);
	
	while(id<M){
		printf("ID[WHILE]: %d\n\n",id);
		printf("Valor encontrado: %f (WHILE)\n", matriz[id][i]); // nao printa nada pois nao consegue ler a matriz
		printf("Posição: [%d][%d] (WHILE)\n",id,i);
		
		for(i = 0; i<N ; i++){
			
			printf("Valor encontrado: %f (FOR)\n", matriz[id][i]);
			printf("Posição: [%d][%d] (FOR)\n",id,i);
			printf("Var i = %d\n",i);
			
			if(matriz[id][i]==V){
				
				auxiliar[id][i] = 1;
				printf("Valor encontrado: %f (IF)\n", auxiliar[id][i]);
				printf("Posicao: [%d][%d] (IF)\n",id,i);
			
			}
		
		id = id + T;
		printf("ID Pos Incremento: %f\n\n",id);
						
		}	
	}
	
	pthread_exit(0);
	
}

void criaThread(int num_threads){  /*Método para criação de threads separadas!(antigo dontthreadonme) com alteração para receber a matriz*/

	printf("Entra na funcao criaThread;\n");

	pthread_t *thread = NULL;
        int *argumentos = NULL;
        int c=0;//contador
		int rc;
		//int c = T; /* [Andre] NÃo entendi o porque desse c = T !! */

        /*[Andre] Alocacao dinamica do vetor com os identificadores dos threads*/
        thread = malloc(sizeof(pthread_t) * num_threads);
        if (thread == NULL)
        {
            perror("Probemas na criacao do vetor de identificadores de threads\n");
            exit(1);
        }
        
        /*[Andre] Alocacao dinamica do vetor com os argumentos dos threads*/
        argumentos = malloc(sizeof(int) * num_threads);
        if (argumentos == NULL)
        {
            perror("Probemas na criacao do vetor de argumentos de threads\n");
            exit(1);
        }
		
		printf("Entra na funcao ;\n");
		
	for(c = 0; c < num_threads; c++){
		printf("Criando a thread %d\n",c);
                argumentos[c] = c; 

		rc = pthread_create(&thread[c], NULL, busca,(void*) &argumentos[c]);
		if (rc){
			printf("ERROR: return code from pthread_create is %d\n", rc);
			exit(-1);
		} 

	}

        /*[Andre] Adicionado para sincronizar as threads.*/
        for (c=0; c < num_threads; c++) 
            pthread_join(thread[c], NULL);

        /*[Andre] Liberando as alocacoes dinamicas*/
        free(thread);
        free(argumentos);
}

int main(){
	
	scanf("%d %d %d %f" , &M,&N,&T,&V);
	
	typedef struct{
		
		float matriz[M][N];
		int auxiliar[M][N];
		int lin;
		int col;
		
	}threadata;
	
	threadata td;
	
	td.lin = M;
	td.col = N;
	
	scanf("%s", nome); //Recebe o nome do arquivo

	dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
	if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return -1;
	}

	int linha, coluna;

    for(linha = 0; linha < M; linha++){
        for(coluna = 0; coluna < N; coluna++){
                fscanf(dados, "%f", &td.matriz[linha][coluna]);
                printf("PREENCHE MATRIZ ---> [%d][%d] = %.2f\n",linha,coluna,td.matriz[linha][coluna]);
        }
    }

	//preencheMatriz(dados,matriz, M, N);
	
	int i_t, j_t;
	
	//print para mostrar que está sim sendo lida
	for(i_t=0;i_t<M;i_t++){
		for(j_t=0;j_t<N;j_t++){
			//auxiliar[i][j]=matriz[i][j];
			printf("Matriz na Posicao MAIN [%d][%d] = %f\n",i_t,j_t,td.matriz[i_t][j_t]);
		}
	}
	
	criaThread(T);
	int x = 0;
	int y = 0;
	while(x < M){
		while(y< N){
			if (auxiliar[x][y] == 1){
				printf("Valor encontrado na posicao %d, %d", x, y);
			}
			y = y + 1;
		}
		x = x + 1;
	}
	
	//matriz = liberaMatriz(M, N, matriz);
	
	/*int  i;
  	for (i=0; i<M; i++) free (mat[i]); //libera as linhas da matriz
  	free (mat);      //libera a matriz
  	return (NULL); //retorna um ponteiro nulo*/
	
	//auxiliar = liberaMatriz(M, N, auxiliar);
	
	fclose(dados);	
	pthread_exit(NULL);
}

