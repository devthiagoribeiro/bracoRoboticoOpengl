#include <GL/freeglut.h>
#include <iostream>

// Estruturas para os segmentos do braço
typedef struct {
    float comprimento;
    float largura;
    float altura;
    float anguloX;
    float anguloY;
    float anguloZ;
    float cor[3];
} SegmentoRobo;

typedef struct {
    float abertura;
    float comprimento;
    float largura;
    float cor[3];
} Garra;

// Enum para modos de câmera
typedef enum {
    CAMERA_PADRAO,
    CAMERA_SUPERIOR,
    CAMERA_FRONTAL
} ModoCamera;

// Controles
int juntaAtiva = 1; // 1 = braço (vermelho), 2 = antebraço (verde)
bool mostrarAjuda = true;
ModoCamera modoCamera = CAMERA_PADRAO;

// Valores iniciais para reiniciar
const SegmentoRobo BASE_INICIAL = { 1.0f, 1.0f, 0.5f, 0, 0, 0, {0.3f, 0.3f, 0.3f} };
const SegmentoRobo BRACO_INICIAL = { 2.0f, 0.4f, 0.4f, 0, 0, 0, {0.8f, 0.2f, 0.2f} };
const SegmentoRobo ANTEBRACO_INICIAL = { 1.5f, 0.3f, 0.3f, 0, 0, 0, {0.2f, 0.8f, 0.2f} };
const Garra GARRA_INICIAL = { 30.0f, 0.5f, 0.1f, {0.2f, 0.2f, 0.8f} };
const float POS_INICIAL_X = 0.0f, POS_INICIAL_Y = 0.0f, POS_INICIAL_Z = -8.0f;

// Componentes do braço
SegmentoRobo base = BASE_INICIAL;
SegmentoRobo braco = BRACO_INICIAL;
SegmentoRobo antebraco = ANTEBRACO_INICIAL;
Garra garra = GARRA_INICIAL;

// Posição da câmera
float posX = POS_INICIAL_X, posY = POS_INICIAL_Y, posZ = POS_INICIAL_Z;

void reiniciarPosicoes() {
    base = BASE_INICIAL;
    braco = BRACO_INICIAL;
    antebraco = ANTEBRACO_INICIAL;
    garra = GARRA_INICIAL;
    posX = POS_INICIAL_X;
    posY = POS_INICIAL_Y;
    posZ = POS_INICIAL_Z;
    juntaAtiva = 1;
    modoCamera = CAMERA_PADRAO;
}

