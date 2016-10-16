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
 * - A partida dura 90 minutos, se houver empate ocorre a decisao pelos penaltis;
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
 *		- Goleiro: grande area de defesa;
 *		- Zagueiro: campo de defesa;
 *		- Lateral: lados de todo o campo;
 *		- Volante: meio do campo;
 *		- Meia:	meio do campo;
 *		- Atacante: campo de ataque.
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
#define NUM_MEIAS 1
#define NUM_ATACANTES 2

#define DURACAO_MIN 90

// Time do Brasil
pthread_t Brasil[NUM_ESCALACAO-1];

// Time da Alemanha
pthread_t Alemanha[NUM_ESCALACAO-1];

// Escalacao do time do Brasil
sem_t escalacao_goleiro_Brasil;
sem_t escalacao_zagueiros_Brasil;
sem_t escalacao_laterais_Brasil;
sem_t escalacao_volantes_Brasil;
sem_t escalacao_meias_Brasil;
sem_t escalacao_atacantes_Brasil;

// Escalacao do time da Alemanha
sem_t escalacao_goleiro_Alemanha;
sem_t escalacao_zagueiros_Alemanha;
sem_t escalacao_laterais_Alemanha;
sem_t escalacao_volantes_Alemanha;
sem_t escalacao_meias_Alemanha;
sem_t escalacao_atacantes_Alemanha;

// Posse de bola
pthread_mutex_t lock_posse = PTHREAD_MUTEX_INITIALIZER;

// Regiao do campo
pthread_mutex_t lock_regiao = PTHREAD_MUTEX_INITIALIZER;

// Time que possui a bola, sendo 0 = Brasil e 1 = Alemanha
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

