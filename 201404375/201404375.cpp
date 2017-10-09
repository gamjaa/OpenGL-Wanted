//header
#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.141592

GLuint bullet, male;
char ch='1';
float bulletAngleX = 90, bulletAngleY = 0;
float bulletX = 0, bulletY = 0, bulletZ = 0;
float camX = 0, camZ = 0;
enum minmaxXYZ {minX, maxX, minY, maxY, minZ, maxZ};
float map[6], clear[6], block[6];
int stage = 0;
bool gameOver = false, gameClear = false;
GLuint texture[2];

GLuint LoadTexture( const char * filename )
{

  GLuint texture;

  int width, height;

  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 512;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}


glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR  );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
free( data );

return texture;
}

void reshape(int w,int h)
{    
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
        gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	//glOrtho(-25,25,-2,2,0.1,100);	
	glMatrixMode(GL_MODELVIEW);
}

void createCylinder(GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat radius, GLfloat h)
{
    /* function createCyliner()
    원기둥의 중심 x,y,z좌표, 반지름, 높이를 받아 원기둥을 생성하는 함수(+z방향으로 원에서 늘어남)
    centerx : 원기둥 원의 중심 x좌표
    centery : 원기둥 원의 중심 y좌표
    centerz : 원기둥 원의 중심 z좌표
    radius : 원기둥의 반지름
    h : 원기둥의 높이
    */
    GLfloat x, y, angle;
 
    glBegin(GL_TRIANGLE_FAN);           //원기둥의 윗면
    glNormal3f(0.0f, 0.0f, -1.0f);
    glColor3ub(139, 69, 19);
    glVertex3f(centerx, centery, centerz);
 
    for(angle = 0.0f; angle < (2.0f*PI); angle += (PI/8.0f))
    {
        x = centerx + radius*sin(angle);
        y = centery + radius*cos(angle);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(x, y, centerz);
    }
    glEnd();
 
    glBegin(GL_QUAD_STRIP);            //원기둥의 옆면
    for(angle = 0.0f; angle < (2.0f*PI); angle += (PI/8.0f))
    {
        x = centerx + radius*sin(angle);
        y = centery + radius*cos(angle);
        glNormal3f(sin(angle), cos(angle), 0.0f);
        glVertex3f(x, y, centerz);
        glVertex3f(x, y, centerz + h);
    }
    glEnd();
 
    glBegin(GL_TRIANGLE_FAN);           //원기둥의 밑면
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(centerx, centery, centerz + h);
    for(angle = (2.0f*PI); angle > 0.0f; angle -= (PI/8.0f))
    {
        x = centerx + radius*sin(angle);
        y = centery + radius*cos(angle);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, y, centerz + h);
    }
    glEnd();
}

void drawBullet()
{
 	glPushMatrix();
		glTranslatef(bulletX,bulletY,bulletZ);
		glRotatef(bulletAngleX,0,1,0);
		glRotatef(bulletAngleY,1,0,0);
		glutSolidSphere(0.15, 10, 10);
		createCylinder(0, 0, -0.15, 0.15, 0.5);
 	glPopMatrix();

	if(!gameOver && !gameClear)
	{
		double r = (double) bulletAngleX * PI / 180;
		bulletX += sin(r) / 2;
		bulletZ += cos(r) / 2;
		printf("%f	%f	%f\n", bulletX,bulletY,bulletZ);
	}

	/*glPushMatrix();
		// 조준점
		glTranslatef(bulletX + camX, bulletY, bulletZ + camZ);
		glutSolidSphere(0.1f,20,20);
	glPopMatrix();*/
}

