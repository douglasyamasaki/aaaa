#include <stdio.h>
#include <stdlib.h>
#define LINES 30
#define COLUMS 40
typedef struct personagem {
	int i;
	int j;
	int vivo;
	int armadilhas;

}Player;
typedef struct matriz {
	char mat[LINES][COLUMS];
}Matriz;
typedef struct monstro {
	int i;
	int j;
	int type;
	int vivo;
	int vidas;
}Monstro;

typedef struct bomba {
	int i;
	int j;
	int ativa;
}Bomba;

typedef struct node {
	int i; // coords x e y da imagem , como contadores
	int j;
	struct node* next;
}Queue;

Queue* create_queue() {
	return NULL;
}

void free_queue(Queue *q) {
	while (q != NULL) {
		Queue *aux = q->next; /*guarda ref. p/ prox.*/
		free(q); /*libera a memÃ³ria.*/
		q = aux; /*faz lista apontar p/ o prox. elem.*/
	}
}

Queue* enqueue(Queue *q, int i, int j) {
	/*Criando novo elemento: */
	Queue *n = (Queue *)malloc(sizeof(Queue));
	n->i = i;
	n->j = j;
	n->next = q;
	return n;
}

Queue* dequeue(Queue *q, int* i, int* j) {
	if (!Empty(q)) {
		Queue *aux = q->next; /*Guarda endereÃ§o do prÃ³ximo elemento.*/
		*i = q->i; // associa os valores que foram retirados a variaveis passada por referencia , pois precisamos destes valores
		*j = q->j;
		free(q); /*Removendo nodo da lista.*/
		return aux; /*Nova cabeÃ§a da lista!*/
	}
	else {
		return NULL; /*RemoÃ§Ã£o em uma lista vazia.*/
	}
}



int Empty(Queue *q) {
	return (q == NULL);
}
void colocaBomba(Bomba** bombas, int i, int j) {
	for (int k = 0; k < 3; k++) {
		if (bombas[k]->ativa == 0) {
			bombas[k]->i = i;
			bombas[k]->j = j;
			bombas[k]->ativa = 1;
			return;
		}
	}
}
void moverMonstro(Monstro* monster, int direcao) {
	switch (direcao) {
	case 1:
		monster->i--;
		break;
	case 2:
		monster->i++;
		break;
	case 3:
		monster->j--;
		break;
	case 4:
		monster->j++;
		break;
	case 0:
		break;
	}
}
Monstro** leituraArquivo(char maze[LINES][COLUMS], Player* player, int* xsaida, int* ysaida) {

	int i, j, k = 0, aux;

	Monstro ** monstros = (Monstro**)malloc(sizeof(Monstro*) * 5);
	for (i = 0; i<5; i++)
		monstros[i] = (Monstro*)malloc(sizeof(Monstro));

	FILE *arquivo = fopen("maze.txt", "r");
	if (arquivo == NULL) {
		printf("Arquivo nao encontrado\n");
	}

	while (k<7)
	{
		fscanf(arquivo, "%d %d", &i, &j);
		if (k == 0) {
			player->i = i;
			player->j = j;
			player->armadilhas = 3;
			player->vivo = 1;
			k++;
			continue;
		}
		if (k == 1) {
			monstros[0]->i = i;
			monstros[0]->j = j;
			monstros[0]->type = 1;
			monstros[0]->vivo = 1;
			monstros[0]->vidas = 3;
			k++;
			continue;
		}
		if (k == 6) {
			*xsaida = i;
			*ysaida = j;
			k++;
			break;
		}
		monstros[k - 1]->i = i;
		monstros[k - 1]->j = j;
		monstros[k - 1]->type = 0;
		monstros[k - 1]->vivo = 1;
		monstros[k - 1]->vidas = 1;
		k++;
	}

	while (fscanf(arquivo, "%d %d %d", &i, &j, &aux) != EOF) {
		if (aux == 0)
			maze[i][j] = ' ';
		else maze[i][j] = '*';
	}
	maze[*xsaida][*ysaida] = 's';
	fclose(arquivo);
	return monstros;
}

