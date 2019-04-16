#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX 100
int NUM_JOBS = 0; 		// quantidade de tarefas

/*
OBSERVACAO   --> 	O Programa esta numa forma generica, ele gera uma quantidade n de valores aleatorios para o prazo e o lucro das tarefas. Funcao geraTarefas().
					Para testar o programa para diferentes tamanhos, basta modificar o arquivo "tarefas.txt".
					
					Alem disso o codigo le um arquivo que contem as tarefas a serem processadas no algoritmo.
*/

typedef struct Job {	// define uma estrutura do tipo tarefa
	char id[5];			// nome
	int deadline;		// prazo
	int profit;			// lucro
} tipoJob;

int minValue(int x, int y) {	// função para definir o minimo entre dois valores
	if(x < y) return x;
	return y;
}

void jobSequencing(tipoJob jobs[]) {
	
	int i, k;
	int maxprofit = 0;			// maior lucro entre as tarefas
	int timeslot[MAX];			// sequencia de tarefas
	int filledTimeSlot = 0;		// quantidade de tarefas no vetor  
	int dmax = 0;				// maior prazo 
	
	// definir qual é o maior prazo
	for(i = 0; i < NUM_JOBS; i++) {
		if(jobs[i].deadline > dmax) {
			dmax = jobs[i].deadline;
		}
	}

	// inicia todos os espaços da sequencia como vazio (-1)
	for(i = 1; i <= dmax; i++) {
		timeslot[i] = -1;
	}
	
	// algoritmo
	for(i = 1; i <= NUM_JOBS; i++) {
		k = minValue(dmax, jobs[i - 1].deadline);
		while(k >= 1) {
			if(timeslot[k] == -1) {
				timeslot[k] = i-1;
				filledTimeSlot++;
				break;
			}
			k--;
		}

		// se todos os epaços possiveis foram preenchidos, entao deve-se parar
		if(filledTimeSlot == dmax) {
			break;
		}
	}

	
	printf("\nSequencia de Tarefas: ");
	
	// sequencia de tarefas e lucro Maximo
	for(i = 1; i <= dmax; i++) {
		printf("%s", jobs[timeslot[i]].id);		// mostra as tarefas na sequencia
		maxprofit += jobs[timeslot[i]].profit;	// soma os lucros da sequencia
		
		if(i < dmax) {
			printf(" --> ");
		}
	}

	printf("\nLucro Maximo: %d\n", maxprofit);
}

void contaLinhas(){		// funcao para contar quantas linas o arquivo possui
		
	char url[]="tarefas.txt";
	int numLinhas = 0;	
	int x, y, z;
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		while (!feof(arq)){
             numLinhas++;
             fscanf(arq,"%s %d %d\n", &x, &y, &z);
  		}
  	}	
	fclose(arq);
	
	NUM_JOBS = numLinhas;
}

void geraTarefas(tipoJob jobs[]){
	
	// ler o arquivo que contem as 
	int deadline, profit;	
	char url[]="tarefas.txt";
	int i = -1;	// contador de linhas do arquivo
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		while (!feof(arq)){
			i ++;
			fscanf(arq,"%s %d %d\n", &jobs[i].id, &jobs[i].deadline, &jobs[i].profit);
  		}
  	}	
	fclose(arq);
	
	// esse codigo gera tarefas com valores aleatorios	
	/*srand( (unsigned)time(NULL) );
	
	for(i = 0; i < NUM_JOBS; i++){

		deadline = 1 + (rand() % 15);	// valores aleatorios 1  - 10
		profit = 50 + (rand() % 200);	// valores aleatorios 15 - 100
		
		sprintf(jobs[i].id,"%s%d", "J", i);	
		jobs[i].deadline = deadline;
		jobs[i].profit = profit;		
						
	}*/
}

void ordenaTarefas(tipoJob jobs[]){
	
	int i, j;
	tipoJob aux;
	
	// ordenar trabalhos de acordo com o lucro (ordem decrescente)
	for(i = 1; i < NUM_JOBS; i++) {
		for(j = 0; j < NUM_JOBS - i; j++) {
			if(jobs[j+1].profit > jobs[j].profit) {
				aux = jobs[j+1];
				jobs[j+1] = jobs[j];
				jobs[j] = aux;
			}
		}
	}
}

int main(void) {
	
	int i, j;
	
	clock_t tempoInicial, tempoFinal;
    double tempoGasto;
    
    tempoInicial = clock();	
	
	contaLinhas();			
	tipoJob jobs[NUM_JOBS];	// cria uma estrutura jobs do tipoJob
	geraTarefas(jobs);
		
	printf("------------ Tarefas ---------------\n\n");
	printf("%10s %10s %10s\n", "Tarefa", "Prazo", "Lucro");
	for(j = 0; j < NUM_JOBS; j++) {
		printf("%10s %10i %10i\n", jobs[j].id, jobs[j].deadline, jobs[j].profit);
	}
			
	ordenaTarefas(jobs);	
	
	printf("\n--------- Tarefas Ordenadas ---------\n\n");
	printf("%10s %10s %10s\n", "Tarefa", "Prazo", "Lucro");
	for(i = 0; i < NUM_JOBS; i++) {
		printf("%10s %10i %10i\n", jobs[i].id, jobs[i].deadline, jobs[i].profit);
	}
	
	jobSequencing(jobs);		// chama a funcao (algoritmo)
	
   	tempoFinal = clock();	
   	tempoGasto = ((tempoFinal-tempoInicial) * 1000/CLOCKS_PER_SEC);		// calcula o tempo de execucao
   	
   	printf("Tempo em milissegundos: %lf", tempoGasto);

	return 0;
}
