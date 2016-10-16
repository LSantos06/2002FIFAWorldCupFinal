/* Universidade de Brasilia */
/* Departamento de Ciencia da Computacao */
/* Programacao Concorrente 2/2016 */

/* Trabalho 1 */
/* Lucas Nascimento Santos Souza 14/0151010 */

/* Simulacao de uma partida de futebol utilizando threads */

/*
 * => PARTIDA
 * - A partida que sera simulada eh a final da Copa do Mundo de 2002,
 * entre Brasil e Alemanha;
 *
 * - Escalacao do Brasil (17 jogadores):
 * 		- Goleiros: Marcos e Rogerio Ceni;
 *		- Zagueiros: Lucio, Edmilson, Roque Junior e Anderson Polga;
 *		- Laterais: Cafu, Roberto Carlos e Junior;
 *		- Volantes: Gilberto Silva, Kleberson e Vampeta;
 *		- Meias: Ronaldinho e Kaka;
 *		- Atacantes: Rivaldo, Ronaldo e Denilson.
 *
 * - Escalacao da Alemanha (17 jogadores):
 *		- Goleiros: Kahn e Lehmann;
 *		- Zagueiros: Linke, Rehmer, Meltzfelder e Baumann;
 *		- Laterais: Frings, Bode e Ziege;
 *		- Volantes: Hamann, Jeremies e Ramelow;
 *		- Meias: Schneider e Ballack;
 *		- Atacantes: Klose, Neuville e Jancker.
 *
 * => PRE-JOGO
 * - REGIAO CRITICA: Apenas 11 jogadores podem entrar em campo, portanto os 17 jogadores
 * de cada time disputam uma vaga (em sua respecitiva posicao) para entrar em campo;
 *
 * - Cada time (em campo) eh composto por:
 *		- 1 Goleiro;
 *		- 3 Zagueiros;
 *		- 2 Laterais;
 *		- 2 Volantes;
 *		- 1 Meia;
 *		- 2 Atacantes;
 *
 * => JOGO
 * - A partida ocorre infinitamente;
 *
 * - 11 jogadores (em campo) de cada time, 22 jogadores (em campo) no total;
 *
 * - Os jogadores de cada time foram dividos em threads
 * de acordo com suas posicoes, sendo elas:
 * 		- Goleiro;
 *		- Zagueiro;
 *		- Lateral;
 *		- Volante;
 *		- Meia;
 *		- Atacante.
 *
 * - As acoes de cada tipo de jogador sao as seguintes:
 *		- Goleiro: defender a bola, passar a bola;
 *		- Zagueiro: tomar a bola, passar a bola;
 *		- Lateral: cruzar a bola, passar a bola;
 *		- Volante: tomar a bola, passar a bola;
 *		- Meia:	chutar a bola, passar a bola;
 *		- Atacante: chutar a bola.
 *
 * - REGIAO CRITICA: Apenas 1 jogador pode possuir a bola entre os 22 em campo;
 *
 * - REGIAO CRITICA: O jogador nao pode sair da sua parte do campo (cada posicao
 * possui uma parte especifica do campo):
 *		- Goleiro: GOL;
 *		- Zagueiro: DEFESA;
 *		- Lateral: LADOS;
 *		- Volante: MEIO DEFENSIVO;
 *		- Meia:	MEIO OFENSIVO;
 *		- Atacante: ATAQUE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_ESCALACAO 17

#define NUM_JOGADORES_TIME 11

#define NUM_GOLEIRO 1
#define NUM_ZAGUEIROS 3
#define NUM_LATERAIS 2
#define NUM_VOLANTES 2
#define NUM_MEIA 1
#define NUM_ATACANTES 2

/*************************************************************/
/* Time do Brasil */
pthread_t Brasil[NUM_ESCALACAO-1];

// Escalacao do time do Brasil
pthread_mutex_t lock_goleiros_Brasil;
pthread_mutex_t lock_zagueiros_Brasil;
pthread_mutex_t lock_laterais_Brasil;
pthread_mutex_t lock_volantes_Brasil;
pthread_mutex_t lock_meias_Brasil;
pthread_mutex_t lock_atacantes_Brasil;

sem_t sem_goleiros_Brasil;
sem_t sem_zagueiros_Brasil;
sem_t sem_laterais_Brasil;
sem_t sem_volantes_Brasil;
sem_t sem_meias_Brasil;
sem_t sem_atacantes_Brasil;

int escalacao_goleiro_Brasil = 0;
int escalacao_zagueiros_Brasil = 0;
int escalacao_laterais_Brasil = 0;
int escalacao_volantes_Brasil = 0;
int escalacao_meia_Brasil = 0;
int escalacao_atacantes_Brasil = 0;

