/* LPLA-br 2021 2022
 * código sob a licensa bsd
 * de 3 cáusulas.
 * leia LICENSE para detalhes.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <SDL.h> /*sdl versão 2.0.9*/
#include "lpsnake.h"

//variáveis e estruturas globais inicializadas.
SDL_Rect cabeca = {100, 100, 10, 10};
SDL_Rect rabo = {100, 100, 10, 10};
SDL_Rect alimento = {200, 200, 10, 10};
char teclatual = 'W';
char ultimatecla = 'S';
unsigned int pontuacao = 0;
uint16_t snaketamanho = 0;
uint16_t caminhoX[CELULAS_TOTAIS];
uint16_t caminhoY[CELULAS_TOTAIS];
bool sinal = false;
uint8_t red = 0x00, green = 0xBB, blue = 0x00;


int main(int argc,int *argv[])
{

	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("\n%s", SDL_GetError());
		return 1;
	}

	uint16_t atraso = 50;	

	//refatoração, leitura da linha de comando melhorada.
	switch(argc)
	{
		case 1:
			break;
		case 2:
			printf("velocidade de execução ajustada para: %s", *(argv+1) );
			atraso = atoi( (char *)(*(argv+1)) );
			break;
		case 3:
			if( !strcmp((char *)(*(argv+2)), "vermelho") )
			{
				red = 0xFF;
				green = 0x00;
				blue = 0x00;
			}
			else if( !strcmp((char *)(*(argv+2)), "azul") )
			{
				red = 0x00;
				green = 0x00;
				blue = 0xFF;
			}
			else if( !strcmp((char *)(*(argv+2)), "amarelo") )
			{
				red = 0xFF;
				green = 0xFF;
				blue = 0x00;
			}
			else if( !strcmp((char *)(*(argv+2)), "rosa") )
			{
				red = 0xFF;
				green = 0x00;
				blue = 0x80;
			}
			else if( !strcmp((char *)(*(argv+2)), "laranja") )
			{
				red = 0xFF;
				green = 0x99;
				blue = 0x80;
			}
			else printf("\n Cor passada no segundo argumento é inválida!");
			break;
		default:
			break;
	}

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
	//SDL_Surface *icone;
	int8_t loop = 1;

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
	SDL_SetWindowTitle(window, "lpsnake");
	//função SDL_LoadBMP precisa de tratamento de erro.
	//icone = SDL_LoadBMP("./lpsnake32x32.bmp");
	//if(icone == NULL) exit(0);
	//SDL_SetWindowIcon(window, icone);

	uint16_t alimentoPosicoesX[60]; /*0 até 59*/
	uint16_t alimentoPosicoesY[40]; /*0 até 39*/

	inicializarPosicoes(60, alimentoPosicoesX);
	inicializarPosicoes(40, alimentoPosicoesY);

	SDL_RenderClear(renderer);

	while(loop)
	{
		salvarAtualPosicaoDeCabeca();
		loop = eventos(&event);
		if( foraDaTela() == 1 || sinal)
		{
			printf("\nvocê perdeu.\nsua pontuação:%d\n", pontuacao);
			loop = 0;
		}
		movimentacaoRabo();
		fluirValoresDasMatrizesParaAEsquerda();
		desenho(renderer);
		if( comer(&atraso) ) reposicionarAlimento(alimentoPosicoesX, alimentoPosicoesY);
		SDL_Delay(atraso);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void desenho(SDL_Renderer *Redenizador)
{
		//SDL_RenderClear(Redenizador);

		SDL_SetRenderDrawColor(Redenizador, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderDrawRect(Redenizador, &rabo);
		SDL_RenderFillRect(Redenizador, &rabo);

		SDL_SetRenderDrawColor(Redenizador, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(Redenizador, &alimento);
		SDL_RenderFillRect(Redenizador, &alimento);

		SDL_SetRenderDrawColor(Redenizador, red, green, blue, 0xFF);
		SDL_RenderDrawRect(Redenizador, &cabeca);
		SDL_RenderFillRect(Redenizador, &cabeca);

		SDL_SetRenderDrawColor(Redenizador, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderPresent(Redenizador);
}

uint8_t eventos(SDL_Event *evento)
{
	/*W A S D contém verificação feia de movimetos ilegais
	 * return 1 continua loop, return 0 para o loop
	 * refatorado e melhorado.*/
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_PollEvent(evento);
	if( keys[SDL_SCANCODE_Q])
	{
		return 0;
	}
	else if( keys[SDL_SCANCODE_W] )
	{
		return verificacaoDeValidadeDeMovimento('W','S');
	}
	else if( keys[SDL_SCANCODE_S] )
	{
		return verificacaoDeValidadeDeMovimento('S','W');
	}
	else if( keys[SDL_SCANCODE_A] )
	{
		return verificacaoDeValidadeDeMovimento('A','D');
	}
	else if( keys[SDL_SCANCODE_D] )
	{
		return verificacaoDeValidadeDeMovimento('D','A');
	}
	else ;
	movimentacao();
	return 1;

}

void movimentacao(void)
{
	switch( teclatual )
	{
		case 'W': /*w*/
			cabeca.y -= 10;
			break;
		case 'S': /*s*/
			cabeca.y += 10;
			break;
		case 'A': /*a*/
			cabeca.x -= 10;
			break;
		case 'D': /*d*/
			cabeca.x += 10;
			break;
		default:
			break;
	}
}

void inicializarPosicoes(uint16_t N_Elementos, uint16_t *matriz)
{
	uint16_t i, valor = 0;
	for(i=0; i<N_Elementos; i++)
	{
		*(matriz + i) = valor;
		valor += 10;
	}
}

uint8_t comer(uint16_t *atraso)
{
	if((cabeca.x == alimento.x)&&(cabeca.y == alimento.y))
	{
		pontuacao++;
		snaketamanho++;
		//dificudade crescente, atraso decrescente
		if(*atraso > 2) *atraso--;
		return 1;
	}
	else return 0;
}

void reposicionarAlimento(uint16_t *x, uint16_t *y)
{
	alimento.x = (int)( *(x+randto(60)) );
	alimento.y = (int)( *(y+randto(40)) );
}

void movimentacaoRabo(void)
{
	retirarXYparaRabo();
}

void retirarXYparaRabo(void)
{
	rabo.x = *(caminhoX+0);
	rabo.y = *(caminhoY+0);
	*(caminhoX+0) = 0;
	*(caminhoY+0) = 0;
}

void salvarAtualPosicaoDeCabeca(void)
{
	/*De acordo com "snaketamanho"*/
	*(caminhoX+snaketamanho) = cabeca.x;
	*(caminhoY+snaketamanho) = cabeca.y;
}

void fluirValoresDasMatrizesParaAEsquerda(void)
{
	/*fluxo de dados nas duas matrizes
	fluxo de dados de 0 até snaketamanho
	possível problema de velocidade de
	execução conforme a cobra cresce*/
	uint8_t a, b = 1;
	for(a = 0; a<snaketamanho; a++)
	{
		//aproveitando esta iteração for
		if(snakeComeASiMesma(a) == true) sinal=true;
		*(caminhoX+a) = *(caminhoX+b);
		*(caminhoY+a) = *(caminhoY+b);
		b++;
	}
}

uint8_t foraDaTela(void)
{
	if( (cabeca.x < 0 || cabeca.x > SCREEN_WIDTH) || (cabeca.y < 0 || cabeca.y > SCREEN_HEIGHT) ) return 1;
	else return 0;
}

bool snakeComeASiMesma(uint16_t a)
{
	//para função fluirvaloresparaesquerda.
	if(cabeca.x == *(caminhoX+a) && cabeca.y == *(caminhoY+a)) return true;
	else return false;
}

//retorna número pseudo aleatório.
unsigned int randto(unsigned int numax)
{
        return (rand() % numax);
}

/*funcão de verificação de validade do movimento de snake
 *para as teclas w,a,s,d. Esta função retorna valor correpodente
 * a teclas de movimento para a função eventos*/
uint8_t verificacaoDeValidadeDeMovimento(char teclapressionada, char teclacontraria)
{
	if(teclatual != teclapressionada) ultimatecla = teclatual;
	teclatual = teclapressionada;
	if(ultimatecla == teclacontraria)
	{
		teclatual = teclacontraria;
		movimentacao();
	}
	else movimentacao();
	return 1;
}

