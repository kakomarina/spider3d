
#include <cmath>
#include <math.h>
#include <iostream>
#include <GL/glut.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

#define SKYBOX_SIZE 30.0 //tamanho da largura e comprimento do hexaedro utilizado para o skybox
#define SKYBOX_HEIGHT 30.0 //tamanho da altura do hexaedro utilizado para o skybox


typedef struct{
	GLfloat x,y,z; //coordenadas x, y e z
} point; 


int width = 900;
int height = 500;
GLfloat raio1 = 1.8; 
GLfloat raio2 = 1.3; 
GLint xA = 0, yA = 0, zA = 1;
GLint xB = raio1+raio2, yB = 0, zB = 1;
GLfloat alfa = 0; 
float tempo_ant; 
float movimento = 0;
int andando = 0; 
GLfloat rotpers = 1.0; 
 
void drawSky(){
	
	cv::Mat img;
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective(65.0, 1.0, 0.01, 1000.0); 
	glMatrixMode(GL_MODELVIEW);

	glLineWidth(2);
	//iniciliazando parametros da textura
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, img.cols, img.rows, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	//carregando as 6 imagens do skybox
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura z negativa do cube map
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura z positiva do cube map
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura x positiva do cube map
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura x negativa do cube map
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura y positiva do cube map
	img = cv::imread("ceu.jpg");
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_RGBA, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());//deteminacao da imagem a utilizar na textura y negativa do cube map
	
	//definindo os parametros para o cube map
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	std::vector<point> POINTS;
	point aux;
	
	// frente e tras
	for(int i=0; i<2; i++){
		aux.x = -SKYBOX_SIZE;
		aux.y = SKYBOX_HEIGHT;
		aux.z = pow(-1, i)*SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = SKYBOX_SIZE;
		aux.y = SKYBOX_HEIGHT;
		aux.z = pow(-1, i)*SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = SKYBOX_SIZE;
		aux.y = -SKYBOX_HEIGHT;
		aux.z = pow(-1, i)*SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = -SKYBOX_SIZE;
		aux.y = -SKYBOX_HEIGHT;
		aux.z = pow(-1, i)*SKYBOX_SIZE;
		POINTS.push_back(aux);
	}

	// direita e embaixo
	for(int i=0; i<2; i++){
		aux.x = pow(-1, i)*SKYBOX_SIZE;
		aux.y = pow(-1, i)*SKYBOX_HEIGHT;
		aux.z = SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = SKYBOX_SIZE;
		aux.y = -SKYBOX_HEIGHT;
		aux.z = SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = SKYBOX_SIZE;
		aux.y = -SKYBOX_HEIGHT;
		aux.z = -SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = pow(-1, i)*SKYBOX_SIZE;
		aux.y = pow(-1, i)*SKYBOX_HEIGHT;
		aux.z = -SKYBOX_SIZE;
		POINTS.push_back(aux);
	}

	//esquerda e em cima
	for(int i=0; i<2; i++){
		aux.x = -SKYBOX_SIZE;
		aux.y = pow(-1, i)*SKYBOX_HEIGHT;
		aux.z = SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = pow(-1, i)*SKYBOX_SIZE;
		aux.y = SKYBOX_HEIGHT;
		aux.z = SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = pow(-1, i)*SKYBOX_SIZE;
		aux.y = SKYBOX_HEIGHT;
		aux.z = -SKYBOX_SIZE;
		POINTS.push_back(aux);
		aux.x = -SKYBOX_SIZE;
		aux.y = pow(-1, i)*SKYBOX_HEIGHT;
		aux.z = -SKYBOX_SIZE;
		POINTS.push_back(aux);
	}
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	//Desenha o cube map com textura 
	glBegin(GL_QUADS);
	for(int i = 0; i < POINTS.size(); i++){
		glTexCoord3f(POINTS[i].x/SKYBOX_SIZE, POINTS[i].y/SKYBOX_HEIGHT, POINTS[i].z/SKYBOX_SIZE);
		glVertex3f(POINTS[i].x, POINTS[i].y,POINTS[i].z);
	}
	glEnd();
	glDisable(GL_TEXTURE_CUBE_MAP);
}


