#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

GLuint idTexturaZumbi,idTexturaAtirador;

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
//Posi��o do controle teclado
struct ponto {
    float x, y;
};
struct ponto posicaoTeclado;
//Struct de objeto
typedef struct {
    float x;
    float y;
    float altura;
    float largura;
    float velocidade;
} bloco;

int qtdBlocos = 35;
bloco blocos[35];
//ATIRADOR
bloco atirador;

//Texturas de movimento do atirador
void movimentoEsquerdoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo.png");
}

void movimentoDireitoAtirador(){
    idTexturaAtirador = carregaTextura("atirador_movendo_direita.png");
}

void movimentoAtirar(){
    idTexturaAtirador = carregaTextura("atirando.png");
}

void inicializaInimigos(){
    float xInicial = 10, yInicial = 500;
    float espacamento=15,largura=40,altura=40,x=xInicial,y=yInicial,velocidade=2;
    int qtdColunas = 7;

    for(int i = 0; i < qtdBlocos; i++){
        if(i % qtdColunas == 0){
            x = xInicial;
            y = y - espacamento - altura;
        }

        blocos[i].x = x;
        blocos[i].y = y;
        blocos[i].altura = altura;
        blocos[i].largura = largura;
        blocos[i].velocidade = velocidade;

        x = x + espacamento + largura;
    }
}

void desenhaInimigos(){
    for(int i = 0; i < qtdBlocos; i++){
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0, 0);
            glVertex3f(blocos[i].x, blocos[i].y, 0.0);

            glTexCoord2f(1, 0);
            glVertex3f(blocos[i].x + blocos[i].largura, blocos[i].y, 0.0);

            glTexCoord2f(1, 1);
            glVertex3f(blocos[i].x + blocos[i].largura, blocos[i].y + blocos[i].altura, 0.0);

            glTexCoord2f(0, 1);
            glVertex3f(blocos[i].x, blocos[i].y + blocos[i].altura, 0.0);
        glEnd();
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


void atualizaCena(int periodo){
    //atirador.x += (posicaoTeclado.x - atirador.x) / 1.0f;
    //atirador.y += (posicaoTeclado.y - atirador.y) / 1.0f;
    //Atualizar a tela
    glutPostRedisplay();
    glutTimerFunc(periodo, atualizaCena, periodo);
}

void inicializa() {
    //Inicializa atirador
    atirador.altura = 40;
    atirador.largura = 40;
    atirador.velocidade = 10;
    atirador.x = 210;
    atirador.y = 10;

    glClearColor(1, 1, 1, 1);

    glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    idTexturaZumbi = carregaTextura("zumbi.png");
    movimentoAtirar();
}

void teclas_de_seta ( int tecla, int x, int y ){

  switch ( tecla ) {
    case GLUT_KEY_LEFT:
        //printf("KEY LEFT\n");

        atirador.x = (atirador.x >= atirador.velocidade)? atirador.x - atirador.velocidade:0;

        movimentoEsquerdoAtirador();
        break;
    case GLUT_KEY_RIGHT:
        //printf("KEY RIGHT\n");
        atirador.x = (atirador.x <= (500 - atirador.largura - atirador.velocidade))? atirador.x + atirador.velocidade : 500 - atirador.largura;
        movimentoDireitoAtirador();
        break;
    default:
		printf("Teclaram: %d\n", tecla);
      break;
  }
}

void teclado(unsigned char key, int x, int y) {

    switch (key) {
        case 27:
            exit(0);
        case 32:
            //Espaco atira
            movimentoAtirar();
            break;
    }
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1, 1, 1);

    // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);

    // Come�a a usar a textura que criamos
    glBindTexture(GL_TEXTURE_2D, idTexturaZumbi);
    inicializaInimigos();
    desenhaInimigos();

    glBindTexture(GL_TEXTURE_2D, idTexturaAtirador);
    desenhaAtirador();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();
}

void redimensiona(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-2, 2, -2, 2, -1.0, 1.0);
    glOrtho(0, 500, 0, 500, -1.0, 1.0);
    //glOrtho(0, w, 0, h, -1, 1);

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

    glutCreateWindow("Carregando textura com SOIL");
    inicializa();

    glutReshapeFunc(redimensiona);
    glutSpecialFunc(teclas_de_seta);
    glutKeyboardFunc(teclado);
    glutDisplayFunc(desenha);
    glutIdleFunc(atualiza);

    //Atualiza a cena
    glutTimerFunc(0, atualizaCena, 10);

    glutMainLoop();

    return 0;
}
