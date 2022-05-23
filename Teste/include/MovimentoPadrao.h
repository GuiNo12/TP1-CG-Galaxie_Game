#ifndef MOVIMENTOPADRAO_H
#define MOVIMENTOPADRAO_H


typedef struct {
    float xMin;
    float xMax;
    float quantidadeDescidaMax;
} AreaMovimento;

typedef struct {
    float x;
    float y;
    float qtdDescida;
    float velocidade;
    int tipoMovimento;
} MovimentoPadrao;

void inicializaMovimentoPadrao(MovimentoPadrao &movimentoPadrao);
void defineProximoMovimentoPadrao(MovimentoPadrao &movimentoPadrao, AreaMovimento &areaMovimento);

#endif // MOVIMENTOPADRAO_H