/* Threads dos jogadores do Brasil */
void * thread_goleiro_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_goleiro_Brasil);

		// Nome do jogador
		if(id == 0){
			jogador = "(1) Marcos";
		}
		else{
			jogador = "(22) Rogerio Ceni";
		}

		printf("\tBrasil, como goleiro: %s\n", jogador);
		sleep(2);

		// Se a regiao for o gol do Brasil
		if(regiao == GOL_BRASIL){

			// Se a posse de bola eh do Brasil, o goleiro esta com a bola
			if(posse == BRASIL){
				printf("Brasil, goleiro %s com a bola\n", jogador);

				// Goleiro tocou para um zagueiro
				pthread_mutex_lock(&lock_regiao);
				regiao = DEFESA_BRASIL;
				pthread_mutex_unlock(&lock_regiao);
				printf("Brasil, goleiro %s tocou a bola\n", jogador);
			}

			// Se a posse de bola eh da Alemanha, houve um chute
			else{

				// Se a defesa aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);
					printf("Brasil, goleiro %s defendeu\n", jogador);
				}
				// Se houve gol
				else{
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);
					printf("GOL DA ALEMANHA!\n");
				}// if chance
			}// if posse
			printf("posse %s\n", getPosse(posse));
			printf("regiao %s\n", getRegiao(regiao));
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

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_zagueiros_Brasil);

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

		printf("\tBrasil, como %do zagueiro: %s\n", (id+1), jogador);
		sleep(2);

		// Se a regiao for a defesa do Brasil
		if(regiao == DEFESA_BRASIL){

			// Se a posse de bola eh do Brasil, um zagueiro esta com a bola
			if(posse == BRASIL){
				printf("Brasil, zagueiro %s com a bola\n", jogador);

				// Zagueiro tocou para um lateral
				pthread_mutex_lock(&lock_regiao);
				regiao = LADOS;
				pthread_mutex_unlock(&lock_regiao);
				printf("Brasil, zagueiro %s tocou a bola\n", jogador);
			}

			// Se a posse de bola eh da Alemanha, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);
					printf("Brasil, zagueiro %s tomou a bola\n", jogador);
				}// if chance
			}// if posse
			printf("posse %s\n", getPosse(posse));
			printf("regiao %s\n", getRegiao(regiao));
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

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_laterais_Brasil);

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

		printf("\tBrasil, como %do lateral: %s\n", (id+1), jogador);
		sleep(2);

		// Se a regiao for os lados do campo
		if(regiao == LADOS){

			// Se a posse de bola eh do Brasil, um lateral esta com a bola
			if(posse == BRASIL){
				printf("Brasil, lateral %s com a bola\n", jogador);

				// Se um passe aconteceu
				if(chance == 0){
					// Lateral tocou para um volante
					pthread_mutex_lock(&lock_regiao);
					regiao = MEIO_DEFENSIVO_BRASIL;
					pthread_mutex_unlock(&lock_regiao);
					printf("Brasil, lateral %s tocou a bola\n", jogador);
				}

				// Se um cruzamento aconteceu
				else{
					// Lateral cruzou para um atacante
					pthread_mutex_lock(&lock_regiao);
					regiao = ATAQUE_BRASIL;
					pthread_mutex_unlock(&lock_regiao);
					printf("Brasil, lateral %s cruzou a bola\n", jogador);
				}// if chance
			}// if posse
			printf("posse %s\n", getPosse(posse));
			printf("regiao %s\n", getRegiao(regiao));
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

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_volantes_Brasil);

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

		printf("\tBrasil, como %do volante: %s\n", (id+1), jogador);
		sleep(2);

		// Se a regiao for o meio de campo defensivo do Brasil
		if(regiao == MEIO_DEFENSIVO_BRASIL){
			// Se a posse de bola eh do Brasil, um volante esta com a bola
			if(posse == BRASIL){
				printf("Brasil, volante %s com a bola\n", jogador);

				// Volante tocou para um meia
				pthread_mutex_lock(&lock_regiao);
				regiao = MEIO_OFENSIVO_BRASIL;
				pthread_mutex_unlock(&lock_regiao);
				printf("Brasil, volante %s tocou a bola\n", jogador);
			}

			// Se a posse de bola eh da Alemanha, um desarme pode acontecer
			else{

				// Se o desarme aconteceu
				if(chance == 0){
					// Posse de bola passa a ser do Brasil
					pthread_mutex_lock(&lock_posse);
					posse = BRASIL;
					pthread_mutex_unlock(&lock_posse);
					printf("Brasil, volante %s tomou a bola\n", jogador);
				}// if chance
			}// if posse
			printf("posse %s\n", getPosse(posse));
			printf("regiao %s\n", getRegiao(regiao));
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

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_meias_Brasil);

		// Nome do jogador
		if(id == 0){
			jogador = "(11) Ronaldinho";
		}
		else{
			jogador = "(23) Kaka";
		}

		printf("\tBrasil, como %do meia: %s\n", (id+1), jogador);
		sleep(2);

		//printf("\nAPITA O ARBITRO, BOLA ROLANDO EM YOKOHAMA!\n\n");

		// Se a regiao for o meio de campo ofensivo do Brasil
		if(regiao == MEIO_OFENSIVO_BRASIL){

			// Se a posse de bola eh do Brasil, um meia esta com a bola
			if(posse == BRASIL){
				printf("Brasil, meia %s com a bola\n", jogador);

				// Se um passe aconteceu
				if(chance == 0){
					// Meia tocou para um atacante
					pthread_mutex_lock(&lock_regiao);
					regiao = ATAQUE_BRASIL;
					pthread_mutex_unlock(&lock_regiao);
					printf("Brasil, meia %s tocou a bola\n", jogador);
				}

				// Se um chute aconteceu
				else{
					// Meia chutou para o gol
					pthread_mutex_lock(&lock_regiao);
					regiao = GOL_ALEMANHA;
					pthread_mutex_unlock(&lock_regiao);
					printf("Brasil, meia %s chutou a bola\n", jogador);
				}// if chance
			}// if posse
			printf("posse %s\n", getPosse(posse));
			printf("regiao %s\n", getRegiao(regiao));
		}// if regiao
	}// while
}

