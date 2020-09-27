#include <freeglut.h> 
#include <math.h>
#include<time.h>
#include<string>

char title[] = "160313037 - UTKU ERDEMÝR";
double xr = 0;
double yr = 0;
double xx = 0.0;
double yy = -2.3;
float* enemiesX;
int* enemiesTypes;
float* enemiesY;
bool* show;
int height = 500;
int width = 2 * height / 3;
bool pause = false;
bool stop = false;
int score = 0;
int health = 0;
int counter = 2;
bool mActive = false;
std::string temp_message;
GLfloat qaBlack[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat qaRed[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat qaBlue[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat qaBlue2[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat qaBlue3[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat qaRed2[] = { 1.0, 0.3, 0.3, 1.0 };
GLfloat qaRed3[] = { 0.7, 0.0, 0.1, 1.0 };
int temp_type;
void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);             
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); 
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaSpecularLight);

	GLfloat qaLightPosition[] = { 1.0, 0.0, 7.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
}
void drawSphere() {
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluSphere(quadratic,0.05,20,20);
}
void printScore()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
	std::string message = "Puan:" + std::to_string(score);
	glRasterPos3f(1.0, 2.0,-6.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)message.c_str());
	if (mActive)
	{
		message = "Fare Aktif.\n\nDegistirme:m";
	}
	else {
		message = "Klayve aktif.\n\nDegistirme:m ";
	}
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue3);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
	glRasterPos3f(0.8, 1.7, -6.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)message.c_str());
	if (pause)
	{
		glRasterPos3f(-0.3, 0.5, -6.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Oyun durduruldu.");
	}
	if (stop)
	{
		glRasterPos3f(-0.3, 0.5, -6.0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Oyun bitti.");
	}
	message = "Can:";
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
	glRasterPos3f(-1.3, 2.0, -6.0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)message.c_str());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
}
void generateEnemyCoords()
{
	static float  xcoords[5];
	static int types[5];
	static float  ycoords[5];
	static bool tempShow[5];
	srand((unsigned)time(NULL));
	for (int i = 0; i < 5; ++i)
	{
		int op;
		op = rand() % 10 + 1;
		if (op % 2 == 0)
		{
			xcoords[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - 0.1)));
		}
		else if (op % 2 == 1)
		{
			xcoords[i] = -1 * (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (3.0 - 0.1))));
		}
		types[i] = rand() % 4 + 1;
		tempShow[i] = false;
		ycoords[i] = 3.5;
	}
	enemiesTypes = types;
	show = tempShow;
	enemiesX = xcoords;
	enemiesY = ycoords;
}
void changeEnemyYCoords()
{
	if (!pause && !stop)
	{
		for (int i = 0; i < 5; i++)
		{
			if (i == 0 || *(enemiesY + i - 1) <= 2.0 && *(enemiesY + i) >= -50.0)
			{
				*(show + i) = true;
				if (*(enemiesTypes + i) == 1)
				{
					*(enemiesY + i) -= 0.05;
				}
				else if (*(enemiesTypes + i) == 2)
				{
					*(enemiesY + i) -= 0.05375;
				}
				else if (*(enemiesTypes + i) == 3)
				{
					*(enemiesY + i) -= 0.0525;
				}
				else if (*(enemiesTypes + i) == 4)
				{
					*(enemiesY + i) -= 0.05435;
				}
			}
		}
	}
}
void drawHealth()
{
	for (int j = 0; j < health; j++)
	{
		glLoadIdentity();
		glPushMatrix();
		glTranslatef(-1.0f+float(j)/8.0f, 2.35f, -7.0f);
		drawSphere();
		glPopMatrix();
	}
}
void checkHealth()
{
	if (counter % 6000 == 0 && health < 2)
	{
		health++;
	}
	drawHealth();
}
void changeEnemyXCoords()
{
	if (!pause && !stop)
	{
		for (int i = 0; i < 5; i++)
		{
			int op;
			op = rand() % 10 + 1;
			if ((*(enemiesX + i) +0.3) >= (*(enemiesX + (i+1)) && (*(enemiesX + i)) <= (*(enemiesX + (i + 1)))))
			{
				if (*(enemiesX + i)>=0)
				{
					*(enemiesX + i) -= 0.30;
				}
				else {
					*(enemiesX + i) += 0.30;
				}
			}
			if (op % 2 == 0)
			{
				if (*(enemiesX + i) >= 0.7)
				{
					*(enemiesX + i) -= (float)(rand() % 10) / 30.0;
				}
				else
				{
					*(enemiesX + i) += (float)(rand() % 10) / 30.0;
				}
			}
			else if (op % 2 == 1)
			{
				if (*(enemiesX + i) >= -0.7)
				{
					*(enemiesX + i) -= (float)(rand() % 10) / 30.0;
				}
				else
				{
					*(enemiesX + i) += (float)(rand() % 10) / 30.0;
				}
			}
		}
	}
}
void keyUp(int key, int x, int y) 
{
	if (!pause && !stop && !mActive)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			if (xx < 1.00)
			{
				xx += 0.05;
			}
			break;
		case GLUT_KEY_LEFT:
			if (xx > -1.65)
			{
				xx -= 0.05;
			}
			break;
		case GLUT_KEY_UP:
			if (yy < 0.0)
			{
				yy += 0.05;
			}
			break;
		case GLUT_KEY_DOWN:
			if (yy > -2.3)
			{
				yy -= 0.05;
			}
			break;
		}
	}
}
void drawTrianglePrism(float xz, float yz)
{
	glTranslatef(xz, yz, -7.0f);
	glRotatef(315.0, 1.0f, 0.0f, 0.0f);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue2);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_QUADS);
	glVertex3f(0.65625, 0, 0.25);
	glVertex3f(0.65625, 0, -0.25);
	glVertex3f(-0.0, 0, -0.25);
	glVertex3f(-0.0, 0, 0.25);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue3);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glVertex3f(0.65625, 0, -0.25);
	glVertex3f(0.65625, 0.65, -0.25);
	glVertex3f(-0.0, 0.65, -0.25);
	glVertex3f(-0.0, 0, -0.25);


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glVertex3f(0.65625, 0.65, -0.25);
	glVertex3f(-0.0, 0.65, -0.25);
	glVertex3f(-0.0, 0, 0.25);
	glVertex3f(0.65625, 0, 0.25);
	glEnd();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.65625, 0, 0.25);
	glVertex3f(0.65625, 0.65, -0.25);
	glVertex3f(0.65625, 0, -0.25);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	glVertex3f(-0.0, 0, 0.25);
	glVertex3f(-0.0, 0.65, -0.25);
	glVertex3f(-0.0, 0, -0.25);
	glEnd();
}
void drawCube(float xz, float yz)
{
	glTranslatef(xz, yz, -7.0f);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	   glVertex3f(0.65625, -0.0, -0.15);
	   glVertex3f(0.65625, 0.65, -0.15);
	   glVertex3f(-0.0, 0.65, -0.15);
	   glVertex3f(-0.0, -0.0, -0.15);

	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.0, 0.15);
	glVertex3f(0.65625, 0.65, 0.15);
	glVertex3f(-0.0, 0.65, 0.15);
	glVertex3f(-0.0, -0.0, 0.15);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.0, -0.15);
	glVertex3f(0.65625, 0.65, -0.15);
	glVertex3f(0.65625, 0.65, 0.15);
	glVertex3f(0.65625, -0.0, 0.15);
	glEnd();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.0, -0.0, 0.15);
	glVertex3f(-0.0, 0.65, 0.15);
	glVertex3f(-0.0, 0.65, -0.15);
	glVertex3f(-0.0, -0.0, -0.15);
	glEnd();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, 0.65, 0.15);
	glVertex3f(0.65625, 0.65, -0.15);
	glVertex3f(-0.0, 0.65, -0.15);
	glVertex3f(-0.0, 0.65, 0.15);
	glEnd();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.0, -0.15);
	glVertex3f(0.65625, -0.0, 0.15);
	glVertex3f(-0.0, -0.0, 0.15);
	glVertex3f(-0.0, -0.0, -0.15);
	glEnd();
}
void drawRectanglePrism()
{
	glTranslatef(xx, yy, -7.0f);
	glBegin(GL_LINE_STRIP);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);
	if (xx>=-0.3)
	{
		glVertex2f(-1.65 - xx, 0.0);
		glVertex2f(-1.50 - xx / 2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.75 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.60 - xx / 2.0, 0.00);
		glVertex2f(-0.45 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.30 - xx / 2.0, 0.00);
		glVertex2f(-0.15 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	else if (xx >= -0.6)
	{
		glVertex2f(-1.50-xx/2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.75 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.60 - xx / 2.0, 0.00);
		glVertex2f(-0.45 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.30 - xx / 2.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	else if (xx > -0.9)
	{
		glVertex2f(-1.50 - xx / 2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.75 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.60 - xx / 2.0, 0.00);
		glVertex2f(-0.45 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	else if (xx > -1.2)
	{
		glVertex2f(-1.50 - xx / 2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.75 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.60 - xx / 2.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	else if (xx > -1.5)
	{
		glVertex2f(-1.50 - xx / 2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.75 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	else if (xx > -1.8)
	{
		glVertex2f(-1.50 - xx / 2.0, 0.00);
		glVertex2f(-1.35 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-1.20 - xx / 2.0, 0.00);
		glVertex2f(-1.05 - xx / 2.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.90 - xx / 2.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
		glVertex2f(-0.0, (1.0 - xx) / 5.5);
		glVertex2f(-0.0, 0.00);
	}
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaRed);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);

	glVertex3f(0.65625, -0.15, -0.15);
	glVertex3f(0.65625, 0.15, -0.15);
	glVertex3f(-0.0, 0.15, -0.15);
	glVertex3f(-0.0, -0.15, -0.15);

	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.15, 0.15);
	glVertex3f(0.65625, 0.15, 0.15);
	glVertex3f(-0.0, 0.15, 0.15);
	glVertex3f(-0.0, -0.15, 0.15);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.15, -0.15);
	glVertex3f(0.65625, 0.15, -0.15);
	glVertex3f(0.65625, 0.15, 0.15);
	glVertex3f(0.65625, -0.15, 0.15);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.0, -0.15, 0.15);
	glVertex3f(-0.0, 0.15, 0.15);
	glVertex3f(-0.0, 0.15, -0.15);
	glVertex3f(-0.0, -0.15, -0.15);
	glEnd();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue2);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, 0.15, 0.15);
	glVertex3f(0.65625, 0.15, -0.15);
	glVertex3f(-0.0, 0.15, -0.15);
	glVertex3f(-0.0, 0.15, 0.15);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaBlue3);
	glMaterialf(GL_FRONT, GL_SHININESS, 220.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.65625, -0.15, -0.15);
	glVertex3f(0.65625, -0.15, 0.15);
	glVertex3f(-0.0, -0.15, 0.15);
	glVertex3f(-0.0, -0.15, -0.15);
	glEnd();
}
void draw_cylinder(GLfloat radius,
	GLfloat height,
	float xz, float yz, int a)
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;
	glTranslatef(xz, yz, -7.0f);
	if (a==3)
	{
		glRotatef(120.0, 1.0f, 1.0f, 1.0f);
	}
	if (a==4)
	{
		glRotatef(90.0, 1.0f, 0.0f, 0.0f);
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed3);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * 3.14) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT, qaRed2);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * 3.14) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();
}
bool enemiesFinished()
{
	bool temp = true;
	for (int i = 0; i < 5; i++)
	{
		if (*(enemiesY + i) >= -2.3)
		{
			temp = false;
		}
	}
	return temp;
}
void checkCrashed()
{
	for (int i = 0; i < 5; i++)
	{
		if (*(enemiesY + i) >= -2.3)
		{
			if ((yy<= *(enemiesY + i) && yy + 0.30 >= *(enemiesY + i)) || (yy <= *(enemiesY + i)+0.30 && yy + 0.30 >= *(enemiesY + i)+0.30) ||  (yy <= *(enemiesY + i) + 0.65 && yy + 0.30 >= *(enemiesY + i) + 0.65))
			{
				if ((xx <= *(enemiesX + i) && xx + 0.65625 >= *(enemiesX + i)) || (xx <= *(enemiesX + i) + 0.30 && xx + +0.65625 >= *(enemiesX + i) + 0.30) || (xx <= *(enemiesX + i) + 0.65625 && xx + 0.65625 >= *(enemiesX + i) + 0.65625))
				{
					*(enemiesY + i) = -50.0;
					if (*(enemiesTypes + i) == 1)
					{
						score += 3;
					}
					if (*(enemiesTypes + i) == 2)
					{
						score += 4;
					}
					if (*(enemiesTypes + i) == 3)
					{
						score += 5;
					}
					if (*(enemiesTypes + i) == 4)
					{
						score += 6;
					}
				}
			}
			else if (*(enemiesTypes + i) == 4 && (yy <= *(enemiesY + i) -0.35 && yy + 0.30 >= *(enemiesY + i) - 0.35)) {
				if ((xx <= *(enemiesX + i) && xx + 0.65625 >= *(enemiesX + i)) || (xx <= *(enemiesX + i) + 0.30 && xx + +0.65625 >= *(enemiesX + i) + 0.30) || (xx <= *(enemiesX + i) + 0.65625 && xx + 0.65625 >= *(enemiesX + i) + 0.65625))
				{
					*(enemiesY + i) = -50.0;
					score += 6;
				}
			}
		}
		else if(*(enemiesY + i) > -50.0)
		{
			*(enemiesY + i) = -50.0;
			if (*(enemiesTypes + i) == 1)
			{
				score -= 3;
			}
			if (*(enemiesTypes + i) == 2)
			{
				score -= 4;
			}
			if (*(enemiesTypes + i) == 3)
			{
				score -= 5;
			}
			if (*(enemiesTypes + i) == 4)
			{
				score -= 6;
			}
			if (score < 0)
			{
				if (health>0)
				{
					health--;
				}
				else 
				{
					stop = true;
				}
			}
		}
	}
}
void display() {
	if (enemiesFinished())
	{
		generateEnemyCoords();
	}
	else
	{
		changeEnemyYCoords();
		changeEnemyXCoords();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	for (int i = 0; i < 5; i++)
	{
		glLoadIdentity();
		glPushMatrix();
		if (*(show + i))
		{
			if (*(enemiesTypes + i) == 1)
			{
				drawTrianglePrism(*(enemiesX + i), *(enemiesY + i));
			}
			if (*(enemiesTypes + i) == 2)
			{
				drawCube(*(enemiesX + i), *(enemiesY + i));
			}
			if (*(enemiesTypes + i) == 3)
			{
				draw_cylinder(0.328125f, 0.65f, *(enemiesX + i), *(enemiesY + i), 3);
			}
			if (*(enemiesTypes + i) == 4)
			{
				draw_cylinder(0.328125f, 0.65f, *(enemiesX + i), *(enemiesY + i), 4);
			}
		}
		glPopMatrix();
	}
	checkHealth();
	printScore();
	drawRectanglePrism();
	checkCrashed();
	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0f, aspect, 0.1f, 10.0f);
}
void timer(int)
{
	glutSwapBuffers();
	glutPostRedisplay();
	counter++;
	glutTimerFunc(10000.0 / 60.0, timer, 0);
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 13:
		if (stop)
		{
			stop = false;
			generateEnemyCoords();
			score = 0;
		}
		break;
	case 'p':
		pause = !pause;
		break;
	case 'm':
		mActive = !mActive;
		break;
	case 27:
		exit(0);
		break;
	}
}

void motion(int x, int y)
{
	if (!pause && !stop && mActive)
	{
		xx = -1.65 + ((1.0 + 1.65) / width) * x;
		yy = -2.3 - (-2.3 + ((0.0 + 2.3) / (height / 2))* (y - (height / 2)));
		if (yy > 0)
		{
			yy = 0;
		}
	}
}
int main(int argc, char** argv) {
	generateEnemyCoords();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow(title);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(motion);
	glutSpecialFunc(keyUp);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(1000.0 / 60.0, timer, 0);
	initGL();
	glutMainLoop();
	return 0;
}