void drawChao(){ 

	cv::Mat img2 = cv::imread("parede.jpg");
	glColor3f(1, 1, 1); 
	
	//Definindo os parametros da textura 2D para o chao
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2.cols, img2.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img2.ptr());
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glEnable(GL_TEXTURE_2D);
	
	//Desenha o chao com textura nas coordenadas passadas 
	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex3f(-10,0,10);
	glTexCoord2f(0,0); glVertex3f(-10,0,-10);
	glTexCoord2f(1,0); glVertex3f(10,0,-10);
	glTexCoord2f(1,1); glVertex3f(10,0,10);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}


void drawPerninhas(GLfloat width, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
	
	glLineWidth(width); 
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0.0);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
}

void drawAranha(GLfloat raio1, GLfloat raio2, GLint n){
	
	cv::Mat img = cv::imread("a.jpg");
	cv::Mat img2 = cv::imread("pelo.jpg");
	
	//Definindo os parametros da textura 3d	
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, img.cols, img.rows, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, img.ptr());
	
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	
	//glTranslatef(xB, yB, 0);
	glRotatef(alfa, 0, 0, 1.0);		//roda tudo
	//glTranslatef(0, 0, 0);
	glPushMatrix();
	glTranslatef(xA, yA, zA);		//translada só a bunda
	
	/*Comeca a textura da bunda*/
	glEnable(GL_TEXTURE_3D);
	GLUquadric *quadObj = gluNewQuadric();
	gluQuadricTexture(quadObj, GL_TRUE);
	gluSphere(quadObj, raio1, n, n);	//desenha bunda
	gluDeleteQuadric(quadObj);
	glDisable(GL_TEXTURE_3D);
	/*Fim da textura da bunda*/
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(xB, yB, zB);		//translada só a cabeça
	
	/*Comeca a textura da cabeca*/
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, img2.cols, img2.rows, 1, 0, GL_BGR, GL_UNSIGNED_BYTE, img2.ptr());
	glEnable(GL_TEXTURE_3D);
	quadObj = gluNewQuadric();
	gluQuadricTexture(quadObj, GL_TRUE);
	gluSphere(quadObj, raio2, n, n);	//desenha cabeca	
	gluDeleteQuadric(quadObj);
	glDisable(GL_TEXTURE_3D);
	/*Fim da textura da cabeca*/
	
	glPushMatrix();
	glTranslatef(raio2*cos(M_PI/6), raio2*sin(M_PI/6), 0);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.15, 5, 5);	//olho 1
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(raio2*cos(M_PI/6), raio2*sin(-M_PI/6), 0);
	glColor3f(0, 0, 0);
	glutSolidSphere(0.15, 5, 5);	//olho 2
	glPopMatrix(); 
	
	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(-M_PI/4), raio2*sin(M_PI/4), 0);
	float teta = sin(movimento * 2 * M_PI);
	glRotatef(teta * 15, 0, teta * 10, 1);
	drawPerninhas(2, 1.8, 1.5, 1, 2.5, 2, 0);		//perna 1
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(M_PI/4), raio2*sin(-M_PI/4), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(-teta * 15, 0, teta * 10, 1);
	drawPerninhas(2, 1.8, -1.5, 1, 2.5, -2, 0);		//perna 1
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(M_PI/3.5), raio2*sin(-M_PI/3.5), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(teta * 7.5, 0, teta * 7.5, 1);
	drawPerninhas(2, 1, -2.5, 1, 2, -3, 0);		//perna 2
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(-M_PI/3.5), raio2*sin(M_PI/3.5), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(-teta * 7.5, 0, teta * 7.5, 1);
	drawPerninhas(2, 1, 2.5, 1, 2, 3, 0);		//perna 2
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(-M_PI/3), raio2*sin(M_PI/3), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(teta * 7.5, 0, teta * 7.5, 1);
	drawPerninhas(2, 0, 3.5, 1, 1, 4, 0);		//perna 3
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(M_PI/3), raio2*sin(-M_PI/3), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(-teta * 7.5, 0, teta * 7.5, 1);
	drawPerninhas(2, 0, -3.5, 1, 1, -4, 0);		//perna 3
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(M_PI/2.5), raio2*sin(-M_PI/2.5), 0);
	//glRotatef(-30, 0, 0, 1);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(teta * 15, 0, teta * 10, 1);
	drawPerninhas(2, -1, -3.5, 1, -2, -5, 0);		//perna 4
	glPopMatrix();

	glPushMatrix();
	glColor3ub(92, 51, 23);
	glTranslatef(raio2*cos(-M_PI/2.5), raio2*sin(M_PI/2.5), 0);
	teta = sin(movimento * 2 * M_PI);
	glRotatef(-teta * 15, 0, teta * 10, 1);
	drawPerninhas(2, -1, 3.5, 1, -2, 5, 0);		//perna 4
	glPopMatrix();

	glPopMatrix();

}