void * thread_atacante_Brasil(void *argumento){
	int id = *((int *)argumento);
	char *jogador;

	while(1){
		// contador++; if contador == NUM_GOLEIRO wait

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_atacantes_Brasil);

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

		printf("\tBrasil, como %do atacante: %s\n", (id+1), jogador);
		sleep(2);

		// Se a regiao for o ataque do Brasil
		if(regiao == ATAQUE_BRASIL){

			// Se a posse de bola eh do Brasil, um atacante esta com a bola
			if(posse == BRASIL){
				printf("Brasil, atacante %s com a bola\n", jogador);

				// Se um chute aconteceu
				pthread_mutex_lock(&lock_regiao);
				regiao = GOL_ALEMANHA;
				pthread_mutex_unlock(&lock_regiao);
				// Atacante chutou
				printf("Brasil, atacante %s chutou a bola\n", jogador);
			}// if posse

		}// if regiao
		printf("posse %s\n", getPosse(posse));
		printf("regiao %s\n", getRegiao(regiao));
	}// while
}

/* Threads dos jogadores da Alemanha */
void * thread_goleiro_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_goleiro_Alemanha);

		// Nome do jogador
		if(id == 0){
			jogador = "(1) Kahn";
		}
		else{
			jogador = "(12) Lehmann";
		}

		// if sem > 0
		printf("\tAlemanha, como goleiro: %s\n", jogador);
		sleep(2);

		// Se a regiao for o gol da Alemanha
		if(regiao == GOL_ALEMANHA){

			// Se a posse de bola eh da Alemanha, o goleiro esta com a bola
			if(posse == ALEMANHA){
				printf("Alemanha, goleiro %s com a bola\n", jogador);

				// Goleiro tocou para um zagueiro
				pthread_mutex_lock(&lock_regiao);
				// DEFESA_ALEMANHA = ATAQUE_BRASIL
				regiao = ATAQUE_BRASIL;
				pthread_mutex_unlock(&lock_regiao);
				printf("Alemanha, goleiro %s tocou a bola\n", jogador);
			}

			// Se a posse de bola eh do Brasil, houve um chute
			else{

				// Se a defesa aconteceu
				if(chance == 0){
					// Posse de bola passa a ser da Alemanha
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);
					printf("Alemanha, goleiro %s defendeu\n", jogador);
				}
				// Se houve gol
				else{
					// Alemanha sai jogando
					pthread_mutex_lock(&lock_posse);
					posse = ALEMANHA;
					pthread_mutex_unlock(&lock_posse);
					printf("GOL DO BRASIL!\n");
				}// if chance
			}// if posse

		}// if regiao
		printf("posse %s\n", getPosse(posse));
		printf("regiao %s\n", getRegiao(regiao));
	}// while
}

void * thread_zagueiro_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_zagueiros_Alemanha);

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

		printf("\tAlemanha, como %do zagueiro: %s\n", (id+1), jogador);
		sleep(2);

		// // Se a regiao for a defesa do Alemanha
		// // ATAQUE_BRASIL == DEFESA_ALEMANHA
		// if(regiao == ATAQUE_BRASIL){
		//
		// 	// Se a posse de bola eh do Alemanha, um zagueiro esta com a bola
		// 	if(posse == ALEMANHA){
		// 		printf("Alemanha, zagueiro %s com a bola\n", jogador);
		//
		// 		// Zagueiro tocou para um lateral
		// 		pthread_mutex_lock(&lock_regiao);
		// 		regiao = LADOS;
		// 		pthread_mutex_unlock(&lock_regiao);
		// 		printf("Alemanha, zagueiro %s tocou a bola\n", jogador);
		// 	}
		//
		// 	// Se a posse de bola eh do Brasil, um desarme pode acontecer
		// 	else{
		//
		// 		// Se o desarme aconteceu
		// 		if(chance == 0){
		// 			// Posse de bola passa a ser da Alemanha
		// 			pthread_mutex_lock(&lock_posse);
		// 			posse = ALEMANHA;
		// 			pthread_mutex_unlock(&lock_posse);
		// 			printf("Alemanha, zagueiro %s tomou a bola\n", jogador);
		// 		}// if chance
		// 	}// if posse
		// 	printf("posse %s\n", getPosse(posse));
		// 	printf("regiao %s\n", getRegiao(regiao));
		// }// if regiao
	}// while
}

