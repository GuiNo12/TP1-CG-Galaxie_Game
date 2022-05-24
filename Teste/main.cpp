#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <irrKlang.h>
#include <string.h>

#include "Bloco.h"
#include "MovimentoPadrao.h"

using namespace irrklang;

//VARIAVEIS GLOBAIS--------------
ISoundEngine *SoundEngine = createIrrKlangDevice();

GLuint idTexturaZumbi, idTexturaAtirador, idTexturaDisparo, idTexturaFundo;

AreaMovimento areaMovimento = {5,215,15};
MovimentoPadrao movimentoPadrao;

int qtdzumbis = 35, fase = 1;

Bloco atirador, municaoJogador, municaoZumbi, posicaoInicialZumbis[35], zumbis[35];
Bloco fundo = {0,0,600,600,0,true};

char textoFase[8];

bool pausado = false, disparou = false, disparouZumbi = false, reiniciar = false;
int pontuacao = 1;


GLuint carregaTextura(const char* arquivo) {
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
}

void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}

//Texturas de movimento do atirador
void movimentoEsquerdoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo.png");
}

void movimentoDireitoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo_direita.png");
}

void movimentoInimigo(){
    defineProximoMovimentoPadrao(movimentoPadrao,areaMovimento);

    for(int i = qtdzumbis-1; i > -1; i--)
        if(zumbis[i].visivel){
            zumbis[i].x = posicaoInicialZumbis[i].x + movimentoPadrao.x;
            zumbis[i].y = posicaoInicialZumbis[i].y + movimentoPadrao.y;
        }
}

//Efeitos Sonoros
void gunHitSound(){
    SoundEngine->play2D("gun-hit.mp3", false);
}

void soundTrack(){
    SoundEngine->play2D("soundtrack.mp3", true);
}

void movimentoAtirar(){
    idTexturaAtirador = carregaTextura("atirando.png");
}

void desenhaBloco(Bloco bloco){
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0);
        glVertex3f(bloco.x, bloco.y, 0.0);

        glTexCoord2f(1, 0);
        glVertex3f(bloco.x + bloco.largura, bloco.y, 0.0);

        glTexCoord2f(1, 1);
        glVertex3f(bloco.x + bloco.largura, bloco.y + bloco.altura, 0.0);

        glTexCoord2f(0, 1);
        glVertex3f(bloco.x, bloco.y + bloco.altura, 0.0);
    glEnd();
}

void inicializaInimigos(){
    inicializaMovimentoPadrao(movimentoPadrao);

    float xInicial = 5, yInicial = 600;
    float espacamento=15,largura=40,altura=40,x=xInicial,y=yInicial,velocidade=10;
    int qtdColunas = 7;

    for(int i = 0; i < qtdzumbis; i++){
        if(i % qtdColunas == 0){
            x = xInicial;
            y = y - espacamento - altura;
        }

        posicaoInicialZumbis[i].x = x;
        posicaoInicialZumbis[i].y = y;

        zumbis[i].x = x;
        zumbis[i].y = y;
        zumbis[i].altura = altura;
        zumbis[i].largura = largura;
        zumbis[i].velocidade = velocidade;
        zumbis[i].visivel = true;

        x = x + espacamento + largura;
    }
}

