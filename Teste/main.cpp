#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <irrKlang.h>

using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

GLuint idTexturaZumbi,idTexturaAtirador, idTexturaDisparo;

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

//Struct de objeto
typedef struct {
    float x;
    float y;
    float altura;
    float largura;
    float velocidade;
    bool visivel;
} Bloco;

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

AreaMovimento areaMovimento = {5,115,15};
MovimentoPadrao movimentoPadrao = {10,0,0,1,0};

//VARIAVEIS GLOBAIS--------------
int qtdzumbis = 35;
Bloco posicaoInicialZumbis[35],zumbis[35];
Bloco atirador;
Bloco municao;

bool pausado = false, disparou = false;


//Texturas de movimento do atirador
void movimentoEsquerdoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo.png");
}
void movimentoDireitoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo_direita.png");
}


void movimentoInimigo(){
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
    blocoMunicao.velocidade = 0.5;
    blocoMunicao.x = 250;
    blocoMunicao.y = 70;
}

void inicializaInimigos(){
    float xInicial =5, yInicial = 500;
    float espacamento=15,largura=40,altura=40,x=xInicial,y=yInicial,velocidade=0.2;
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
    inicializaMunicao(municao);
    inicializaInimigos();

    glClearColor(1, 1, 1, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaZumbi = carregaTextura("zumbi.png");
    idTexturaDisparo = carregaTextura("municao.jpg");
    movimentoAtirar();
}

bool colisao(float Ax, float Ay, float Alarg, float Aalt, float Bx, float By, float Blarg, float Balt) {
    if(Ay + Aalt < By) return false;
    else if(Ay > By+Balt) return false;
    else if(Ax+Alarg < Bx) return false;
    else if(Ax > Bx+Blarg) return false;
    gunHitSound();
    return true;
}

void desenhaInimigos(){
    for(int i = 0; i < qtdzumbis; i++){
        if(zumbis[i].visivel){
            glBegin(GL_TRIANGLE_FAN);
                glTexCoord2f(0, 0);
                glVertex3f(zumbis[i].x, zumbis[i].y, 0.0);

                glTexCoord2f(1, 0);
                glVertex3f(zumbis[i].x + zumbis[i].largura, zumbis[i].y, 0.0);

                glTexCoord2f(1, 1);
                glVertex3f(zumbis[i].x + zumbis[i].largura, zumbis[i].y + zumbis[i].altura, 0.0);

                glTexCoord2f(0, 1);
                glVertex3f(zumbis[i].x, zumbis[i].y + zumbis[i].altura, 0.0);
            glEnd();
        }
    }
}

void desenhaAtirador(){
    glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0, 0);
        glVertex3f(atirador.x, atirador.y, 0.0);

        glTexCoord2f(1, 0);
        glVertex3f(atirador.x + atirador.largura, atirador.y, 0.0);

        glTexCoord2f(1, 1);
        glVertex3f(atirador.x + atirador.largura, atirador.y + atirador.altura, 0.0);

        glTexCoord2f(0, 1);
        glVertex3f(atirador.x, atirador.y + atirador.altura, 0.0);
    glEnd();
}

void desenhaDisparo(){
    if(disparou){
        if(municao.y <= (500 - municao.altura - municao.velocidade)){
            municao.y += municao.velocidade;
            for(int i = 0; i < qtdzumbis; i++){
                if(zumbis[i].visivel)
                    if(colisao(municao.x, municao.y, municao.largura, municao.altura, zumbis[i].x, zumbis[i].y, zumbis[i].largura, zumbis[i].altura) == true) {
                        disparou = false;
                        zumbis[i].visivel = false;
                        break;
                    }
            }
        }
        else{
            disparou = false;
            return ;
        }

        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0);
            glVertex3f(municao.x, municao.y, 0.0);
            glTexCoord2f(1, 0);
            glVertex3f(municao.x + municao.largura, municao.y, 0.0);
            glTexCoord2f(1, 1);
            glVertex3f(municao.x + municao.largura, municao.y + municao.altura, 0.0);
            glTexCoord2f(0, 1);
            glVertex3f(municao.x, municao.y + municao.altura, 0.0);
        glEnd();
    }
}

void atirar(){
    if(!disparou){
        disparou = true;
        municao.x = atirador.x + 25;
        municao.y = atirador.y + atirador.altura +10;
        SoundEngine->play2D("gun-shot.mp3", false);
    }
}

void atualizaCena(int periodo){
    //Atualizar a tela
    if(!pausado){
        movimentoInimigo();
        desenhaDisparo();
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
            atirador.x = (atirador.x <= (500 - atirador.largura - atirador.velocidade))? atirador.x + atirador.velocidade : 500 - atirador.largura;
            movimentoDireitoAtirador();
            break;
        default:
            printf("Teclaram: %d\n", tecla);
          break;
      }
    }
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            exit(0);
        case 32:
            //Espaco atira
            if(!pausado){
                movimentoAtirar();
                atirar();
                break;
            }
            else
                break;
        case 112:
            if(!pausado){
                pausado = true;
                break;
            }
            if(pausado){
                pausado = false;
                break;
            }
    }
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Começa a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaZumbi);

    desenhaInimigos();

    glBindTexture(GL_TEXTURE_2D, idTexturaDisparo);
    desenhaDisparo();

    glBindTexture(GL_TEXTURE_2D, idTexturaAtirador);
    desenhaAtirador();
    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 500, 0, 500, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
}

void atualiza() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

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

    glutMainLoop();

    return 0;
}