void * thread_lateral_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_laterais_Alemanha);

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

		printf("\tAlemanha, como %do lateral: %s\n", (id+1), jogador);
		sleep(2);

		// // Se a regiao for os lados do campo
		// if(regiao == LADOS){
		//
		// 	// Se a posse de bola eh da Alemanha, um lateral esta com a bola
		// 	if(posse == ALEMANHA){
		// 		printf("Alemanha, lateral %s com a bola\n", jogador);
		//
		// 		// Se um passe aconteceu
		// 		if(chance == 0){
		// 			// Lateral tocou para um volante
		// 			pthread_mutex_lock(&lock_regiao);
		// 			// MEIO_OFENSIVO_BRASIL = MEIA_DEFENSIVO_ALEMANHA
		// 			regiao = MEIO_OFENSIVO_BRASIL;
		// 			pthread_mutex_unlock(&lock_regiao);
		// 			printf("Alemanha, lateral %s tocou a bola\n", jogador);
		// 		}
		//
		// 		// Se um cruzamento aconteceu
		// 		else{
		// 			// Lateral cruzou para um atacante
		// 			pthread_mutex_lock(&lock_regiao);
		// 			// DEFESA_BRASIL = ATAQUE_ALEMANHA
		// 			regiao = DEFESA_BRASIL;
		// 			pthread_mutex_unlock(&lock_regiao);
		// 			printf("Alemanha, lateral %s cruzou a bola\n", jogador);
		// 		}// if chance
		// 	}// if posse
		// 	printf("posse %s\n", getPosse(posse));
		// 	printf("regiao %s\n", getRegiao(regiao));
		// }// if regiao
	}// while
}

void * thread_volante_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_volantes_Alemanha);

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

		printf("\tAlemanha, como %do volante: %s\n", (id+1), jogador);
		sleep(2);

		// // Se a regiao for o meio de campo defensivo da Alemanha
		// // MEIO_OFENSIVO_BRASIL = MEIO_DEFENSIVO_ALEMANHA
		// if(regiao == MEIO_OFENSIVO_BRASIL){
		// 	// Se a posse de bola eh da Alemanha, um volante esta com a bola
		// 	if(posse == ALEMANHA){
		// 		printf("Alemanha, volante %s com a bola\n", jogador);
		//
		// 		// Volante tocou para um meia
		// 		pthread_mutex_lock(&lock_regiao);
		// 		// MEIO_DEFENSIVO_BRASIL = MEIO_OFENSIVO_ALEMANHA
		// 		regiao = MEIO_DEFENSIVO_BRASIL;
		// 		pthread_mutex_unlock(&lock_regiao);
		// 		printf("Alemanha, volante %s tocou a bola\n", jogador);
		// 	}
		//
		// 	// Se a posse de bola eh do Brasil, um desarme pode acontecer
		// 	else{
		//
		// 		// Se o desarme aconteceu
		// 		if(chance == 0){
		// 			// Posse de bola passa a ser da Alemanha
		// 			pthread_mutex_lock(&lock_posse);
		// 			posse = ALEMANHA;
		// 			pthread_mutex_unlock(&lock_posse);
		// 			printf("Alemanha, volante %s tomou a bola\n", jogador);
		// 		}// if chance
		// 	}// if posse
		// 	printf("posse %s\n", getPosse(posse));
		// 	printf("regiao %s\n", getRegiao(regiao));
		// }// if regiao
	}// while
}

