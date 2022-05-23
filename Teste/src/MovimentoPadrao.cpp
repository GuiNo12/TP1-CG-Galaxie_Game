#include "MovimentoPadrao.h"

void inicializaMovimentoPadrao(MovimentoPadrao &movimentoPadrao){
    movimentoPadrao.x = 10;
    movimentoPadrao.y = 0;
    movimentoPadrao.qtdDescida = 0;
    movimentoPadrao.velocidade = 1;
    movimentoPadrao.tipoMovimento = 0;
}

void defineProximoMovimentoPadrao(MovimentoPadrao &movimentoPadrao, AreaMovimento &areaMovimento){
    if( movimentoPadrao.tipoMovimento == 0){ // tipoMovimento == 0 -> zumbi vai para direita
        if(movimentoPadrao.x <= areaMovimento.xMax){
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para direita
        }else{
            movimentoPadrao.tipoMovimento = 1;
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
        }
    }else if(movimentoPadrao.tipoMovimento == 1 ){ // tipoMovimento == 1 -> zumbi vai para baixo
        if(movimentoPadrao.qtdDescida < areaMovimento.quantidadeDescidaMax){
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
        }else{
            movimentoPadrao.qtdDescida = 0;
            movimentoPadrao.tipoMovimento = 2;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para esquerda
        }
    }else if(movimentoPadrao.tipoMovimento == 2){
        if(movimentoPadrao.x > areaMovimento.xMin){
            movimentoPadrao.x -= movimentoPadrao.velocidade;// vai para esquerda
        }else{
            movimentoPadrao.tipoMovimento = 3;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para direita
        }
    }else{
        if(movimentoPadrao.qtdDescida < areaMovimento.quantidadeDescidaMax){
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
        }else{
            movimentoPadrao.qtdDescida = 0;
            movimentoPadrao.tipoMovimento = 0;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para esquerda
        }
    }
}