void inicializa() {
    inicializaAtirador(atirador);
    inicializaMunicaoJogador(municaoJogador);
    inicializaMunicaoZumbi(municaoZumbi);
    inicializaInimigos();

    glClearColor(1, 1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaZumbi = carregaTextura("zumbi.png");
    idTexturaDisparo = carregaTextura("municao.png");
    idTexturaFundo = carregaTextura("fundo.png");
    movimentoAtirar();
}

bool colisao(float Ax, float Ay, float Alarg, float Aalt, float Bx, float By, float Blarg, float Balt) {
    if(Ay + Aalt < By) return false;
    else if(Ay > By+Balt) return false;
    else if(Ax+Alarg < Bx) return false;
    else if(Ax > Bx+Blarg) return false;
    return true;
}

void desenhaInimigos(){
    for(int i = 0; i < qtdzumbis; i++){
        if(zumbis[i].visivel){
            desenhaBloco(zumbis[i]);
        }
    }
}

void novaFase(){
    fase += 1;
    inicializaInimigos();
    movimentoPadrao.velocidade += fase*0.1;
    printf("Fase: %d, velocidade: %f\n",fase,movimentoPadrao.velocidade);
}

void desenhaDisparoJogador(){
    if(disparou){
        if(municaoJogador.y <= (600 - municaoJogador.altura - municaoJogador.velocidade)){
            municaoJogador.y += municaoJogador.velocidade;
            for(int i = 0; i < qtdzumbis; i++){
                if(zumbis[i].visivel)
                    if(colisao(municaoJogador.x, municaoJogador.y, municaoJogador.largura, municaoJogador.altura, zumbis[i].x, zumbis[i].y, zumbis[i].largura, zumbis[i].altura) == true) {
                        disparou = false;
                        pontuacao++;
                        zumbis[i].visivel = false;
                        gunHitSound();

                        if(blocosApagados(zumbis,qtdzumbis)){
                            novaFase();
                        }

                        break;
                    }
            }
        } else{
            disparou = false;
            return ;
        }

        desenhaBloco(municaoJogador);
    }
}

void atirar(){
    if(!disparou){
        disparou = true;
        municaoJogador.x = atirador.x + 25;
        municaoJogador.y = atirador.y + atirador.altura + 10;
        SoundEngine->play2D("gun-shot.mp3", false);
    }
}

void desenhaDisparoZumbi(){
    if(disparouZumbi){
        printf("Zumbi %lf, %lf, %lf, %lf\n", municaoZumbi.x, municaoZumbi.y, municaoZumbi.velocidade, municaoZumbi.altura);
        if(municaoZumbi.y >= 0){
            municaoZumbi.y -= municaoZumbi.velocidade;
            if(colisao(municaoZumbi.x, municaoZumbi.y, municaoZumbi.largura, municaoZumbi.altura, atirador.x, atirador.y, atirador.largura, atirador.altura) == true) {
                disparouZumbi = false;
                pontuacao--;
                gunHitSound();

            }
        }else {
            disparouZumbi = false;
            return ;
        }

        desenhaBloco(municaoZumbi);
    }
}

void atirarZumbi(int periodo){
    int zumbiAtirador = rand() % 34;
    if(!disparouZumbi){
        disparouZumbi = true;
        municaoZumbi.x = zumbis[zumbiAtirador].x + 25;
        municaoZumbi.y = zumbis[zumbiAtirador].y - 10;
        SoundEngine->play2D("gun-shot.mp3", false);
    }

    glutPostRedisplay();
    glutTimerFunc(periodo, atirarZumbi, periodo);
}

//Escreve texto na tela pausada
void textoTelaPause(){
    if(pausado){
        escreveTexto(GLUT_BITMAP_TIMES_ROMAN_24, "       Zombie Kill!", 200, 300, 0);
        escreveTexto(GLUT_BITMAP_HELVETICA_18,   "Use <-, -> para controle", 200, 280, 0);
        escreveTexto(GLUT_BITMAP_HELVETICA_18,   "    (P) para play/pause", 200, 260, 0);
    }
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);
    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, idTexturaFundo);
    desenhaBloco(fundo);

    glBindTexture(GL_TEXTURE_2D, idTexturaZumbi);
    desenhaInimigos();

    glBindTexture(GL_TEXTURE_2D, idTexturaDisparo);
    desenhaDisparoJogador();

    glBindTexture(GL_TEXTURE_2D, idTexturaDisparo);
    desenhaDisparoZumbi();

    glBindTexture(GL_TEXTURE_2D, idTexturaAtirador);
    desenhaBloco(atirador);

    glDisable(GL_TEXTURE_2D);

    textoTelaPause();
    //Mostra a fase atual
    sprintf(textoFase, "Fase: %d", fase);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, textoFase, 5, 5, 0);

    glutSwapBuffers();
}

void atualizaCena(int periodo){
    //Atualizar a tela
    if(!pausado && !reiniciar){
        movimentoInimigo();
        desenhaDisparoJogador();
        desenhaDisparoZumbi();
        for(int i = 0; i < qtdzumbis; i++){
            if(zumbis[i].visivel){
                if(colisao(atirador.x, atirador.y, atirador.largura, atirador.altura, zumbis[i].x, zumbis[i].y, zumbis[i].largura, zumbis[i].altura) == true) {
                    exit(0);
                }
            }
        }

        if (pontuacao == 0) {
            exit(0);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(periodo, atualizaCena, periodo);
}

void teclas_de_seta (int tecla, int x, int y ){
    if(!pausado){
        switch ( tecla ) {
        case GLUT_KEY_LEFT:
            atirador.x = (atirador.x >= atirador.velocidade)? atirador.x - atirador.velocidade : 0;
            movimentoEsquerdoAtirador();
            break;
        case GLUT_KEY_RIGHT:
            atirador.x = (atirador.x <= (600 - atirador.largura - atirador.velocidade))? atirador.x + atirador.velocidade : 600 - atirador.largura;
            movimentoDireitoAtirador();
            break;
      }
    }
}

void teclado(unsigned char key, int x, int y) {
    if(key == 27)
        exit(0);
    if(key == 32) {
        //Espaco atira
        if(!pausado){
            movimentoAtirar();
            atirar();
            return;
        }
        else
            return;
    }

    if(key == 112 || key == 80){
        if(!pausado){
            pausado = true;
            return;
        }
        if(pausado){
            pausado = false;
            return;
        }
    }

     if(key == 114 || key == 82){
        if(reiniciar == false){
            reiniciar = true;
            pontuacao = 0;
            inicializa();
            desenha();
            reiniciar = false;
            return;
        }
     }
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 600, 0, 600, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void atualiza() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Zombie Kill!");

    inicializa();

    soundTrack();

    glutReshapeFunc(redimensiona);
    glutSpecialFunc(teclas_de_seta);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);
    glutIdleFunc(atualiza);

    //Atualiza a cena
    glutTimerFunc(0, atualizaCena, 30);
    glutTimerFunc(0, atirarZumbi, 5000);

    glutMainLoop();

    return 0;
}
