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

GLuint idTexturaZumbi, idTexturaAtirador, idTexturaDisparo, idTexturaDisparoZumbi, idTexturaFundo;

AreaMovimento areaMovimento = {5,215,15};
MovimentoPadrao movimentoPadrao;

int qtdzumbis = 35, fase = 1, vida = 1;

Bloco atirador, municaoJogador, municaoZumbi, posicaoInicialZumbis[35], zumbis[35];
Bloco fundo = {0,0,600,600,0,true};

char textoFase[8], textoVida[8];

bool pausado = true, terminou = false, disparou = false, disparouZumbi = false, reiniciar = false;

void escreveTexto(void * font, char *s, float x, float y, float z) {
    int i;
    glRasterPos3f(x, y, z);

    for (i = 0; i < strlen(s); i++) {
        glutBitmapCharacter(font, s[i]);
    }
}
//Define estado de fim de jogo
void fimDeJogo(){
    SoundEngine->play2D("sounds/gameover.mp3", false);
    escreveTexto(GLUT_BITMAP_TIMES_ROMAN_24, "       GAME OVER!", 200, 350, 0);
    terminou = true;
    pausado = true;
}

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

/*--Texturas de movimento do atirador---------------------------------------*/
void movimentoEsquerdoAtirador(){
    idTexturaAtirador = carregaTextura("pictures/atirador_movendo.png");
}

void movimentoDireitoAtirador(){
    idTexturaAtirador = carregaTextura("pictures/atirador_movendo_direita.png");
}

void movimentoInimigo(){
    defineProximoMovimentoPadrao(movimentoPadrao,areaMovimento);

    for(int i = qtdzumbis-1; i > -1; i--)
        if(zumbis[i].visivel){
            zumbis[i].x = posicaoInicialZumbis[i].x + movimentoPadrao.x;
            zumbis[i].y = posicaoInicialZumbis[i].y + movimentoPadrao.y;
        }
}
/*--------------------------------------------------------------------------*/

//--Efeitos Sonoros----------------------------------------------------------
void gunHitSound(){
    SoundEngine->play2D("sounds/gun-hit.mp3", false);
}

void soundTrack(){
    SoundEngine->play2D("sounds/soundtrack.mp3", true);
}

void movimentoAtirar(){
    idTexturaAtirador = carregaTextura("pictures/atirando.png");
}
//---------------------------------------------------------------------------