int titular_goleiro_Brasil[NUM_GOLEIRO+1];
int titular_zagueiros_Brasil[NUM_ZAGUEIROS+1];
int titular_laterais_Brasil[NUM_LATERAIS+1];
int titular_volantes_Brasil[NUM_VOLANTES+1];
int titular_meia_Brasil[NUM_MEIA+1];
int titular_atacantes_Brasil[NUM_ATACANTES+1];

// Gols
int gols_Brasil = 0;

/*************************************************************/
/* Time da Alemanha */
pthread_t Alemanha[NUM_ESCALACAO-1];

// Escalacao do time da Alemanha
pthread_mutex_t lock_goleiros_Alemanha;
pthread_mutex_t lock_zagueiros_Alemanha;
pthread_mutex_t lock_laterais_Alemanha;
pthread_mutex_t lock_volantes_Alemanha;
pthread_mutex_t lock_meias_Alemanha;
pthread_mutex_t lock_atacantes_Alemanha;

sem_t sem_goleiros_Alemanha;
sem_t sem_zagueiros_Alemanha;
sem_t sem_laterais_Alemanha;
sem_t sem_volantes_Alemanha;
sem_t sem_meias_Alemanha;
sem_t sem_atacantes_Alemanha;

int escalacao_goleiro_Alemanha = 0;
int escalacao_zagueiros_Alemanha = 0;
int escalacao_laterais_Alemanha = 0;
int escalacao_volantes_Alemanha = 0;
int escalacao_meia_Alemanha = 0;
int escalacao_atacantes_Alemanha = 0;

int titular_goleiro_Alemanha[NUM_GOLEIRO+1];
int titular_zagueiros_Alemanha[NUM_ZAGUEIROS+1];
int titular_laterais_Alemanha[NUM_LATERAIS+1];
int titular_volantes_Alemanha[NUM_VOLANTES+1];
int titular_meia_Alemanha[NUM_MEIA+1];
int titular_atacantes_Alemanha[NUM_ATACANTES+1];

// Gols
int gols_Alemanha = 0;

/*************************************************************/
// Posse de bola
pthread_mutex_t lock_posse = PTHREAD_MUTEX_INITIALIZER;
// Regiao do campo
pthread_mutex_t lock_regiao = PTHREAD_MUTEX_INITIALIZER;
// Gols
pthread_mutex_t lock_gol = PTHREAD_MUTEX_INITIALIZER;
// Bola
pthread_mutex_t lock_bola = PTHREAD_MUTEX_INITIALIZER;

/*************************************************************/
// Time que possui a bola
typedef enum times{
	BRASIL, ALEMANHA
}Times;
// Posse de bola inicial
Times posse = BRASIL;
char * getPosse(Times posse){
  switch(posse){
    case BRASIL: return "BRASIL";
    case ALEMANHA: return "ALEMANHA";
  }
}

// Regioes do campo
typedef enum regioes{
	GOL_BRASIL, DEFESA_BRASIL, LADOS, MEIO_OFENSIVO_BRASIL,
	MEIO_DEFENSIVO_BRASIL, ATAQUE_BRASIL, GOL_ALEMANHA
}Regioes;
// Regiao do campo onde o jogo comeca
Regioes regiao = MEIO_OFENSIVO_BRASIL;
char * getRegiao(Regioes regiao){
  // Imprime o valor do estado
  switch(regiao){
    case GOL_BRASIL: return "GOL_BRASIL";
		case DEFESA_BRASIL: return "DEFESA_BRASIL";
		case LADOS:	return "LADOS";
		case MEIO_OFENSIVO_BRASIL: return "MEIO_OFENSIVO_BRASIL";
		case MEIO_DEFENSIVO_BRASIL: return "MEIO_DEFENSIVO_BRASIL";
		case ATAQUE_BRASIL: return "ATAQUE_BRASIL";
		case GOL_ALEMANHA: return "GOL_ALEMANHA";
  }
}

