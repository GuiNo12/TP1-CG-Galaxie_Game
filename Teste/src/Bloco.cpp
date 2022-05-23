#include "Bloco.h"

void inicializaAtirador(Bloco &blocoAtirador){
    //Inicializa atirador
    blocoAtirador.altura = 40;
    blocoAtirador.largura = 40;
    blocoAtirador.velocidade = 10;
    blocoAtirador.x = 210;
    blocoAtirador.y = 10;
}

void inicializaMunicao(Bloco &blocoMunicao){
    blocoMunicao.altura = 10;
    blocoMunicao.largura = 10;
    blocoMunicao.velocidade = 1;
    blocoMunicao.x = 250;
    blocoMunicao.y = 70;
}

bool blocosApagados(Bloco blocos[],int quantidade){
    for(int i = 0; i < quantidade; i++){
        if(blocos[i].visivel) return false;
    }

    return true;
}