void displayInit()
{
	glClearColor (0.0,0.0,0.0,1.0); 
   	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	double r = (double) bulletAngleX * PI / 180;
	camX = sin(r) * 10;
	camZ = cos(r) * 10;

	//gluLookAt(bulletX, bulletY+ 10, bulletZ, bulletX, bulletY, bulletZ, 1, 0, 0);
	// 카메라
	gluLookAt(bulletX - camX, bulletY+5, bulletZ - camZ, bulletX, bulletY, bulletZ, 0, 1, 0);

	GLfloat diffuse0[]={1.0, 1.0, 1.0, 1.0};
	GLfloat ambient0[]={1.0, 1.0, 1.0, 1.0};
	GLfloat specular0[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light0_pos[]={0.0, 100.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void renderBitmapCharacter(float x, float y, float z, void *font, char *string)
{ 
    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void display(void)
{  
   	displayInit();
	drawBullet();

	glPushMatrix();
	glColor4f(0.5, 0.5, 0.5, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
		// 왼쪽 벽	
		glVertex3f(map[minX], map[minY], map[minZ]);
		glVertex3f(map[minX], map[maxY], map[minZ]);
		glVertex3f(map[maxX], map[maxY], map[minZ]);
		glVertex3f(map[maxX], map[minY], map[minZ]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glColor4f(0.5, 0.5, 0.5, 1);
		// 오른쪽 벽
		glVertex3f(map[minX], map[minY], map[maxZ]);
		glVertex3f(map[minX], map[maxY], map[maxZ]);
		glVertex3f(map[maxX], map[maxY], map[maxZ]);
		glVertex3f(map[maxX], map[minY], map[maxZ]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 1);
		// 맞은편 벽
		glVertex3f(map[maxX], map[minY], map[minZ]);
		glVertex3f(map[maxX], map[minY], map[maxZ]);
		glVertex3f(map[maxX], map[maxY], map[maxZ]);
		glVertex3f(map[maxX], map[maxY], map[minZ]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glColor4f(0.2, 0.2, 0.2, 1);
		// 바닥
		glVertex3f(map[minX], map[minY], map[minZ]);
		glVertex3f(map[minX], map[minY], map[maxZ]);
		glVertex3f(map[maxX], map[minY], map[maxZ]);
		glVertex3f(map[maxX], map[minY], map[minZ]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	if(!gameOver && ((bulletX >= block[minX] && bulletX <= block[maxX] && bulletY >= block[minY] && bulletY <= block[maxY] && bulletZ >= block[minZ] && bulletZ <= block[maxZ])
	|| bulletX <= map[minX] || bulletX >= map[maxX] || bulletY <= map[minY] || bulletY >= map[maxY] || bulletZ <= map[minZ] || bulletZ >= map[maxZ]))
	{
		// 장애물, 맵 외곽 충돌 확인
		gameOver = true;
	}
	else if(!gameOver && (bulletX >= clear[minX] && bulletX <= clear[maxX] && bulletY >= clear[minY] && bulletY <= clear[maxY] && bulletZ >= clear[minZ] && bulletZ <= clear[maxZ]))
	{
		// 목표물 충돌 확인
		gameClear = true;
	}

	if(gameClear)
	{
		// 클리어
		glPushMatrix();
		glColor3f(1, 1, 1);
		renderBitmapCharacter(bulletX-3, bulletY-1, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "SUCCESS");
		renderBitmapCharacter(bulletX-3, bulletY-2, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "Press '1' or '2' to play each stage");
		glPopMatrix();
	}
	else if(gameOver  && stage > 0)
	{
		glPushMatrix();
		glColor3f(0, 0, 0);
		renderBitmapCharacter(bulletX-3, bulletY-1, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "FAIL");
		renderBitmapCharacter(bulletX-3, bulletY-2, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "Press 'R' to replay");
		glPopMatrix();
	}

	switch(stage)
	{
	case 0:
		// 첫 화면
		glPushMatrix();
		glColor3f(1, 1, 1);
		renderBitmapCharacter(bulletX, bulletY-1, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "WANTED");
		renderBitmapCharacter(bulletX, bulletY-2, bulletZ-1, GLUT_BITMAP_TIMES_ROMAN_24, "Press '1' or '2' to play each stage");
		glPopMatrix();
		break;
	case 1:
		// 스테이지 1
		glPushMatrix();
			glTranslatef(100, 1, 0);		
			glRotatef(90, 1, 0, 0);
			createCylinder(0, 0, 0, 5, 30);
		glPopMatrix();
		glTranslatef(150, 0, 0);
		glutSolidSphere(1.0f,20,20);
		break;
	case 2:
		// 스테이지 2
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
			glBegin(GL_QUADS);
			glColor3f(0.5, 0.5, 0.5);
				glVertex3f(block[minX], block[minY], block[minZ]);
				glVertex3f(block[minX], block[minY], block[maxZ]);
				glVertex3f(block[minX], block[maxY], block[maxZ]);
				glVertex3f(block[minX], block[maxY], block[minZ]);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		glTranslatef(150, 0, 0);
		glutSolidSphere(1.0f,20,20);
		break;
	}	

   	glutSwapBuffers(); //swap the buffers
}

// 스테이지 초기화
void stageInit(int i)
{
	bulletAngleX = 90;
	bulletAngleY = 0;
	bulletX = 0;
	bulletY = 0;
	bulletZ = 0;

	switch(i)
	{
	case 1:
		map[minX] = -10;
		map[maxX] = 160;
		map[minY]= -20;
		map[maxY] = 50;
		map[minZ] = -50;
		map[maxZ] = 50;

		block[minX] = 95;
		block[maxX] = 105;
		block[minY]= -1;
		block[maxY] = 1;
		block[minZ] = -5;
		block[maxZ] = 5;

		clear[minX] = 149;
		clear[maxX] = 151;
		clear[minY]= -1;
		clear[maxY] = 1;
		clear[minZ] = -1;
		clear[maxZ] = 1;
		break;
	case 2:
		map[minX] = -10;
		map[maxX] = 170;
		map[minY]= -20;
		map[maxY] = 50;
		map[minZ] = -50;
		map[maxZ] = 50;

		block[minX] = 100;
		block[maxX] = 101;
		block[minY]= -20;
		block[maxY] = 50;
		block[minZ] = -50;
		block[maxZ] = 15;

		clear[minX] = 149;
		clear[maxX] = 151;
		clear[minY]= -1;
		clear[maxY] = 1;
		clear[minZ] = -1;
		clear[maxZ] = 1;
		break;
	}

	gameClear = false;
	gameOver = false;
}

// 키보드 입력
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'r':
	case 'R':
		stageInit(stage);
		break;
	case '1':
		// 스테이지 1
		stage = 1;
		stageInit(stage);
		break;
	case '2':
		// 스테이지 2
		stage = 2;
		stageInit(stage);
		break;
	}
}

// 방향키 입력
void special(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		/*bulletAngleY -= 1;
		if(bulletAngleY < 0)
			bulletAngleY = 0;*/
		break;
	case GLUT_KEY_LEFT:
		bulletAngleX += 1;
		if(bulletAngleX > 180)
			bulletAngleX = 180;
		break;
	case GLUT_KEY_DOWN:
		/*bulletAngleY += 1;
		if(bulletAngleY > 180)
			bulletAngleY = 180;*/
		break; 
	case GLUT_KEY_RIGHT:
		bulletAngleX -= 1;
		if(bulletAngleX < 0)
			bulletAngleX = 0;
		break;
	}
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1280,720);
	glutCreateWindow("201404375");

	texture[0] = LoadTexture( "wall.bmp" );

	glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutIdleFunc(display);
       
	glutMainLoop();
	return 0;
}