/**
 * @desc Função de callback para desenho na tela.
 */
void displayCallback(){
	/** Limpa a janela APENAS uma vez */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	glColor3f(1.0f, 0.0f, 0.0f);
	/** Desenha a janela mais a esquerda de cima */
	glViewport(0, 0, width/2, height);
	glLoadIdentity();
	gluLookAt(5.0, 5.0, 10.0, xB*cos(alfa*M_PI/180), yB*sin(alfa*M_PI/180), zB, 0.0, 1.0, 0.0);
	drawChao();
	drawSky();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawAranha(raio1, raio2, 15);

	/** Desenha a janela mais a direita de cima */
	glViewport(width/2, 0, width/2, height);
	glLoadIdentity();
	gluLookAt(0, 10, 10.0, xB*cos(alfa*M_PI/180), yB*sin(alfa*M_PI/180), zB, 0.0, 1.0, 0.0);
	drawChao();
	drawSky();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	drawAranha(raio1, raio2, 15);

	/** Dispara os comandos APENAS uma vez */
	glFlush();
}

/* Realiza a animacao das pernas da aranha*/
void anima(){

	int tempo_novo = glutGet(GLUT_ELAPSED_TIME);

	int dt = tempo_novo - tempo_ant;

	if(andando){
		movimento = fmod(movimento + dt/1000.0 * rotpers, 1);
		andando--; 
	}
	if(!andando) movimento = 0;


	tempo_ant = tempo_novo;
	glutPostRedisplay();
}

void neblina(unsigned char key, int x, int y){
	//a neblina deve aparecer quando a tecla n é pressionada, e desaparecer quando é pressionada novamente

	if(key == 'n'){
		/* Definindo os parametros do fog*/
		GLfloat fogColor[4] = {0.8, 0.8, 0.8, 1};
		GLint fogMode = GL_EXP;
		glFogi(GL_FOG_MODE, fogMode);
		glFogfv(GL_FOG_COLOR, fogColor); 
		glFogf(GL_FOG_DENSITY, 0.2);
		glHint(GL_FOG_HINT, GL_DONT_CARE); 
		glFogf(GL_FOG_START, 1);
		glFogf(GL_FOG_END, 5);

		/*Ativando ou desativando o fog*/ 
		if(glIsEnabled(GL_FOG) == GL_TRUE) glDisable(GL_FOG);
		else glEnable(GL_FOG);
		glutPostRedisplay();
	}

}

void keyboard_func(GLint key, GLint x, GLint y){

	//GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_LEFT, GLUT_KEY_RIGHT
	if(key == GLUT_KEY_UP){
		//andar para frente
		if(xA < 10 && xB < 10){
			xA += 1; 
			xB += 1;
		} 
		andando = 100; 	
		glutPostRedisplay();	
	} else if(key == GLUT_KEY_DOWN){
		//andar para tras
		if(xA > -10 && xB > -10) {
			xA -= 1;  
			xB -= 1;
		}
		andando = 100; 
		glutPostRedisplay();	
	} else if(key == GLUT_KEY_LEFT){
		//rodar
		alfa -= 10; 
		glutPostRedisplay();
	} else if(key == GLUT_KEY_RIGHT){
		//rodar
		alfa += 10;
		glutPostRedisplay();
	}
}

/**
 * @desc Função de callback para reshape.
 * @param {int} w Nova larguraAda Aanela.
 * @param {int} h Nova altura da janela.
 */
void reshapeCallback(int w, int h){
	/** Atualiza os valores da janela */
	width = w;
	height = h;
	/** Define o volume de vista */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv){
	/** Passo 1: Inicializa funções GLUT */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Duas viewports");
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	tempo_ant = glutGet(GLUT_ELAPSED_TIME);
	/** Passo 2: Registra callbacks da OpenGl */
	glutDisplayFunc(displayCallback);
	glutIdleFunc(anima);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(keyboard_func);
	glutKeyboardFunc(neblina); 

	/** Passo 3: Executa o programa */
	glutMainLoop();
	return 0;
}
