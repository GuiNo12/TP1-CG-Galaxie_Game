#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <irrKlang.h>
#include <string.h>

#include "Bloco.h"
#include "MovimentoPadrao.h"

void inicializaMovimentoPadrao(MovimentoPadrao &movimentoPadrao){
    movimentoPadrao.x = 0;
    movimentoPadrao.y = 0;
    movimentoPadrao.qtdDescida = 0;
    movimentoPadrao.velocidade = 2;
    movimentoPadrao.tipoMovimento = 0;
}
/*
GLuint carregaTexturaZumbi(const char* arquivo) {
    GLuint idTextura = SOIL_load_OGL_texture(
                           arquivo,
                           SOIL_LOAD_AUTO,
                           SOIL_CREATE_NEW_ID,
                           SOIL_FLAG_INVERT_Y
                       );

    if (idTextura == 0) {
        printf("Erro do SOIL: '%s'\n", SOIL_last_result());
    }

    return idTextura;
}*/

//GLuint idTexturaZumbi;
/*
void movimentoEsquerdoZumbi(){
    idTexturaZumbi = carregaTexturaZumbi("pictures/zumbi_esquerdo.png");
}

void movimentoDireitoZumbi(){
    idTexturaZumbi = carregaTexturaZumbi("pictures/zumbi_direito.png");
}

void movimentoBaixoZumbi(){
    idTexturaZumbi = carregaTexturaZumbi("pictures/zumbi.png");
}*/

void defineProximoMovimentoPadrao(MovimentoPadrao &movimentoPadrao, AreaMovimento &areaMovimento){
    if( movimentoPadrao.tipoMovimento == 0){ // tipoMovimento == 0 -> zumbi vai para direita
        if(movimentoPadrao.x <= areaMovimento.xMax){
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para direita
            //movimentoDireitoZumbi();
        }else{
            movimentoPadrao.tipoMovimento = 1;
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
            //movimentoBaixoZumbi();
        }
    }else if(movimentoPadrao.tipoMovimento == 1 ){ // tipoMovimento == 1 -> zumbi vai para baixo
        if(movimentoPadrao.qtdDescida < areaMovimento.quantidadeDescidaMax){
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
            //movimentoBaixoZumbi();
        }else{
            movimentoPadrao.qtdDescida = 0;
            movimentoPadrao.tipoMovimento = 2;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para esquerda
            //movimentoEsquerdoZumbi();
        }
    }else if(movimentoPadrao.tipoMovimento == 2){
        if(movimentoPadrao.x > areaMovimento.xMin){
            movimentoPadrao.x -= movimentoPadrao.velocidade;// vai para esquerda
            //movimentoEsquerdoZumbi();
        }else{
            movimentoPadrao.tipoMovimento = 3;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para direita
            //movimentoDireitoZumbi();
        }
    }else{
        if(movimentoPadrao.qtdDescida < areaMovimento.quantidadeDescidaMax){
            movimentoPadrao.y -= movimentoPadrao.velocidade; // vai para baixo
            movimentoPadrao.qtdDescida += movimentoPadrao.velocidade;
            //movimentoBaixoZumbi();
        }else{
            movimentoPadrao.qtdDescida = 0;
            movimentoPadrao.tipoMovimento = 0;
            movimentoPadrao.x += movimentoPadrao.velocidade; // vai para esquerda
            //movimentoEsquerdoZumbi();
        }
    }
}