void realizaMovimentacaoPl(Player* player, int direcao) {
	switch (direcao) {
	case 1:
		player->i--;
		break;
	case 2:
		player->i++;
		break;
	case 3:
		player->j--;
		break;
	case 4:
		player->j++;
		break;
	}
}

Bomba** criaBombas(void) {
	Bomba** novo = (Bomba**)malloc(sizeof(Bomba*) * 3);
	for (int i = 0; i < 3; i++) {
		novo[i] = malloc(sizeof(Bomba));
		novo[i]->ativa = 0;
		novo[i]->i = -1;
		novo[i]->j = -1;
	}
	return novo;
}
void checaExplosao(Monstro** monstros, Bomba** bombas) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			if (monstros[i]->i == bombas[j]->i && monstros[i]->j == bombas[j]->j && monstros[i]->vivo == 1 && bombas[j]->ativa == 1) {
				monstros[i]->vidas--;
				if (monstros[i]->vidas == 0) {
					monstros[i]->vivo = 0;
				}
				bombas[j]->ativa = 0;
			}
		}
	}
}
void checaColisao(Player* pl, Monstro** monsters) {
	for (int i = 0; i < 5; i++) {
		if (monsters[i]->i == pl->i && monsters[i]->j == pl->j)
			pl->vivo = 0;
	}
}
int componenteConexo(Monstro* monster, Player* personagem, int direcao, Matriz ma) {
	printf("\n");
	Queue* fila = create_queue();
	int i, j;
	i = monster->i;
	j = monster->j;
	switch (direcao) {
	case 1:
		if (ma.mat[i - 1][j] != ' ')
			return 0;
		break;
	case 2:
		if (ma.mat[i + 1][j] != ' ')
			return 0;
		break;
	case 3:
		if (ma.mat[i][j - 1] != ' ')
			return 0;
		break;
	case 4:
		if (ma.mat[i][j + 1] != ' ')
			return 0;
		break;

	}
	ma.mat[i-1][j] = 'z';
	ma.mat[i+1][j] = 'z';
	ma.mat[i][j-1] = 'z';
	ma.mat[i][j+1] = 'z';
	ma.mat[i][j] = 'z';
	int iaux, jaux, dist = 0;
	switch (direcao) {
	case 1:
		ma.mat[i - 1][j] = ' ';
		iaux = i - 1;
		jaux = j;
		break;
	case 2:
		ma.mat[i + 1][j] = ' ';
		iaux = i + 1;
		jaux = j;
		break;
	case 3:
		ma.mat[i][j - 1] = ' ';
		iaux = i;
		jaux = j - 1;
		break;
	case 4:
		ma.mat[i][j + 1] = ' ';
		iaux = i;
		jaux = j + 1;
		break;
	}
	fila = enqueue(fila, iaux, jaux);
	while (!Empty(fila)) {
		fila = dequeue(fila, &iaux, &jaux);
		dist++;
		printf("%d", dist);
		if (iaux == personagem->i && jaux == personagem->j && dist <= 4) {
			return direcao;
		}
		else if (dist >= 10)
			return 0;
		if (ma.mat[iaux - 1][jaux] == ' ') {
			ma.mat[iaux - 1][jaux] = 'z';
			fila = enqueue(fila, iaux - 1, jaux);
		}
		if (ma.mat[iaux + 1][jaux] == ' ') {
			ma.mat[iaux + 1][jaux] = 'z';
			fila = enqueue(fila, iaux + 1, jaux);
		}
		if (ma.mat[iaux][jaux - 1] == ' ') {
			ma.mat[iaux][jaux - 1] = 'z';
			fila = enqueue(fila, iaux, jaux - 1);
		}
		if (ma.mat[iaux][jaux + 1] == ' ') {
			ma.mat[iaux][jaux + 1] = 'z';
			fila = enqueue(fila, iaux, jaux + 1);
		}

	}
}



