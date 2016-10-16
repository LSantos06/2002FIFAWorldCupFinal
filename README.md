# 2002FIFAWorldCupFinal

## Trabalho 1 de Programação Concorrente 

O trabalho consiste no desenvolvimento de um algoritmo para tratar o problema proposto de comunicação entre processos através de uma memória compartilhada.

## Problema Proposto

O problema consiste na simulação de uma partida de futebol, mais especificamente a Final da Copa do Mundo FIFA 2002, entre Brasil e Alemanha.

## Formalizando o problema proposto

Em um jogo de futebol, cada time possui 11 jogadores titulares, e se baseando na formação **4-2-3-1** (utilizada pela Seleção Brasileira em 2002), onde cada time titular eh composto por:
  - 1 Goleiro;
  - 3 Zagueiros;
  - 2 Laterais;
  - 2 Volantes;
  - 1 Meia;
  - 2 Atacantes.

Para fins de concorência, mais um jogador foi adicionado em cada posição, resultando no total de 17 jogadores, sendo eles definidos abaixo para cada time:

#### Escalação do Brasil (17 jogadores):
  - Goleiros: Marcos e Rogerio Ceni;
  - Zagueiros: Lucio, Edmilson, Roque Junior e Anderson Polga;
  - Laterais: Cafu, Roberto Carlos e Junior;
  - Volantes: Gilberto Silva, Kleberson e Vampeta;
  - Meias: Ronaldinho e Kaka;
  - Atacantes: Rivaldo, Ronaldo e Denilson.

#### Escalação da Alemanha (17 jogadores):
  - Goleiros: Kahn e Lehmann;
  - Zagueiros: Linke, Rehmer, Meltzfelder e Baumann;
  - Laterais: Frings, Bode e Ziege;
  - Volantes: Hamann, Jeremies e Ramelow;
  - Meias: Schneider e Ballack;
  - Atacantes: Klose, Neuville e Jancker.

A **primeira região crítica** é a designição do time titular, ou seja, os 17 processos concorrem entre si para ocupar uma vaga (em suas respectivas posições) no time que vai jogar a Final. 

A partida ocorre infinitamente, e os jogadores de cada time foram divididos em threads de acordo com suas posições, sendo elas:
  - Goleiro;
  - Zagueiro;
  - Lateral;
  - Volante;
  - Meia;
  - Atacante.

Para fins de fidedignidade do jogo, cada posição possui um conjunto de ações que podem ser realizadas e um pedaço do campo, sendo elas:
  - Goleiro (Gol de defesa)
    - Defender uma bola chutada em direção ao seu gol; 
    - Tocar a bola para um zagueiro de seu time.
  - Zagueiro (Defesa)
    - Tomar a bola de um atacante adversário e toca-la para um zagueiro de seu time; 
    - Tocar a bola para um lateral de seu time.
  - Lateral (Lados)
    - Cruzar a bola para um atacante de seu time; 
    - Tocar a bola para um volante de seu time;
  - Volante (Meio Defensivo)
    - Tomar a bola de um meia adversário;
    - Tocar a bola para um meia de seu time;
  - Meia (Meio Ofensivo)
    - Chutar a bola em direção ao gol adversário;
    - Tocar a bola para um atacante de seu time;
  - Atacante (Ataque)
    - Chutar a bola em direção ao gol adversário.

O time nunca passa a bola para trás, sempre passa para frente buscando o gol. 
 
A **segunda região crítica** implementada é a bola, pois entre os 22 processos ativos, somente 1 pode acessar a bola.