void desenhaSegmento(SegmentoRobo s) {
    glColor3fv(s.cor);
    glPushMatrix();
        glScalef(s.comprimento, s.altura, s.largura);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void desenhaGarra(Garra g) {
    glColor3fv(g.cor);
    
    // Garra esquerda
    glPushMatrix();
        glRotatef(g.abertura, 0, 0, 1);
        glTranslatef(g.comprimento/2, 0, 0);
        glScalef(g.comprimento, g.largura, g.largura);
        glutSolidCube(1.0f);
    glPopMatrix();
    
    // Garra direita
    glPushMatrix();
        glRotatef(-g.abertura, 0, 0, 1);
        glTranslatef(g.comprimento/2, 0, 0);
        glScalef(g.comprimento, g.largura, g.largura);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void desenhaTexto(float x, float y, const char* texto) {
    glRasterPos2f(x, y);
    for (const char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Configuração da câmera baseada no modo selecionado
    switch(modoCamera) {
        case CAMERA_PADRAO:
            gluLookAt(0, 0, posZ, 0, 0, 0, 0, 1, 0);
            break;
        case CAMERA_SUPERIOR:
            gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, -1);
            break;
        case CAMERA_FRONTAL:
            gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
            break;
    }

    // Desenha o braço robótico
    glPushMatrix();
    glTranslatef(posX, posY, 0);
    
    // Base
    desenhaSegmento(base);
    
    // Braço (vermelho)
    glPushMatrix();
        glTranslatef(0, base.altura/2, 0);
        
        // Junta do braço
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidSphere(0.1, 16, 16);
        
        // Rotação do braço
        glRotatef(braco.anguloZ, 0, 0, 1);
        glRotatef(braco.anguloY, 0, 1, 0);
        glRotatef(braco.anguloX, 1, 0, 0);
        
        // Desenha braço
        glTranslatef(braco.comprimento/2, 0, 0);
        desenhaSegmento(braco);
        
        // Antebraço (verde)
        glTranslatef(braco.comprimento/2, 0, 0);
        
        // Junta do antebraço
        glColor3f(0.1f, 0.1f, 0.1f);
        glutSolidSphere(0.08, 16, 16);
        
        // Rotação do antebraço
        glRotatef(antebraco.anguloZ, 0, 0, 1);
        glRotatef(antebraco.anguloY, 0, 1, 0);
        glRotatef(antebraco.anguloX, 1, 0, 0);
        
        // Desenha antebraço
        glTranslatef(antebraco.comprimento/2, 0, 0);
        desenhaSegmento(antebraco);
        
        // Garra (azul)
        glTranslatef(antebraco.comprimento/2, 0, 0);
        desenhaGarra(garra);
    glPopMatrix();
    glPopMatrix();

    // Mostra ajuda na tela
    if (mostrarAjuda) {
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 800, 0, 600);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glColor3f(1, 1, 1);
        
        // Nomes dos alunos (canto superior direito)
        desenhaTexto(600, 580, "Alunos:");
        desenhaTexto(600, 560, "Thalles Inacio Araujo");
        desenhaTexto(600, 540, "Thiago Roberto de Lima Ribeiro");
        
        // Ajuda de controles (canto inferior esquerdo)
        desenhaTexto(10, 580, "CONTROLES:");
        desenhaTexto(10, 560, "1 - Junta Vermelha (Braco)");
        desenhaTexto(10, 540, "2 - Junta Verde (Antebraco)");
        desenhaTexto(10, 520, "EIXO X: Q/A");
        desenhaTexto(10, 500, "EIXO Y: W/S");
        desenhaTexto(10, 480, "EIXO Z: E/D");
        desenhaTexto(10, 460, "GARRA: Z/X");
        desenhaTexto(10, 440, "MOVIMENTO: Setas");
        desenhaTexto(10, 420, "ZOOM: +/-");
        desenhaTexto(10, 400, "R - Reiniciar posicoes");
        desenhaTexto(10, 380, "H - Mostrar/Esconder Ajuda");
        
        // Controles de câmera
        desenhaTexto(10, 360, "CAMERAS:");
        desenhaTexto(10, 340, "3 - Padrao");
        desenhaTexto(10, 320, "4 - Superior");
        desenhaTexto(10, 300, "5 - Frontal");
        
        char status[50];
        sprintf(status, "JUNTA ATIVA: %s", (juntaAtiva == 1) ? "VERMELHA" : "VERDE");
        desenhaTexto(10, 280, status);
        
        const char* modoCameraStr;
        switch(modoCamera) {
            case CAMERA_PADRAO: modoCameraStr = "PADRAO"; break;
            case CAMERA_SUPERIOR: modoCameraStr = "SUPERIOR"; break;
            case CAMERA_FRONTAL: modoCameraStr = "FRONTAL"; break;
        }
        sprintf(status, "CAMERA ATUAL: %s", modoCameraStr);
        desenhaTexto(10, 260, status);
        
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    glutSwapBuffers();
}

void teclado(unsigned char key, int x, int y) {
    SegmentoRobo* alvo = (juntaAtiva == 1) ? &braco : &antebraco;
    
    switch (tolower(key)) {
        case '1': juntaAtiva = 1; break;
        case '2': juntaAtiva = 2; break;
            
        // Eixo X
        case 'q': alvo->anguloX += 5; break;
        case 'a': alvo->anguloX -= 5; break;
            
        // Eixo Y
        case 'w': alvo->anguloY += 5; break;
        case 's': alvo->anguloY -= 5; break;
            
        // Eixo Z
        case 'e': alvo->anguloZ += 5; break;
        case 'd': alvo->anguloZ -= 5; break;
            
        // Garra
        case 'z': garra.abertura += 5; if (garra.abertura > 60) garra.abertura = 60; break;
        case 'x': garra.abertura -= 5; if (garra.abertura < 0) garra.abertura = 0; break;
            
        // Câmeras
        case '3': modoCamera = CAMERA_PADRAO; break;
        case '4': modoCamera = CAMERA_SUPERIOR; break;
        case '5': modoCamera = CAMERA_FRONTAL; break;
            
        // Ajuda
        case 'h': mostrarAjuda = !mostrarAjuda; break;
            
        // Zoom
        case '+': posZ += 0.5; break;
        case '-': posZ -= 0.5; break;
            
        // Reiniciar
        case 'r': reiniciarPosicoes(); break;
    }
    
    glutPostRedisplay();
}

void teclasEspeciais(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT: posX += 0.1; break;
        case GLUT_KEY_RIGHT: posX -= 0.1; break;
        case GLUT_KEY_UP: posY += 0.1; break;
        case GLUT_KEY_DOWN: posY -= 0.1; break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Braco Robotico 3D - Controle de Juntas");
    
    glEnable(GL_DEPTH_TEST);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    
    glutMainLoop();
    return 0;
}