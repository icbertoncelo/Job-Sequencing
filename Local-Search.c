#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX 100
int NUM_JOBS = 0; 		// quantidade de tarefas
int DMAX = 0;			// prazo maximo
int timeslot[MAX];		// vetor de tarefas alocadas
int newJobs[MAX];		// vetor de tarefas nao alocadas
int COUNT = 1;			// quantidade de tarefas nao alocadas
int MAXPROFIT = 0;		// lucro maximo

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
	int filledTimeSlot = 0;		// quantidade de tarefas no vetor  
	
	// definir qual é o maior prazo
	for(i = 0; i < NUM_JOBS; i++) {
		if(jobs[i].deadline > DMAX) {
			DMAX = jobs[i].deadline;
		}
	}

	// inicia todos os espaços da sequencia como vazio (-1)
	for(i = 1; i <= DMAX; i++) {
		timeslot[i] = -1;
	}
		
	// algoritmo
	for(i = 1; i <= NUM_JOBS; i++) {
		k = minValue(DMAX, jobs[i - 1].deadline);
		while(k >= 1) {
			if(timeslot[k] == -1) {
				timeslot[k] = i-1;
				filledTimeSlot++;
				break;
			}
			k--;
		}

		// se todos os epaços possiveis foram preenchidos, entao deve-se parar
		if(filledTimeSlot == DMAX) {
			break;
		}
	}
	
	
	printf("\nSequencia Gulosa de Tarefas: ");
	
	// sequencia de tarefas e lucro Maximo
	for(i = 1; i <= DMAX; i++) {
		printf("%s", jobs[timeslot[i]].id);		// mostra as tarefas na sequencia
		maxprofit += jobs[timeslot[i]].profit;	// soma os lucros da sequencia
		
		if(i < DMAX) {
			printf(" --> ");
		}
	}

	printf("\nLucro Maximo Guloso: %d\n", maxprofit);
	MAXPROFIT = maxprofit;
}

// funcao para contar a quantidade de linhas do arquivo (quantidade de tarefas)
void countLines(){		
		
	char url[]="tarefas.txt";
	int numLines = 0;	
	int x, y, z;
	FILE *arq;
	
	arq = fopen(url, "r");
	if(arq == NULL)
		printf("Erro, nao foi possivel abrir o arquivo\n");
	else{
		while (!feof(arq)){
             numLines++;
             fscanf(arq,"%s %d %d\n", &x, &y, &z);
  		}
  	}	
	fclose(arq);
	
	NUM_JOBS = numLines;
}

// ler o arquivo que contem as tarefas
void readJobs(tipoJob jobs[]){
	
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
}

// gerar tarefas com valores aleatorios	
void generateJobs(tipoJob jobs[]){
	 
	int deadline, profit;	
	int i;	// contador de linhas do arquivo

	srand( (unsigned)time(NULL) );
	
	for(i = 0; i < NUM_JOBS; i++){

		deadline = 1 + (rand() % 5);	// valores aleatorios 1  - 10
		profit = 15 + (rand() % 100);	// valores aleatorios 15 - 100
		
		sprintf(jobs[i].id,"%s%d", "J", i);	
		jobs[i].deadline = deadline;
		jobs[i].profit = profit;		
						
	}
}