void * thread_meia_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;
	int chance;

	while(1){
		// 50% de chance de executar a jogada quando o outro time esta com a bola
		chance = rand()%2;

		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_meias_Alemanha);

		// Nome do jogador
		if(id == 0){
			jogador = "(19) Schneider";
		}
		else{
			jogador = "(13) Ballack";
		}

		printf("\tAlemanha, como %do meia: %s\n", (id+1), jogador);
		sleep(2);

		// // Se a regiao for o meio de campo ofensivo da Alemanha
		// if(regiao == MEIO_DEFENSIVO_BRASIL){
		//
		// 	// Se a posse de bola eh da Alemanha, um meia esta com a bola
		// 	if(posse == ALEMANHA){
		// 		printf("Alemanha, meia %s com a bola\n", jogador);
		//
		// 		// Se um passe aconteceu
		// 		if(chance == 0){
		// 			// Meia tocou para um atacante
		// 			pthread_mutex_lock(&lock_regiao);
		// 			// DEFESA_BRASIL = ATAQUE_ALEMANHA
		// 			regiao = DEFESA_BRASIL;
		// 			pthread_mutex_unlock(&lock_regiao);
		// 			printf("Alemanha, meia %s tocou a bola\n", jogador);
		// 		}
		//
		// 		// Se um chute aconteceu
		// 		else{
		// 			// Meia chutou para o gol
		// 			pthread_mutex_lock(&lock_regiao);
		// 			regiao = GOL_BRASIL;
		// 			pthread_mutex_unlock(&lock_regiao);
		// 			printf("Alemanha, meia %s chutou a bola\n", jogador);
		// 		}// if chance
		// 	}// if posse
		// 	printf("posse %s\n", getPosse(posse));
		// 	printf("regiao %s\n", getRegiao(regiao));
		// }// if regiao
	}// while
}

void * thread_atacante_Alemanha(void *argumento){
	int id = *((int *)argumento);
	char *jogador;

	while(1){
		// Pega uma posicao vaga no time que vai jogar
		sem_wait(&escalacao_atacantes_Alemanha);

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

		printf("\tAlemanha, como %do atacante: %s\n", (id+1), jogador);
		sleep(2);

		// // Se a regiao for o meio de campo ofensivo da Alemanha
		// // DEFESA_BRASIL = ATAQUE_ALEMANHA
		// if(regiao == DEFESA_BRASIL){
		//
		// 	// Se a posse de bola eh da Alemanha, um atacante esta com a bola
		// 	if(posse == ALEMANHA){
		// 		printf("Alemanha, atacante %s com a bola\n", jogador);
		//
		// 		// Se um chute aconteceu
		// 		pthread_mutex_lock(&lock_regiao);
		// 		regiao = GOL_BRASIL;
		// 		pthread_mutex_unlock(&lock_regiao);
		// 		// Atacante chutou para o gol
		// 		printf("Alemanha, atacante %s chutou a bola\n", jogador);
		// 	}// if posse
		//
		// }// if regiao
		// printf("posse %s\n", getPosse(posse));
		// printf("regiao %s\n", getRegiao(regiao));
	}// while
}


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
	sem_init(&escalacao_goleiro_Brasil, 0, NUM_GOLEIRO);
	sem_init(&escalacao_zagueiros_Brasil, 0, NUM_ZAGUEIROS);
	sem_init(&escalacao_laterais_Brasil, 0, NUM_LATERAIS);
	sem_init(&escalacao_volantes_Brasil, 0, NUM_VOLANTES);
	sem_init(&escalacao_meias_Brasil, 0, NUM_MEIAS);
	sem_init(&escalacao_atacantes_Brasil, 0, NUM_ATACANTES);

	// Inicializando os semaforos da escalacao da Alemanha
	sem_init(&escalacao_goleiro_Alemanha, 0, NUM_GOLEIRO);
	sem_init(&escalacao_zagueiros_Alemanha, 0, NUM_ZAGUEIROS);
	sem_init(&escalacao_laterais_Alemanha, 0, NUM_LATERAIS);
	sem_init(&escalacao_volantes_Alemanha, 0, NUM_VOLANTES);
	sem_init(&escalacao_meias_Alemanha, 0, NUM_MEIAS);
	sem_init(&escalacao_atacantes_Alemanha, 0, NUM_ATACANTES);

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

	for(indice = 0; indice <= NUM_MEIAS; indice++){
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