//Desenha bloco correspondente aos objetos do jogo
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
//Inicializa vari?veis dos zumbis
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
//Inicializa o jogo como um todo
void inicializa() {
    inicializaAtirador(atirador);
    inicializaMunicaoJogador(municaoJogador);
    inicializaMunicaoZumbi(municaoZumbi);
    inicializaInimigos();

    glClearColor(1, 1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaZumbi = carregaTextura("pictures/zumbi.png");
    idTexturaDisparo = carregaTextura("pictures/municao.png");
    idTexturaDisparoZumbi = carregaTextura("pictures/tiro_zumbi.png");
    idTexturaFundo = carregaTextura("pictures/fundo.png");

    movimentoAtirar();
}
//Fun??o para verificar colis?o de dois objetos
bool colisao(float Ax, float Ay, float Alarg, float Aalt, float Bx, float By, float Blarg, float Balt) {
    if(Ay + Aalt < By) return false;
    else if(Ay > By+Balt) return false;
    else if(Ax+Alarg < Bx) return false;
    else if(Ax > Bx+Blarg) return false;
    return true;
}
//Desenha vetor de zumbis
void desenhaInimigos(){
    for(int i = 0; i < qtdzumbis; i++){
        if(zumbis[i].visivel){
            desenhaBloco(zumbis[i]);
        }
    }
}
//Seta par?metros para nova faae
void novaFase(){
    fase += 1;
    vida +=1;
    SoundEngine->play2D("sounds/levelup.mp3", false);
    inicializaInimigos();
    movimentoPadrao.velocidade += fase*0.4;
}
//Desenha e define limites para disparo do jogador
void desenhaDisparoJogador(){
    if(disparou && !pausado){
        if(municaoJogador.y <= (600 - municaoJogador.altura - municaoJogador.velocidade)){
            municaoJogador.y += municaoJogador.velocidade;
            for(int i = 0; i < qtdzumbis; i++){
                if(zumbis[i].visivel)
                    if(colisao(municaoJogador.x, municaoJogador.y, municaoJogador.largura, municaoJogador.altura, zumbis[i].x, zumbis[i].y, zumbis[i].largura, zumbis[i].altura) == true) {
                        disparou = false;
                        zumbis[i].visivel = false;
                        SoundEngine->play2D("sounds/zumbi_morte.mp3", false);

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
//Define tiro do jogador
void atirar(){
    if(!disparou){
        disparou = true;
        municaoJogador.x = atirador.x + 25;
        municaoJogador.y = atirador.y + atirador.altura + 10;
        SoundEngine->play2D("sounds/gun-shot.mp3", false);
    }
}
//Desenha e define limites para disparo do zumbi
void desenhaDisparoZumbi(){
    if(disparouZumbi){
        if(municaoZumbi.y >= 0){
            municaoZumbi.y -= municaoZumbi.velocidade;
            if(colisao(municaoZumbi.x, municaoZumbi.y, municaoZumbi.largura, municaoZumbi.altura, atirador.x, atirador.y, atirador.largura, atirador.altura) == true) {
                disparouZumbi = false;
                gunHitSound();
                vida--;

                if(vida == 0){
                    SoundEngine->play2D("sounds/atirador_morte.mp3", false);
                    fimDeJogo();
                }
            }
        }else {
            disparouZumbi = false;
            return ;
        }

        desenhaBloco(municaoZumbi);
    }
}
//Define tiro do zumbi
void atirarZumbi(int periodo){
    int zumbiAtirador;

    if(!disparouZumbi && !pausado && !terminou){
        do {
            zumbiAtirador = rand() % 34;
        } while(!zumbis[zumbiAtirador].visivel);

        disparouZumbi = true;
        municaoZumbi.x = zumbis[zumbiAtirador].x + 25;
        municaoZumbi.y = zumbis[zumbiAtirador].y - 10;
        SoundEngine->play2D("sounds/tiro_zumbi.mp3", false);
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
//Verifica se zumbis chegaram ao fim da tela
void verificaFimJogo(){
    if(!pausado)
        for(int i = 0; i < qtdzumbis; i++){
            if( zumbis[i].visivel ){
                if(((zumbis[i].y - zumbis[i].altura) == 0)){
                    fimDeJogo();
                }
            }
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

    glBindTexture(GL_TEXTURE_2D, idTexturaDisparoZumbi);
    desenhaDisparoZumbi();

    glBindTexture(GL_TEXTURE_2D, idTexturaAtirador);
    desenhaBloco(atirador);
    glDisable(GL_TEXTURE_2D);

    textoTelaPause();

    verificaFimJogo();

    //Mostra texto ao pausar a fase atual
    sprintf(textoFase, "Fase: %d", fase);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, textoFase, 5, 5, 0);

    sprintf(textoVida, " Vida: %d", vida);
    escreveTexto(GLUT_BITMAP_HELVETICA_18, textoVida, 75, 5, 0);

    glutSwapBuffers();
}
//Atualiza os objetos na tela
void atualizaCena(int periodo){
    //Atualizar a tela
    if(!pausado){
        movimentoInimigo();
        desenhaDisparoJogador();
        desenhaDisparoZumbi();
        for(int i = 0; i < qtdzumbis; i++){
            if(zumbis[i].visivel){
                if(colisao(atirador.x, atirador.y, atirador.largura, atirador.altura, zumbis[i].x, zumbis[i].y, zumbis[i].largura, zumbis[i].altura) == true) {
                    fimDeJogo();
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(periodo, atualizaCena, periodo);
    }

//Captura ativa??o das setas
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
//Reinicializa par?metros do jogo
void reiniciaGame(){
    terminou = false;
    vida = 1;
    fase = 1;
    inicializa();
    desenha();
}
//Captura demais teclas
void teclado(unsigned char key, int x, int y) {
    //Tecla ESC
    if(key == 27)
        exit(0);
    //Tecla ESPACO
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
    //Tecla P
    if(key == 112 && !terminou){
        if(!pausado){
            pausado = true;
            return;
        }
        if(pausado && vida != 0){
            SoundEngine->play2D("sounds/gamestart.mp3", false);
            pausado = false;
            return;
        }
    }
    //Tecla R
     if(key == 114){
        reiniciaGame();
     }
}
//Redimensiona janela
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
    glutInitWindowPosition((1920/2)-300, (1080/2)-300);

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