// ordenar trabalhos de acordo com o lucro (ordem decrescente)
void sortJobs(tipoJob jobs[]){
	
	int i, j;
	tipoJob aux;
	
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

// mostrar dados na tela
void printData(tipoJob jobs[]){
	
	int i, j;
		
	printf("------------ Tarefas ---------------\n\n");
	printf("%10s %10s %10s\n", "Tarefa", "Prazo", "Lucro");
	for(j = 0; j < NUM_JOBS; j++) {
		printf("%10s %10i %10i\n", jobs[j].id, jobs[j].deadline, jobs[j].profit);
	}
			
	sortJobs(jobs);  	
	
	printf("\n--------- Tarefas Ordenadas ---------\n\n");
	printf("%10s %10s %10s\n", "Tarefa", "Prazo", "Lucro");
	for(i = 0; i < NUM_JOBS; i++) {
		printf("%10s %10i %10i\n", jobs[i].id, jobs[i].deadline, jobs[i].profit);
	}	
	
	printf("\nQuantidade de Tarefas: %d", NUM_JOBS);	
}

// definir tarefas que nao foram alocadas
void findJobs(tipoJob jobs[]){
	
	int i, j = 1, aux = 0;
		
	// verificar elementos que nao pertencem ao vetor de tarefas existente
	for (i = 0; i < NUM_JOBS; i++){		
		while(aux == 0 && j <= DMAX){
			if(jobs[i].id == jobs[timeslot[j]].id)  
				aux = 1;  // caso pertenca, aux recebe 1 --> se receber 1, o while para	
			j++;
		}
		
		if (aux == 0) { 		// se aux == 0 (o elemento nao pertence ao vetor de tarefas)
			newJobs[COUNT] = i;	// vetor de tarefas nao alocadas recebe o valor do elemento no vetor de jobs						
			//printf("\n%s\n", jobs[i].id);
			COUNT++;					
		}
		aux = 0;
		j = 1;
	} // for
	
	COUNT--;
}

// melhor vizinho que produz melhoria a cada iteracao
void BL1(tipoJob jobs[]){
	
	int i, j, k, aux1, profitBL1=0;
	int maxprofit = 0;			// maior lucro entre as tarefas
	
	for (i = 1; i <= COUNT; i ++){	// loop nas tarefas nao alocadas
		k = minValue(DMAX, jobs[newJobs[i]].deadline);
		
		// comeca a comparacao
		while(k >= 1) {		
			aux1 = timeslot[k];
			timeslot[k] = newJobs[i];  // efetua a troca dos elementos 
	
			for(j = 1; j <= DMAX; j++) {
				//printf("%s --> ", jobs[timeslot[j]].id);
				maxprofit += jobs[timeslot[j]].profit;	// soma os lucros da sequencia
			}
			//printf("Lucro Total = %d\n", maxprofit);

			// armazena o melhor lucro encontrado
			if (maxprofit > profitBL1)
				profitBL1 = maxprofit;
							
			timeslot[k] = aux1;
			maxprofit = 0;
			k--;
		}
	}
	
	printf("Lucro Maximo BL1: %d\n", profitBL1);
}


// primeiro vizinho que produz melhoria a cada iteracao
void BL2(tipoJob jobs[]){
	
	int i, j, k, aux1, profitBL2 = 0, stop = 0;
	int maxprofit = 0;			// maior lucro entre as tarefas
	
	for (i = 1; i <= COUNT; i ++){	// loop nas tarefas nao alocadas
		k = minValue(DMAX, jobs[newJobs[i]].deadline);
		// comeca a comparacao
		while(k >= 1) {		
			aux1 = timeslot[k];
			timeslot[k] = newJobs[i];  // efetua a troca dos elementos 
	
			for(j = 1; j <= DMAX; j++) {
				//printf("%s --> ", jobs[timeslot[j]].id);
				maxprofit += jobs[timeslot[j]].profit;	// soma os lucros da sequencia
			}
			//printf("Lucro Total = %d\n", maxprofit);
			
			// se o lucro BL1 encontrado for maior que o lucro guloso, a execucao para e lucro gulos0 = lucro BL1
			if (maxprofit > MAXPROFIT){
				profitBL2 = maxprofit;
				stop = 1;	
				break;	
			}
			
			// armazena o melhor lucro encontrado
			if (maxprofit > profitBL2)
				profitBL2 = maxprofit;
				
			timeslot[k] = aux1;
			maxprofit = 0;
			k--;
		}
		
		// se achou uma lucro maior que o lucro guloso, para o loop for
		if (stop == 1)
			break;	
	}
	
	printf("Lucro Maximo BL2: %d\n", profitBL2);
}

int jaEscolhido(int aleatorio[], int num, int i){
	
	int jaEscolhido = 0, j;

	for (j = 1; j < i; j++){
    	if(num == aleatorio[j])	// se o valor gerado for igual aos valores pertencentes no vetor
        	jaEscolhido = 1; // true
	}
	return jaEscolhido;
}

// primeiro vizinho aleatorio que produz melhoria a cada iteracao
void BL3(tipoJob jobs[]){  

	int i, j, k, cont, num, aux1, profitBL3 = 0, stop = 0, maxprofit = 0;
	int aleatorio[COUNT];

	srand( (unsigned)time(NULL) );
	
	// define uma sequencia de numeros aleatorios para fazer as trocas
	for (cont = 1; cont <= COUNT; cont++){
		
    	num = 1 + (rand()%COUNT);	// gerar valor aleatorio
		while (jaEscolhido(aleatorio, num, cont) == 1)	// enquanto o valor for igual a algum existente
			num = 1 + (rand()%COUNT);	// gerar novo valor	
 		
       	aleatorio[cont] = num; 	
       	//printf("\n%d\n", num);
   	}
	
	for (i = 1; i <= COUNT; i ++){	// loop nas tarefas nao alocadas
		k = minValue(DMAX, jobs[newJobs[aleatorio[i]]].deadline);	// pegar o valor na sequencia do velor aleatorio
		// comeca a comparacao

		while(k >= 1) {		
			aux1 = timeslot[k];
			timeslot[k] = newJobs[aleatorio[i]];  // efetua a troca dos elementos
				
			for(j = 1; j <= DMAX; j++) {
				//printf("%s --> ", jobs[timeslot[j]].id);
				maxprofit += jobs[timeslot[j]].profit;	// soma os lucros da sequencia
			}
			//printf("Lucro Total = %d\n", maxprofit);
			
			// se o lucro BL1 encontrado for maior que o lucro guloso, a execucao para e lucro gulos0 = lucro BL1
			if (maxprofit > MAXPROFIT){
				profitBL3 = maxprofit;
				stop = 1;	
				break;	
			}
			
			// armazena o melhor lucro encontrado
			if (maxprofit > profitBL3)
				profitBL3 = maxprofit;
				
			timeslot[k] = aux1;
			maxprofit = 0;
			k--;
		}
		
		// se achou uma lucro maior que o lucro guloso, para o loop for
		if (stop == 1)
			break;	
	}
	
	printf("Lucro Maximo BL3: %d\n", profitBL3); 			
}

int main(void) {
	
	clock_t inicialTime, finalTime;
    double timeSpent;
    
    inicialTime = clock();	
	
	countLines();				// determina NUM_JOBS		
	tipoJob jobs[NUM_JOBS];		// cria uma estrutura jobs do tipoJob
	readJobs(jobs);				// le o arquivo de tarefas
	//generateJobs(jobs);			// gera as tarefas aleatoriamente (deve-se alterar o valor da variavel global NUM_JOBS)
	printData(jobs);			// mostra as tabelas na tela
	
	jobSequencing(jobs);	// algoritmo guloso
	findJobs(jobs);			// determina quais tarefas nao foram alocadas
	BL1(jobs);				
	BL2(jobs);
	BL3(jobs);
		
   	finalTime = clock();	
   	timeSpent = ((finalTime - inicialTime) * 1000/CLOCKS_PER_SEC);		// calcula o tempo de execucao
   	
   	printf("Tempo em milissegundos: %lf", timeSpent);

	return 0;
}