/*************************************************************/
/* Threads dos jogadores do Brasil */
void * thread_goleiro_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(1) Marcos";
		}
		else{
			jogador = "(22) Rogerio Ceni";
		}

		pthread_mutex_lock(&lock_goleiros_Brasil);
		// Incrementa a preferencia do goleiro
		escalacao_goleiro_Brasil++;
		// Contempla todos os candidatos a vaga de goleiro
		if(escalacao_goleiro_Brasil <= (NUM_GOLEIRO+1)){
			// Armazena a preferencia do goleiro em ser titular (maior sendo 0)
			titular_goleiro_Brasil[id] = escalacao_goleiro_Brasil;
		}
		pthread_mutex_unlock(&lock_goleiros_Brasil);

		// printf("%d\n", escalacao_goleiro_Brasil);
		// printf("%d\n", titular_goleiro_Brasil[id]);

		// Jogador selecionado
		if(escalacao_goleiro_Brasil == NUM_GOLEIRO){
			printf("\tBrasil, como goleiro: %s\n", jogador);
			sleep(2);
		}
		// Se a preferencia do goleiro for a menor possivel (1), ele fica no banco
		if(titular_goleiro_Brasil[id] == (NUM_GOLEIRO+1)){
			sem_wait(&sem_goleiros_Brasil);
		}

		// Se a regiao for o gol do Brasil
		if(regiao == GOL_BRASIL){

			// Se a posse de bola eh do Brasil, o goleiro esta com a bola
			if(posse == BRASIL){

				// Goleiro tocou para um zagueiro
				pthread_mutex_lock(&lock_regiao);
				regiao = DEFESA_BRASIL;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Brasil, goleiro %s com a bola\n", jogador);
				printf("Brasil, goleiro %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh da Alemanha, houve um chute
			else{

				// Se a defesa aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Brasil, goleiro %s defendeu\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}
				// Se houve gol
				else{
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);
					pthread_mutex_lock(&lock_gol);
					gols_Alemanha++;
					pthread_mutex_unlock(&lock_gol);
					printf("GOL DA ALEMANHA!\n");
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_zagueiro_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(3) Lucio";
		}
		else if(id == 1){
			jogador = "(5) Edmilson";
		}
		else if(id == 2){
			jogador = "(4) Roque Junior";
		}
		else{
			jogador = "(14) Anderson Polga";
		}

		pthread_mutex_lock(&lock_zagueiros_Brasil);
		// Incrementa a preferencia do zagueiro
		escalacao_zagueiros_Brasil++;
		// Contempla todos os candidatos a vaga de zagueiro
		if(escalacao_zagueiros_Brasil <= (NUM_ZAGUEIROS+1)){
			// Armazena a preferencia do zagueiro em ser titular (maior sendo 0)
			titular_zagueiros_Brasil[id] = escalacao_zagueiros_Brasil;
		}
		pthread_mutex_unlock(&lock_zagueiros_Brasil);

		// printf("%d\n", escalacao_zagueiros_Brasil);
		// printf("%d\n", titular_zagueiros_Brasil[id]);

		// Jogador selecionado
		if(escalacao_zagueiros_Brasil <= NUM_ZAGUEIROS){
			printf("\tBrasil, como %do zagueiro: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do zagueiro for a menor possivel (3), ele fica no banco
		if(titular_zagueiros_Brasil[id] == (NUM_ZAGUEIROS+1)){
			sem_wait(&sem_zagueiros_Brasil);
		}

		// Se a regiao for a defesa do Brasil
		if(regiao == DEFESA_BRASIL){

			// Se a posse de bola eh do Brasil, um zagueiro esta com a bola
			if(posse == BRASIL){

				// Zagueiro tocou para um lateral
				pthread_mutex_lock(&lock_regiao);
				regiao = LADOS;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Brasil, zagueiro %s com a bola\n", jogador);
				printf("Brasil, zagueiro %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh da Alemanha, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Brasil, zagueiro %s tomou e tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_lateral_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(2) Cafu";
		}
		else if(id == 1){
			jogador = "(6) Roberto Carlos";
		}
		else{
			jogador = "(16) Junior";
		}

		pthread_mutex_lock(&lock_laterais_Brasil);
		// Incrementa a preferencia do lateral
		escalacao_laterais_Brasil++;
		// Contempla todos os candidatos a vaga de lateral
		if(escalacao_laterais_Brasil <= (NUM_LATERAIS+1)){
			// Armazena a preferencia do lateral em ser titular (maior sendo 0)
			titular_laterais_Brasil[id] = escalacao_laterais_Brasil;
		}
		pthread_mutex_unlock(&lock_laterais_Brasil);

		// printf("%d\n", escalacao_laterais_Brasil);
		// printf("%d\n", titular_laterais_Brasil[id]);

		// Jogador selecionado
		if(escalacao_laterais_Brasil <= NUM_LATERAIS){
			printf("\tBrasil, como %do lateral: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do zagueiro for a menor possivel (2), ele fica no banco
		if(titular_laterais_Brasil[id] == (NUM_LATERAIS+1)){
			sem_wait(&sem_laterais_Brasil);
		}

		// Se a regiao for os lados do campo
		if(regiao == LADOS){

			// Se a posse de bola eh do Brasil, um lateral esta com a bola
			if(posse == BRASIL){

				// Se um passe aconteceu
				if(chance == 0){
					// Lateral tocou para um volante
					pthread_mutex_lock(&lock_regiao);
					regiao = MEIO_DEFENSIVO_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Brasil, lateral %s com a bola\n", jogador);
					printf("Brasil, lateral %s tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}

				// Se um cruzamento aconteceu
				else{
					// Lateral cruzou para um atacante
					pthread_mutex_lock(&lock_regiao);
					regiao = ATAQUE_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Brasil, lateral %s com a bola\n", jogador);
					printf("Brasil, lateral %s cruzou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_volante_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(8) Gilberto Silva";
		}
		else if(id == 1){
			jogador = "(15) Kleberson";
		}
		else{
			jogador = "(18) Vampeta";
		}

		pthread_mutex_lock(&lock_volantes_Brasil);
		// Incrementa a preferencia do volante
		escalacao_volantes_Brasil++;
		// Contempla todos os candidatos a vaga de volante
		if(escalacao_volantes_Brasil <= (NUM_VOLANTES+1)){
			// Armazena a preferencia do volante em ser titular (maior sendo 0)
			titular_volantes_Brasil[id] = escalacao_volantes_Brasil;
		}
		pthread_mutex_unlock(&lock_volantes_Brasil);

		// printf("%d\n", escalacao_volantes_Brasil);
		// printf("%d\n", titular_volantes_Brasil[id]);

		// Jogador selecionado
		if(escalacao_volantes_Brasil <= NUM_VOLANTES){
			printf("\tBrasil, como %do volante: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do volante for a menor possivel (2), ele fica no banco
		if(titular_volantes_Brasil[id] == (NUM_VOLANTES+1)){
			sem_wait(&sem_volantes_Brasil);
		}

		// Se a regiao for o meio de campo defensivo do Brasil
		if(regiao == MEIO_DEFENSIVO_BRASIL){
			// Se a posse de bola eh do Brasil, um volante esta com a bola
			if(posse == BRASIL){

				// Volante tocou para um meia
				pthread_mutex_lock(&lock_regiao);
				regiao = MEIO_OFENSIVO_BRASIL;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Brasil, volante %s com a bola\n", jogador);
				printf("Brasil, volante %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh da Alemanha, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Brasil, volante %s tomou e tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_meia_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(11) Ronaldinho";
		}
		else{
			jogador = "(23) Kaka";
		}

		pthread_mutex_lock(&lock_meias_Brasil);
		// Incrementa a preferencia do meia, comeca em 1
		escalacao_meia_Brasil++;
		// Contempla todos os candidatos a vaga de meia
		if(escalacao_meia_Brasil <= (NUM_MEIA+1)){
			// Armazena a preferencia do meia em ser titular (maior sendo 1)
			titular_meia_Brasil[id] = escalacao_meia_Brasil;
		}
		pthread_mutex_unlock(&lock_meias_Brasil);

		// printf("%d\n", escalacao_meia_Brasil);
		// printf("%d\n", titular_meia_Brasil[id]);

		// Jogador selecionado
		if(escalacao_meia_Brasil == NUM_MEIA){
			printf("\tBrasil, como %do meia: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do meia for a menor possivel (2), ele fica no banco
		if(titular_meia_Brasil[id] == (NUM_MEIA+1)){
			sem_wait(&sem_meias_Brasil);
		}

		// Se a regiao for o meio de campo ofensivo do Brasil
		if(regiao == MEIO_OFENSIVO_BRASIL){

			// Se a posse de bola eh do Brasil, um meia esta com a bola
			if(posse == BRASIL){

				// Se um passe aconteceu
				if(chance == 0){
					// Meia tocou para um atacante
					pthread_mutex_lock(&lock_regiao);
					regiao = ATAQUE_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Brasil, meia %s com a bola\n", jogador);
					printf("Brasil, meia %s tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}

				// Se um chute aconteceu
				else{
					// Meia chutou para o gol
					pthread_mutex_lock(&lock_regiao);
					regiao = GOL_ALEMANHA;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Brasil, meia %s com a bola\n", jogador);
					printf("Brasil, meia %s chutou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_atacante_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;

	while(1){
		// Nome do jogador
		if(id == 0){
			jogador = "(10) Rivaldo";
		}
		else if(id == 1){
			jogador = "(9) Ronaldo";
		}
		else{
			jogador = "(17) Denilson";
		}

		pthread_mutex_lock(&lock_atacantes_Brasil);
		// Incrementa a preferencia do atacante
		escalacao_atacantes_Brasil++;
		// Contempla todos os candidatos a vaga de atacante
		if(escalacao_atacantes_Brasil <= (NUM_ATACANTES+1)){
			// Armazena a preferencia do atacante em ser titular (maior sendo 0)
			titular_atacantes_Brasil[id] = escalacao_atacantes_Brasil;
		}
		pthread_mutex_unlock(&lock_atacantes_Brasil);

		// printf("%d\n", escalacao_atacantes_Brasil);
		// printf("%d\n", titular_atacantes_Brasil[id]);

		// Jogador selecionado
		if(escalacao_atacantes_Brasil <= NUM_ATACANTES){
			printf("\tBrasil, como %do atacante: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do atacante for a menor possivel (2), ele fica no banco
		if(titular_atacantes_Brasil[id] == (NUM_ATACANTES+1)){
			sem_wait(&sem_atacantes_Brasil);
		}

		// Se a regiao for o ataque do Brasil
		if(regiao == ATAQUE_BRASIL){

			// Se a posse de bola eh do Brasil, um atacante esta com a bola
			if(posse == BRASIL){

				// Se um chute aconteceu
				pthread_mutex_lock(&lock_regiao);
				regiao = GOL_ALEMANHA;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Brasil, atacante %s com a bola\n", jogador);
				printf("Brasil, atacante %s chutou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

/*************************************************************/
/* Threads dos jogadores da Alemanha */
void * thread_goleiro_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(1) Kahn";
		}
		else{
			jogador = "(12) Lehmann";
		}

		pthread_mutex_lock(&lock_goleiros_Alemanha);
		// Incrementa a preferencia do goleiro
		escalacao_goleiro_Alemanha++;
		// Contempla todos os candidatos a vaga de goleiro
		if(escalacao_goleiro_Alemanha <= (NUM_GOLEIRO+1)){
			// Armazena a preferencia do goleiro em ser titular (maior sendo 0)
			titular_goleiro_Alemanha[id] = escalacao_goleiro_Alemanha;
		}
		pthread_mutex_unlock(&lock_goleiros_Alemanha);

		// printf("%d\n", escalacao_goleiro_Alemanha);
		// printf("%d\n", titular_goleiro_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_goleiro_Alemanha == NUM_GOLEIRO){
			printf("\tAlemanha, como goleiro: %s\n", jogador);
			sleep(2);
		}
		// Se a preferencia do goleiro for a menor possivel (1), ele fica no banco
		if(titular_goleiro_Alemanha[id] == (NUM_GOLEIRO+1)){
			sem_wait(&sem_goleiros_Alemanha);
		}

		// Se a regiao for o gol da Alemanha
		if(regiao == GOL_ALEMANHA){

			// Se a posse de bola eh da Alemanha, o goleiro esta com a bola
			if(posse == ALEMANHA){

				// Goleiro tocou para um zagueiro
				pthread_mutex_lock(&lock_regiao);
				// DEFESA_ALEMANHA = ATAQUE_BRASIL
				regiao = ATAQUE_BRASIL;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Alemanha, goleiro %s com a bola\n", jogador);
				printf("Alemanha, goleiro %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh do Brasil, houve um chute
			else{

				// Se a defesa aconteceu
				if(chance == 0){
					// Posse de bola passa a ser da Alemanha
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Alemanha, goleiro %s defendeu\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}
				// Se houve gol
				else{
					// Alemanha sai jogando
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);
					pthread_mutex_lock(&lock_gol);
					gols_Brasil++;
					pthread_mutex_unlock(&lock_gol);
					printf("GOL DO BRASIL!\n");
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_zagueiro_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(2) Linke";
		}
		else if(id == 1){
			jogador = "(3) Rehmer";
		}
		else if(id == 2){
			jogador = "(21) Meltzfelder";
		}
		else{
			jogador = "(4) Baumann";
		}

		pthread_mutex_lock(&lock_zagueiros_Alemanha);
		// Incrementa a preferencia do zagueiro
		escalacao_zagueiros_Alemanha++;
		// Contempla todos os candidatos a vaga de zagueiro
		if(escalacao_zagueiros_Alemanha <= (NUM_ZAGUEIROS+1)){
			// Armazena a preferencia do zagueiro em ser titular (maior sendo 0)
			titular_zagueiros_Alemanha[id] = escalacao_zagueiros_Alemanha;
		}
		pthread_mutex_unlock(&lock_zagueiros_Alemanha);

		// printf("%d\n", escalacao_zagueiros_Alemanha);
		// printf("%d\n", titular_zagueiros_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_zagueiros_Alemanha <= NUM_ZAGUEIROS){
			printf("\tAlemanha, como %do zagueiro: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do zagueiro for a menor possivel (3), ele fica no banco
		if(titular_zagueiros_Alemanha[id] == (NUM_ZAGUEIROS+1)){
			sem_wait(&sem_zagueiros_Alemanha);
		}

		// Se a regiao for a defesa do Alemanha
		// ATAQUE_BRASIL == DEFESA_ALEMANHA
		if(regiao == ATAQUE_BRASIL){

			// Se a posse de bola eh do Alemanha, um zagueiro esta com a bola
			if(posse == ALEMANHA){

				// Zagueiro tocou para um lateral
				pthread_mutex_lock(&lock_regiao);
				regiao = LADOS;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Alemanha, zagueiro %s com a bola\n", jogador);
				printf("Alemanha, zagueiro %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh do Brasil, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser da Alemanha
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Alemanha, zagueiro %s tomou e tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_lateral_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(22) Frings";
		}
		else if(id == 1){
			jogador = "(17) Bode";
		}
		else{
			jogador = "(6) Ziege";
		}

		pthread_mutex_lock(&lock_laterais_Alemanha);
		// Incrementa a preferencia do lateral
		escalacao_laterais_Alemanha++;
		// Contempla todos os candidatos a vaga de lateral
		if(escalacao_laterais_Alemanha <= (NUM_LATERAIS+1)){
			// Armazena a preferencia do lateral em ser titular (maior sendo 0)
			titular_laterais_Alemanha[id] = escalacao_laterais_Alemanha;
		}
		pthread_mutex_unlock(&lock_laterais_Alemanha);

		// printf("%d\n", escalacao_laterais_Alemanha);
		// printf("%d\n", titular_laterais_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_laterais_Alemanha <= NUM_LATERAIS){
			printf("\tAlemanha, como %do lateral: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do zagueiro for a menor possivel (2), ele fica no banco
		if(titular_laterais_Alemanha[id] == (NUM_LATERAIS+1)){
			sem_wait(&sem_laterais_Alemanha);
		}

		// Se a regiao for os lados do campo
		if(regiao == LADOS){

			// Se a posse de bola eh da Alemanha, um lateral esta com a bola
			if(posse == ALEMANHA){

				// Se um passe aconteceu
				if(chance == 0){
					// Lateral tocou para um volante
					pthread_mutex_lock(&lock_regiao);
					// MEIO_OFENSIVO_BRASIL = MEIA_DEFENSIVO_ALEMANHA
					regiao = MEIO_OFENSIVO_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Alemanha, lateral %s com a bola\n", jogador);
					printf("Alemanha, lateral %s tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}

				// Se um cruzamento aconteceu
				else{
					// Lateral cruzou para um atacante
					pthread_mutex_lock(&lock_regiao);
					// DEFESA_BRASIL = ATAQUE_ALEMANHA
					regiao = DEFESA_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Alemanha, lateral %s com a bola\n", jogador);
					printf("Alemanha, lateral %s cruzou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_volante_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(8) Hamann";
		}
		else if(id == 1){
			jogador = "(16) Jeremies";
		}
		else{
			jogador = "(5) Ramelow";
		}

		pthread_mutex_lock(&lock_volantes_Alemanha);
		// Incrementa a preferencia do volante
		escalacao_volantes_Alemanha++;
		// Contempla todos os candidatos a vaga de volante
		if(escalacao_volantes_Alemanha <= (NUM_VOLANTES+1)){
			// Armazena a preferencia do volante em ser titular (maior sendo 0)
			titular_volantes_Alemanha[id] = escalacao_volantes_Alemanha;
		}
		pthread_mutex_unlock(&lock_volantes_Alemanha);

		// printf("%d\n", escalacao_volantes_Alemanha);
		// printf("%d\n", titular_volantes_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_volantes_Alemanha <= NUM_VOLANTES){
			printf("\tAlemanha, como %do volante: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do volante for a menor possivel (2), ele fica no banco
		if(titular_volantes_Alemanha[id] == (NUM_VOLANTES+1)){
			sem_wait(&sem_volantes_Alemanha);
		}

		// Se a regiao for o meio de campo defensivo da Alemanha
		// MEIO_OFENSIVO_BRASIL = MEIO_DEFENSIVO_ALEMANHA
		if(regiao == MEIO_OFENSIVO_BRASIL){
			// Se a posse de bola eh da Alemanha, um volante esta com a bola
			if(posse == ALEMANHA){

				// Volante tocou para um meia
				pthread_mutex_lock(&lock_regiao);
				// MEIO_DEFENSIVO_BRASIL = MEIO_OFENSIVO_ALEMANHA
				regiao = MEIO_DEFENSIVO_BRASIL;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Alemanha, volante %s com a bola\n", jogador);
				printf("Alemanha, volante %s tocou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}

			// Se a posse de bola eh do Brasil, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser da Alemanha
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("Alemanha, volante %s tomou e tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_meia_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Nome do jogador
		if(id == 0){
			jogador = "(19) Schneider";
		}
		else{
			jogador = "(13) Ballack";
		}

		pthread_mutex_lock(&lock_meias_Alemanha);
		// Incrementa a preferencia do meia
		escalacao_meia_Alemanha++;
		// Contempla todos os candidatos a vaga de meia
		if(escalacao_meia_Alemanha <= (NUM_MEIA+1)){
			// Armazena a preferencia do meia em ser titular (maior sendo 0)
			titular_meia_Alemanha[id] = escalacao_meia_Alemanha;
		}
		pthread_mutex_unlock(&lock_meias_Alemanha);

		// printf("%d\n", escalacao_meia_Alemanha);
		// printf("%d\n", titular_meia_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_meia_Alemanha == NUM_MEIA){
			printf("\tAlemanha, como %do meia: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do meia for a menor possivel (1), ele fica no banco
		if(titular_meia_Alemanha[id] == (NUM_MEIA+1)){
			sem_wait(&sem_meias_Alemanha);
		}

		// Se a regiao for o meio de campo ofensivo da Alemanha
		if(regiao == MEIO_DEFENSIVO_BRASIL){

			// Se a posse de bola eh da Alemanha, um meia esta com a bola
			if(posse == ALEMANHA){

				// Se um passe aconteceu
				if(chance == 0){
					// Meia tocou para um atacante
					pthread_mutex_lock(&lock_regiao);
					// DEFESA_BRASIL = ATAQUE_ALEMANHA
					regiao = DEFESA_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Alemanha, meia %s com a bola\n", jogador);
					printf("Alemanha, meia %s tocou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}

				// Se um chute aconteceu
				else{
					// Meia chutou para o gol
					pthread_mutex_lock(&lock_regiao);
					regiao = GOL_BRASIL;
					pthread_mutex_unlock(&lock_regiao);

					// So um jogador pode possuir a bola por vez
					pthread_mutex_lock(&lock_bola);
					printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
					printf("Alemanha, meia %s com a bola\n", jogador);
					printf("Alemanha, meia %s chutou a bola\n", jogador);
					pthread_mutex_unlock(&lock_bola);
					sleep(4);
				}// if chance
			}// if posse
			// printf("posse %s\n", getPosse(posse));
			// printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_atacante_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;

	while(1){
		// Nome do jogador
		if(id == 0){
			jogador = "(11) Klose";
		}
		else if(id == 1){
			jogador = "(7) Neuville";
		}
		else{
			jogador = "(9) Jancker";
		}

		pthread_mutex_lock(&lock_atacantes_Alemanha);
		// Incrementa a preferencia do atacante
		escalacao_atacantes_Alemanha++;
		// Contempla todos os candidatos a vaga de atacante
		if(escalacao_atacantes_Alemanha <= (NUM_ATACANTES+1)){
			// Armazena a preferencia do atacante em ser titular (maior sendo 0)
			titular_atacantes_Alemanha[id] = escalacao_atacantes_Alemanha;
		}
		pthread_mutex_unlock(&lock_atacantes_Alemanha);

		// printf("%d\n", escalacao_atacantes_Alemanha);
		// printf("%d\n", titular_atacantes_Alemanha[id]);

		// Jogador selecionado
		if(escalacao_atacantes_Alemanha <= NUM_ATACANTES){
			printf("\tAlemanha, como %do atacante: %s\n", (id+1), jogador);
			sleep(2);
		}
		// Se a preferencia do atacante for a menor possivel (2), ele fica no banco
		if(titular_atacantes_Alemanha[id] == (NUM_ATACANTES+1)){
			sem_wait(&sem_atacantes_Alemanha);
		}

		// Se a regiao for o meio de campo ofensivo da Alemanha
		// DEFESA_BRASIL = ATAQUE_ALEMANHA
		if(regiao == DEFESA_BRASIL){

			// Se a posse de bola eh da Alemanha, um atacante esta com a bola
			if(posse == ALEMANHA){

				// Se um chute aconteceu
				pthread_mutex_lock(&lock_regiao);
				regiao = GOL_BRASIL;
				pthread_mutex_unlock(&lock_regiao);

				// So um jogador pode possuir a bola por vez
				pthread_mutex_lock(&lock_bola);
				printf("\nBRASIL %d X %d ALEMANHA\n", gols_Brasil, gols_Alemanha);
				printf("Alemanha, atacante %s com a bola\n", jogador);
				printf("Alemanha, atacante %s chutou a bola\n", jogador);
				pthread_mutex_unlock(&lock_bola);
				sleep(4);
			}// if posse

		}// if regiao
		// printf("posse %s\n", getPosse(posse));
		// printf("regiao %s\n", getRegiao(regiao));
	}// while
}

/*************************************************************/
int main(){
	int indice;
	int *id_goleiro_Brasil, *id_zagueiro_Brasil, *id_lateral_Brasil, *id_volante_Brasil, *id_meia_Brasil, *id_atacante_Brasil;
	int *id_goleiro_Alemanha, *id_zagueiro_Alemanha, *id_lateral_Alemanha, *id_volante_Alemanha, *id_meia_Alemanha, *id_atacante_Alemanha;

	// Seed para geracao de numeros aleatorios
	srand48(time(NULL));

	printf("\n***** SIMULACAO DA FINAL DA COPA DO MUNDO DE 2002 NO JAPAO *****\n");
	printf("\nELENCOS:");
	printf("\n\tBRASIL\n");
	printf("\t- Goleiros: Marcos e Rogerio Ceni;\n");
	printf("\t- Zagueiros: Lucio, Edmilson, Roque Junior e Anderson Polga;\n");
	printf("\t- Laterais: Cafu, Roberto Carlos e Junior;\n");
	printf("\t- Volantes: Gilberto Silva, Kleberson e Vampeta;\n");
	printf("\t- Meias: Ronaldinho e Kaka;\n");
	printf("\t- Atacantes: Rivaldo, Ronaldo e Denilson.\n");

	printf("\n\tALEMANHA\n");
	printf("\t- Goleiros: Kahn e Lehmann;\n");
	printf("\t- Zagueiros: Linke, Rehmer, Meltzfelder e Baumann;\n");
	printf("\t- Laterais: Frings, Bode e Ziege;\n");
	printf("\t- Volantes: Hamann, Jeremies e Ramelow;\n");
	printf("\t- Meias: Schneider e Ballack;\n");
	printf("\t- Atacantes: Klose, Neuville e Jancker.\n");

	printf("\nTITULARES:\n");

	// Inicializando os semaforos da escalacao do Brasil
	sem_init(&sem_goleiros_Brasil, 0, 0);
	sem_init(&sem_zagueiros_Brasil, 0, 0);
	sem_init(&sem_laterais_Brasil, 0, 0);
	sem_init(&sem_volantes_Brasil, 0, 0);
	sem_init(&sem_meias_Brasil, 0, 0);
	sem_init(&sem_atacantes_Brasil, 0, 0);

	// Inicializando os semaforos da escalacao da Alemanha
	sem_init(&sem_goleiros_Alemanha, 0, 0);
	sem_init(&sem_zagueiros_Alemanha, 0, 0);
	sem_init(&sem_laterais_Alemanha, 0, 0);
	sem_init(&sem_volantes_Alemanha, 0, 0);
	sem_init(&sem_meias_Alemanha, 0, 0);
	sem_init(&sem_atacantes_Alemanha, 0, 0);

	/* Criacao das threads de ambos os times */
	for(indice = 0; indice <= NUM_GOLEIRO; indice++){
		// Goleiros Brasileiros
		id_goleiro_Brasil = (int *)malloc(sizeof(int));
		*id_goleiro_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_goleiro_Brasil, (void *)(id_goleiro_Brasil));
		// Goleiros Alemaes
		id_goleiro_Alemanha = (int *)malloc(sizeof(int));
		*id_goleiro_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_goleiro_Alemanha, (void *)(id_goleiro_Alemanha));
	}

	for(indice = 0; indice <= NUM_ZAGUEIROS; indice++){
		// Zagueiros Brasileiros
		id_zagueiro_Brasil = (int *)malloc(sizeof(int));
		*id_zagueiro_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_zagueiro_Brasil, (void *)(id_zagueiro_Brasil));
		// Zagueiros Alemaes
		id_zagueiro_Alemanha = (int *)malloc(sizeof(int));
		*id_zagueiro_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_zagueiro_Alemanha, (void *)(id_zagueiro_Alemanha));
	}

	for(indice = 0; indice <= NUM_LATERAIS; indice++){
		// Laterais Brasileiros
		id_lateral_Brasil = (int *)malloc(sizeof(int));
		*id_lateral_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_lateral_Brasil, (void *)(id_lateral_Brasil));
		// Laterais Alemaes
		id_lateral_Alemanha = (int *)malloc(sizeof(int));
		*id_lateral_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_lateral_Alemanha, (void *)(id_lateral_Alemanha));
	}

	for(indice = 0; indice <= NUM_VOLANTES; indice++){
		// Volantes Brasileiros
		id_volante_Brasil = (int *)malloc(sizeof(int));
		*id_volante_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_volante_Brasil, (void *)(id_volante_Brasil));
		// Volantes Alemaes
		id_volante_Alemanha = (int *)malloc(sizeof(int));
		*id_volante_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_volante_Alemanha, (void *)(id_volante_Alemanha));
	}

	for(indice = 0; indice <= NUM_MEIA; indice++){
		// Meias Brasileiros
		id_meia_Brasil = (int *)malloc(sizeof(int));
		*id_meia_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_meia_Brasil, (void *)(id_meia_Brasil));
		// Meias Alemaes
		id_meia_Alemanha = (int *)malloc(sizeof(int));
		*id_meia_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_meia_Alemanha, (void *)(id_meia_Alemanha));
	}

	for(indice = 0; indice <= NUM_ATACANTES; indice++){
		// Atacantes Brasileiros
		id_atacante_Brasil = (int *)malloc(sizeof(int));
		*id_atacante_Brasil = indice;
		pthread_create(&Brasil[indice], NULL, thread_atacante_Brasil, (void *)(id_atacante_Brasil));
		// Atacantes Alemaes
		id_atacante_Alemanha = (int *)malloc(sizeof(int));
		*id_atacante_Alemanha = indice;
		pthread_create(&Alemanha[indice], NULL, thread_atacante_Alemanha, (void *)(id_atacante_Alemanha));
	}

	// Inicio do jogo
	pthread_join(Brasil[0], NULL);
	pthread_join(Alemanha[0], NULL);

	return 0;
}
