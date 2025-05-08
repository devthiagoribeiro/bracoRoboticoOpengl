#include <GL/freeglut.h>
#include <iostream>

// Segmentos do braço robótico
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

// Segmentos
SegmentoRobo base = { 1.0f, 1.0f, 0.5f, 0, 0, 0, {0.3f, 0.3f, 0.3f} };
SegmentoRobo braco = { 2.0f, 0.4f, 0.4f, 0, 0, 45, {0.8f, 0.2f, 0.2f} };
SegmentoRobo antebraco = { 1.5f, 0.3f, 0.3f, 0, 0, -90, {0.2f, 0.8f, 0.2f} };
Garra garra = { 20.0f, 0.5f, 0.1f, {0.2f, 0.2f, 0.8f} };

// Controle da posição da base
float posX = 0.0f, posY = 0.0f, posZ = -8.0f;

// Segmento atualmente selecionado
int segmentoSelecionado = 1;

// Modo de câmera: 0 = padrão, 1 = lateral, 2 = superior, 3 = frontal
int modoCamera = 0;

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
        glTranslatef(0.25f, 0.0f, 0.0f);
        glRotatef(g.abertura, 0, 0, 1);
        glTranslatef(g.comprimento / 2, 0.0f, 0.0f);
        glScalef(g.comprimento, g.largura, g.largura);
        glutSolidCube(1.0f);
    glPopMatrix();

    // Garra direita
    glPushMatrix();
        glTranslatef(-0.25f, 0.0f, 0.0f);
        glRotatef(-g.abertura, 0, 0, 1);
        glTranslatef(g.comprimento / 2, 0.0f, 0.0f);
        glScalef(g.comprimento, g.largura, g.largura);
        glutSolidCube(1.0f);
    glPopMatrix();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Câmera baseada no modo selecionado
    switch (modoCamera) {
        case 0:
            glTranslatef(posX, posY, posZ);
            break;
        case 1:
            gluLookAt(5, 0, 0, 0, 0, 0, 0, 1, 0);
            break;
        case 2:
            gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);
            break;
        case 3:
            gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
            break;
    }

    // Base
    glPushMatrix();
        desenhaSegmento(base);

        // Posição para o braço
        glRotatef(braco.anguloZ, 0, 0, 1); // Gira todo o braço em torno da base
        glTranslatef(0.0f, base.altura / 2 + braco.altura / 2, 0.0f);
        glPushMatrix();
        glPopMatrix();

        glRotatef(braco.anguloX, 1, 0, 0);
        glRotatef(braco.anguloY, 0, 1, 0);
        desenhaSegmento(braco);


        // Posição para o antebraço
        glTranslatef(braco.comprimento, 0.0f, 0.0f);
        glPushMatrix();
        glPopMatrix();

        glRotatef(antebraco.anguloX, 1, 0, 0);
        glRotatef(antebraco.anguloY, 0, 1, 0);
        glRotatef(antebraco.anguloZ, 0, 0, 1);
        desenhaSegmento(antebraco);

        // Posição para a garra
        glTranslatef(antebraco.comprimento, 0.0f, 0.0f);
        glPushMatrix();
        glPopMatrix();
        desenhaGarra(garra);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': posY += 0.1f; break;
        case 's': posY -= 0.1f; break;
        case 'a': posX -= 0.1f; break;
        case 'd': posX += 0.1f; break;

        case '1': segmentoSelecionado = 1; break;
        case '2': segmentoSelecionado = 2; break;
        case '3': segmentoSelecionado = 3; break;

        case 'q':
            if (segmentoSelecionado == 1) braco.anguloZ += 5;
            else if (segmentoSelecionado == 2) antebraco.anguloZ += 5;
            break;
        case 'e':
            if (segmentoSelecionado == 1) braco.anguloZ -= 5;
            else if (segmentoSelecionado == 2) antebraco.anguloZ -= 5;
            break;

        case 'u':
            if (segmentoSelecionado == 1) braco.anguloX += 5;
            else if (segmentoSelecionado == 2) antebraco.anguloX += 5;
            break;
        case 'j':
            if (segmentoSelecionado == 1) braco.anguloX -= 5;
            else if (segmentoSelecionado == 2) antebraco.anguloX -= 5;
            break;

        case 'i':
            if (segmentoSelecionado == 1) braco.anguloY += 5;
            else if (segmentoSelecionado == 2) antebraco.anguloY += 5;
            break;
        case 'k':
            if (segmentoSelecionado == 1) braco.anguloY -= 5;
            else if (segmentoSelecionado == 2) antebraco.anguloY -= 5;
            break;

        case 'o': garra.abertura += 2; if (garra.abertura > 45) garra.abertura = 45; break;
        case 'p': garra.abertura -= 2; if (garra.abertura < 0) garra.abertura = 0; break;

        case '+': posZ += 0.5f; break;
        case '-': posZ -= 0.5f; break;

        case '0': modoCamera = 0; break;
        case '4': modoCamera = 1; break;
        case '5': modoCamera = 2; break;
        case '6': modoCamera = 3; break;

        case 27: exit(0);
    }
    glutPostRedisplay();
}

void instrucoes() {
    std::cout << "=== CONTROLES ===\n";
    std::cout << "W/S/A/D - Mover base\n";
    std::cout << "+/-     - Zoom\n";
    std::cout << "1/2/3   - Selecionar segmento (braco/antebraco/garra)\n";
    std::cout << "Q/E     - Rotacionar Z\n";
    std::cout << "U/J     - Rotacionar X\n";
    std::cout << "I/K     - Rotacionar Y\n";
    std::cout << "O/P     - Abrir/Fechar garra\n";
    std::cout << "4       - Visao lateral\n";
    std::cout << "5       - Visao superior\n";
    std::cout << "6       - Visao frontal\n";
    std::cout << "0       - Visao livre (WASD + +/-)\n";
    std::cout << "ESC     - Sair\n";
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braco Robotico Articulado - OpenGL");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);

    instrucoes();

    glutMainLoop();
    return 0;
}
