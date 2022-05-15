#ifndef LPSNAKE_H_INCLUDED
#define LPSNAKE_H_INCLUDED

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400
#define CELULAS_TOTAIS 2400

#define W 0
#define S 1
#define A 2
#define D 3

void desenho(SDL_Renderer *Redenizador);

uint8_t eventos(SDL_Event *evento);

void movimentacao(void);

void inicializarPosicoes(uint16_t N_Elementos, uint16_t *matriz);

uint8_t comer(uint16_t *atraso);

void reposicionarAlimento(uint16_t *x, uint16_t *y);

void movimentacaoRabo(void);

void reposicionarElementosAposRetirada(void);

void movimentacaoRabo(void);

void retirarXYparaRabo(void);

void salvarAtualPosicaoDeCabeca(void);

void fluirValoresDasMatrizesParaAEsquerda(void);

uint8_t foraDaTela(void);

bool snakeComeASiMesma(uint16_t a);

unsigned int randto(unsigned int numax);

uint8_t verificacaoDeValidadeDeMovimento(char teclapressionada, char teclacontraria);

#endif //LPSNAKE_H_INCLUDED

