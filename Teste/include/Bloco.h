#ifndef BLOCO_H
#define BLOCO_H

typedef struct {
    float x;
    float y;
    float altura;
    float largura;
    float velocidade;
    bool visivel;
} Bloco;

void inicializaAtirador(Bloco &blocoAtirador);
void inicializaMunicaoJogador(Bloco &blocoMunicao);
void inicializaMunicaoZumbi(Bloco &blocoMunicao);
bool blocosApagados(Bloco blocos[],int quantidade);

#endif // BLOCO_H