int main(int argc, char** argv) {
	char maze[LINES][COLUMS];
	int gameloop = 1, renderizar = 1, processar = 0, colocabomba = 0, moveplayer, isaida, jsaida, aux, flag = 0, direcaoMovimento = 0,auximovimento;
	char player = 88, monstroa = 65, monstrob = 66, armadilha = 162, saida = 219, caminho = 32, parede = 177, var;
	Player* pl = (Player*)malloc(sizeof(Player));
	Monstro** monsters = leituraArquivo(maze, pl, &isaida, &jsaida);
	Bomba** vetordeBombas = criaBombas();
	Matriz base;
	Matriz auxi;
	for (int i = 0; i < LINES; i++) {
		for (int j = 0; j < COLUMS; j++) {
			base.mat[i][j] = maze[i][j];
		}
	}
	auxi = base;
	while (gameloop) {
		if (renderizar == 1) {
			printf("\n");
			for (int i = 0; i < LINES; i++) {
				for (int j = 0; j < COLUMS; j++) {
					for (int k = 0; k < 5; k++) {
						if (monsters[k]->i == i && monsters[k]->j == j && monsters[k]->vivo) {
							flag = 1;
							if (monsters[k]->type == 1 && monsters[k]->vivo == 1)
								printf("%c", monstroa);
							else if (monsters[k]->vivo == 1)
								printf("%c", monstrob);
						}
						if (flag == 1)
							break;
					}
					if (flag == 1) {
						flag = 0;
						continue;
					}
					for (int k = 0; k < 3; k++) {
						if (i == vetordeBombas[k]->i && j == vetordeBombas[k]->j && vetordeBombas[k]->ativa) {
							flag = 1;
							printf("%c", armadilha);
							break;
						}
					}
					if (flag == 1) {
						flag = 0;
						continue;
					}

					if (i == pl->i && j == pl->j)
						printf("%c", player);
					else if (maze[i][j] == 's')
						printf("%c", saida);
					else if (maze[i][j] == '*')
						printf("%c", parede);
					else if (maze[i][j] == ' ')
						printf("%c", caminho);
				}
				printf("\n");
			}
			renderizar = 0;
			for (int i = 0; i < 5; i++) {
				printf("%d %d ", monsters[i]->i, monsters[i]->j);
			}
			printf("\n");
		}
		if (renderizar == 0 && processar == 0) {
			scanf("%c", &var);
			switch (var) {
			case 'w':
				if (maze[pl->i - 1][pl->j] == ' ') {
					moveplayer = 1;
					processar = 1;
				}
				break;
			case 's':
				if (maze[pl->i + 1][pl->j] == ' ') {
					moveplayer = 2;
					processar = 1;
				}
				break;
			case 'a':
				if (maze[pl->i][pl->j - 1] == ' ') {
					moveplayer = 3;
					processar = 1;
				}
				break;
			case 'd':
				if (maze[pl->i][pl->j + 1] == ' ') {
					moveplayer = 4;
					processar = 1;
				}
				break;
			case 'x':
				colocabomba = 1;
				break;
			}
		}
		if (processar == 1) {
			processar = 0;
			renderizar = 1;
			if (colocabomba == 1) {
				colocabomba = 0;
				colocaBomba(vetordeBombas, pl->i, pl->j);
			}
			realizaMovimentacaoPl(pl, moveplayer);
			if (pl->vivo == 0) {
				printf("voce morreu\n");
				break;
			}
			if (pl->i == isaida && pl->j == jsaida) {
				printf("voce ganhou\n");
				break;
			}
			auxi = base;
			for (int i = 0; i < 5; i++) {
                    for (int z=1;z<5;z++){
                        auximovimento = componenteConexo(monsters[i],pl,z,auxi);
                        if (auximovimento != 0){
                            moverMonstro(monsters[i],auximovimento);
                            auximovimento = 0;
                            break;
                        }
                    }
            }
            checaExplosao(monsters, vetordeBombas);
			checaColisao(pl, monsters);
				//printf("%d\n", auximovimento);
			}

		}
		return 0;
	}


