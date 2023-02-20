#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <glut.h>

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

int WIDTH = 1280;
int HEIGHT = 650;

GLuint tex;
char title[] = "3D Model Loader Sample";

char* p0s[20];

bool s = true;
double r = 0.01;
int rd = 1;

int goalKeeperFlag = 0;
int playerBallFlag = 0;
float goalKeeperX = 0;
float playerBallX = 0;
float playerBallZ = 0;
float ballZ = 0;
boolean entered = false;
boolean pressedX = false;
boolean firstPerson = false;
boolean firstFrame = true;

float ballTX = 0;
float ballTZ = 0;
int rotDegee = -180;
int px = 0;
int py = 1;
int pz = 0;
boolean saved = false;
void Timer3(int value);

void Timer(int value);
void myDisplay2();
void myDisplay1();
void print(float x, float y, char* string);

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;


	Camera(float eyeX = 0.5f, float eyeY = 0.6f, float eyeZ = 2.0f,
		float centerX = 0.5f, float centerY = 0.5f, float centerZ = 0.0f,
		float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}


	void top() {
		eye = Vector3f(0.4, 2.5, 0.0);
		center = Vector3f(0.0, 0.0, 0.0);
		up = Vector3f(0.0, 1.0, 0.0);
	}

	void left() {
		eye = Vector3f(0.5, 0.6, 2.0);
		center = Vector3f(0.5, 0.5, 0.0);
		up = Vector3f(0.0, 1.0, 0.0);
	}

	void right() {
		eye = Vector3f(0.5, 0.6, -2.0);
		center = Vector3f(0.5, 0.5, 0.0);
		up = Vector3f(0.0, 1.0, 0.0);
	}

	void front() {
		eye = Vector3f(2.4, 0.5, 0.0);
		center = Vector3f(0.5, 0.5, 0.0);
		up = Vector3f(0.0, 1.0, 0.0);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;


void Timer(int value) {
	glClearColor(0, 0, 0, 0);
	glutPostRedisplay();
	glutDisplayFunc(myDisplay2);
	goalKeeperFlag = 0;
	playerBallFlag = 0;
	goalKeeperX = 0;
	playerBallX = 0;
	playerBallZ = 0;
	ballZ = 0;
	entered = false;
	pressedX = false;
	rotDegee = -180;
	ballTX = 0;
	ballTZ = 0;
	px = 0;
	py = 1;
	pz = 0;
	firstFrame = false;
	camera.eye.x = 0.5f; camera.eye.y = 0.6f; camera.eye.z = 2.0f;
	camera.center.x = 0.5f; camera.center.z = 0.5f; camera.center.z = 0.0f;
	camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
	saved = false;
}


void Timer2(int value) {
	glutPostRedisplay();
	glutDisplayFunc(myDisplay1);
	goalKeeperFlag = 0;
	playerBallFlag = 0;
	goalKeeperX = 0;
	playerBallX = 0;
	playerBallZ = 0;
	ballZ = 0;
	entered = false;
	pressedX = false;
	saved = false;
	rotDegee = -180;
	ballTX = 0;
	ballTZ = 0;
	px = 0;
	py = 1;
	pz = 0;
	camera.eye.x = 0.5f; camera.eye.y = 0.6f; camera.eye.z = 2.0f;
	camera.center.x = 0.5f; camera.center.z = 0.5f; camera.center.z = 0.0f;
	camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
}

void Timer3(int value) {
	glutPostRedisplay();
	glutDisplayFunc(myDisplay2);
	goalKeeperFlag = 0;
	playerBallFlag = 0;
	goalKeeperX = 0;
	playerBallX = 0;
	playerBallZ = 0;
	ballZ = 0;
	entered = false;
	pressedX = false;
	saved = false;
	rotDegee = -180;
	ballTX = 0;
	ballTZ = 0;
	px = 0;
	py = 1;
	pz = 0;
	camera.eye.x = 0.5f; camera.eye.y = 0.6f; camera.eye.z = 2.0f;
	camera.center.x = 0.5f; camera.center.z = 0.5f; camera.center.z = 0.0f;
	camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

int cameraZoom = 0;

// Model Variables of environment 1
Model_3DS model_ball;
Model_3DS model_player;
Model_3DS model_net;
Model_3DS model_keeper;
Model_3DS model_flag;
Model_3DS model_stands;
Model_3DS model_cone;
Model_3DS model_banner;
Model_3DS model_manSitting;
Model_3DS model_light;

// Model Variables of environment 2
Model_3DS model_player2;
Model_3DS model_keeper2;
Model_3DS model_cup;
Model_3DS model_rack;
Model_3DS model_manSitting2;
Model_3DS model_obstacle;

// Textures of environment 1
GLTexture tex_ground;
GLTexture tex_score;
GLTexture tex_banner;

// Textures of environment 2
GLTexture tex_ground2;
GLTexture tex_score2;
GLTexture tex_banner2;


void RenderPitchGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0.5, 0.0, 0.0);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0.5, 0.03, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5, 0.03, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5, 0.03, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5, 0.03, 1);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderPitchGround2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground2.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0.5, 0.0, 0.0);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0.5, 0.03, -1);
	glTexCoord2f(1, 0);
	glVertex3f(-0.5, 0.03, -1);
	glTexCoord2f(1, 1);
	glVertex3f(-0.5, 0.03, 1);
	glTexCoord2f(0, 1);
	glVertex3f(0.5, 0.03, 1);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderScore()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_score.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0.2, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3f(0.5, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0.5, 0.5, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 0.5, 0);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();


	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderScore2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_score2.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0.2, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3f(0.5, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0.5, 0.5, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 0.5, 0);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();


	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}


void RenderBanner()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_banner.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(0.635, 0.03, 0.15);
	glLineWidth(40);
	glBegin(GL_LINES);
	glVertex3f(0.3, 0.17, 0.2);
	glVertex3f(0.315, 0, 0.2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(0.635, 0.03, 0.35);
	glLineWidth(40);
	glBegin(GL_LINES);
	glVertex3f(0.3, 0.17, 0.2);
	glVertex3f(0.315, 0, 0.2);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.9, 0.03, 0.3);
	glRotated(-10, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3f(0.3, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0.3, 0.2, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 0.2, 0);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderBanner2()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_banner2.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(0.635, 0.03, 0.15);
	glLineWidth(40);
	glBegin(GL_LINES);
	glVertex3f(0.3, 0.17, 0.2);
	glVertex3f(0.315, 0, 0.2);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(0.635, 0.03, 0.35);
	glLineWidth(40);
	glBegin(GL_LINES);
	glVertex3f(0.3, 0.17, 0.2);
	glVertex3f(0.315, 0, 0.2);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.9, 0.03, 0.3);
	glRotated(-10, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	glBegin(GL_QUADS);
	glTexCoord2d(1, 0);
	glVertex3f(0.3, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0.3, 0.2, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 0.2, 0);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.
	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void lightAnim() {

	//	setupLightAnimation();

	glPushMatrix();
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glTranslated(0.8, 0.38, -0.5);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
	glPopMatrix();


	glPushMatrix();
	GLUquadricObj* sphere1 = gluNewQuadric();
	gluQuadricNormals(sphere1, GLU_SMOOTH);
	glTranslated(0.8, 0.38, 0.8);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere1, r, 50, 50);
	gluDeleteQuadric(sphere1);
	glPopMatrix();


	glPushMatrix();
	GLUquadricObj* sphere2 = gluNewQuadric();
	gluQuadricNormals(sphere2, GLU_SMOOTH);
	glTranslated(0.2, 0.38, 0.8);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere2, r, 50, 50);
	gluDeleteQuadric(sphere2);
	glPopMatrix();



	glPushMatrix();
	GLUquadricObj* sphere3 = gluNewQuadric();
	gluQuadricNormals(sphere3, GLU_SMOOTH);
	glTranslated(0.2, 0.38, -0.6);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere3, r, 50, 50);
	gluDeleteQuadric(sphere3);
	glPopMatrix();


}
void lightAnim2() {

	//	setupLightAnimation();

	glPushMatrix();
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glTranslated(0.8, 0.37, 0);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere, r, 50, 50);
	gluDeleteQuadric(sphere);
	glPopMatrix();


	glPushMatrix();
	GLUquadricObj* sphere1 = gluNewQuadric();
	gluQuadricNormals(sphere1, GLU_SMOOTH);
	glTranslated(0.8, 0.37, 0.8);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere1, r, 50, 50);
	gluDeleteQuadric(sphere1);
	glPopMatrix();


	glPushMatrix();
	GLUquadricObj* sphere2 = gluNewQuadric();
	gluQuadricNormals(sphere2, GLU_SMOOTH);
	glTranslated(0.2, 0.37, 0);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere2, r, 50, 50);
	gluDeleteQuadric(sphere2);
	glPopMatrix();



	glPushMatrix();
	GLUquadricObj* sphere3 = gluNewQuadric();
	gluQuadricNormals(sphere3, GLU_SMOOTH);
	glTranslated(0.2, 0.37, -0.6);
	glColor3f(1, 0.9, 0);
	gluSphere(sphere3, r, 50, 50);
	gluDeleteQuadric(sphere3);
	glPopMatrix();


}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay1(void)
{
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);


	if (0.5 + goalKeeperX <= 0.7 && goalKeeperFlag == 0)
	{
		goalKeeperX += 0.0008;
	}
	else {
		goalKeeperFlag = 1;
	}

	if (0.5 + goalKeeperX >= 0.3 && goalKeeperFlag == 1) {
		goalKeeperX -= 0.0008;

	}
	else {
		goalKeeperFlag = 0;
	}

	if (0.65 + ballZ + playerBallZ > -0.9 && pressedX == true) {
		ballZ -= 0.001;
	}
	if (0.65 + ballZ + playerBallZ <= -0.9 && pressedX == true && saved == false) {
		glClearColor(1, 1, 1, 0);
		glutPostRedisplay();
		sprintf((char*)p0s, "GOAL");
		print(0.4, 0.8, (char*)p0s);
		glFlush();
		glutTimerFunc(3000, Timer, 0);
	}


	if (-0.48 <= playerBallZ && playerBallZ <= -0.44 && 0.06 <= playerBallX && playerBallX <= 0.14) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		glutTimerFunc(2000, Timer2, 0);
	}

	if (-0.88 <= playerBallZ && playerBallZ <= -0.84 && -0.24 <= playerBallX && playerBallX <= -0.18) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		glutTimerFunc(2000, Timer2, 0);
	}
	if (-0.9 <= playerBallZ && playerBallZ <= -0.86 && 0.25 <= playerBallX && playerBallX <= 0.30) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		glutTimerFunc(2000, Timer2, 0);
	}

	if (0.65 + ballZ + playerBallZ <= -0.75 && 0.65 + ballZ + playerBallZ >= -0.8 && 0.4 + playerBallX + ballTX <= 0.55 + goalKeeperX && 0.4 + playerBallX + ballTX >= 0.45 + goalKeeperX) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		saved = true;
		glutTimerFunc(2000, Timer2, 0);
	}


	lightAnim();

	glPushMatrix();
	RenderPitchGround();
	glPopMatrix();


	glPushMatrix();
	glTranslated(playerBallX, 0, playerBallZ);
	//ball
	glPushMatrix();
	glTranslated(ballTX, 0, ballTZ);
	glTranslated(0.4, 0.06, 0.65 + ballZ);
	glScaled(0.03, 0.03, 0.03);
	model_ball.Draw();
	glPopMatrix();
	//player
	glPushMatrix();
	glTranslated(0.4, 0.04, 0.7);
	glScaled(0.005, 0.005, 0.005);
	glRotated(rotDegee, px, py, pz);
	model_player.Draw();
	glPopMatrix();
	glPopMatrix();


	//keeper
	glPushMatrix();
	glTranslated(0.5 + goalKeeperX, 0.04, -0.75);
	glScaled(0.005, 0.005, 0.005);
	model_keeper.Draw();
	glPopMatrix();


	//net
	glPushMatrix();
	glTranslated(0.50, 0.28, -0.87);
	glScaled(0.0015, 0.0025, 0.0015);
	glRotated(-90, 0, 1, 0);
	model_net.Draw();
	glPopMatrix();

	//flag1
	glPushMatrix();
	glTranslated(0.85, 0.22, -0.78);
	glScaled(0.00000125, 0.0000007, 0.0000007);
	glRotated(-180, 0, 1, 0);
	model_flag.Draw();
	glPopMatrix();

	//flag2
	glPushMatrix();
	glTranslated(0.15, 0.22, -0.78);
	glScaled(0.00000125, 0.0000007, 0.0000007);
	model_flag.Draw();
	glPopMatrix();

	//stands
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(-0.18, 0.37, 0);
	glScaled(0.0003, 0.0007, 0.0003);
	glRotated(90, 0, 1, 0);
	model_stands.Draw();
	glPopMatrix();


	//cone1
	glPushMatrix();
	glTranslated(0.5, 0.03, 0.2);
	glScaled(0.0001, 0.0001, 0.0001);
	model_cone.Draw();
	glPopMatrix();

	//cone2
	glPushMatrix();
	glTranslated(0.7, 0.03, -0.2);
	glScaled(0.0001, 0.0001, 0.0001);
	model_cone.Draw();
	glPopMatrix();

	//cone3
	glPushMatrix();
	glTranslated(0.2, 0.03, -0.2);
	glScaled(0.0001, 0.0001, 0.0001);
	model_cone.Draw();
	glPopMatrix();


	//score board
	glPushMatrix();
	RenderScore();
	glPopMatrix();

	//banner1
	glPushMatrix();
	RenderBanner();
	glPopMatrix();

	//banner2
	glPushMatrix();
	glTranslated(0, 0, -0.7);
	RenderBanner();
	glPopMatrix();


	//manSitting1
	glPushMatrix();
	glTranslated(-0.12, 0.2, 0.1);
	glScaled(0.003, 0.003, 0.005);
	glRotated(-180, 0, 1, 0);
	model_manSitting.Draw();
	glPopMatrix();

	//manSitting2
	glPushMatrix();
	glTranslated(-0.12, 0.2, -0.3);
	glScaled(0.003, 0.003, 0.005);
	glRotated(-180, 0, 1, 0);
	model_manSitting.Draw();
	glPopMatrix();

	//light 1
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.85, 0.25, -0.5);
	glScaled(0.002, 0.002, 0.002);
	model_light.Draw();
	glPopMatrix();

	//light 2
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.85, 0.25, 0.8);
	glScaled(0.002, 0.002, 0.002);
	model_light.Draw();
	glPopMatrix();

	//light 3
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.15, 0.25, 0.8);
	glScaled(0.002, 0.002, 0.002);
	glRotated(-180, 0, 1, 0);
	model_light.Draw();
	glPopMatrix();

	//light 4
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.15, 0.25, -0.6);
	glScaled(0.002, 0.002, 0.002);
	glRotated(-180, 0, 1, 0);
	model_light.Draw();
	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();
}

void myDisplay2(void)
{
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);


	if (0.5 + goalKeeperX <= 0.7 && goalKeeperFlag == 0)
	{
		goalKeeperX += 0.0016;
	}
	else {
		goalKeeperFlag = 1;
	}

	if (0.5 + goalKeeperX >= 0.3 && goalKeeperFlag == 1) {
		goalKeeperX -= 0.0016;

	}
	else {
		goalKeeperFlag = 0;
	}

	if (0.45 + ballZ + playerBallZ > -0.9 && pressedX == true) {
		ballZ -= 0.001;
	}

	if (0.65 + ballZ + playerBallZ <= -0.7 && pressedX == true && saved == false) {
		glClearColor(1, 1, 1, 0);
		glutPostRedisplay();
		sprintf((char*)p0s, "You Win");
		print(0.4, 0.8, (char*)p0s);
		glFlush();
	}

	if (-0.25 <= playerBallX && playerBallX <= -0.10 && -0.47 <= playerBallZ && playerBallZ <= -0.34) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		glutTimerFunc(2000, Timer3, 0);
	}
	if (0.2 <= playerBallX && playerBallX <= 0.35 && -0.75 <= playerBallZ && playerBallZ <= -0.62) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		glutTimerFunc(2000, Timer3, 0);
	}
	if (0.65 + ballZ + playerBallZ <= -0.62 && 0.65 + ballZ + playerBallZ >= -0.67 && 0.4 + playerBallX + ballTX <= 0.55 + goalKeeperX && 0.4 + playerBallX + ballTX >= 0.45 + goalKeeperX) {
		rotDegee = -90;
		px = 1;
		py = 0;
		pz = 0;
		saved = true;
		glutTimerFunc(2000, Timer3, 0);
	}


	lightAnim2();

	glPushMatrix();
	RenderPitchGround2();
	glPopMatrix();


	glPushMatrix();
	glTranslated(playerBallX, 0, playerBallZ);
	//ball
	glPushMatrix();
	glTranslated(ballTX, 0, ballTZ);
	glTranslated(0.4, 0.06, 0.45 + ballZ);
	glScaled(0.03, 0.03, 0.03);
	model_ball.Draw();
	glPopMatrix();
	//player
	glPushMatrix();
	glTranslated(0.4, 0.04, 0.5);
	glScaled(0.005, 0.005, 0.005);
	glRotated(rotDegee, px, py, pz);
	model_player2.Draw();
	glPopMatrix();
	glPopMatrix();


	//keeper
	glPushMatrix();
	glTranslated(0.5 + goalKeeperX, 0.04, -0.75);
	glScaled(0.005, 0.005, 0.005);
	model_keeper2.Draw();
	glPopMatrix();


	//net
	glPushMatrix();
	glTranslated(0.50, 0.28, -0.87);
	glScaled(0.0015, 0.0025, 0.0015);
	glRotated(-90, 0, 1, 0);
	model_net.Draw();
	glPopMatrix();

	//flag1
	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0.85, 0.22, -0.78);
	glScaled(0.00000125, 0.0000007, 0.0000007);
	glRotated(-180, 0, 1, 0);
	model_flag.Draw();
	glPopMatrix();

	//flag2
	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0.15, 0.22, -0.78);
	glScaled(0.00000125, 0.0000007, 0.0000007);
	model_flag.Draw();
	glPopMatrix();

	//stands
	glPushMatrix();
	glColor3f(0.6, 0.6, 0.6);
	glTranslated(-0.18, 0.37, 0);
	glScaled(0.0003, 0.0007, 0.0003);
	glRotated(90, 0, 1, 0);
	model_stands.Draw();
	glPopMatrix();

	//score board
	glPushMatrix();
	RenderScore2();
	glPopMatrix();


	//cup
	glPushMatrix();
	glTranslated(0.9, 0.355, -0.92);
	glScaled(0.0003, 0.0001, 0.0004);
	model_cup.Draw();
	glPopMatrix();

	//rack
	glPushMatrix();
	glTranslated(0.9, 0.03, -0.87);
	glScaled(0.01, 0.01, 0.01);
	model_rack.Draw();
	glPopMatrix();

	//banner1
	glPushMatrix();
	RenderBanner2();
	glPopMatrix();

	//banner2
	glPushMatrix();
	glTranslated(0, 0, -0.7);
	RenderBanner2();
	glPopMatrix();


	//manSitting1
	glPushMatrix();
	glTranslated(-0.26, 0.22, 0.3);
	glScaled(0.003, 0.003, 0.005);
	glRotated(90, 0, 1, 0);
	model_manSitting2.Draw();
	glPopMatrix();

	//manSitting2
	glPushMatrix();
	glTranslated(-0.26, 0.22, -0.32);
	glScaled(0.003, 0.003, 0.005);
	glRotated(90, 0, 1, 0);
	model_manSitting2.Draw();
	glPopMatrix();

	//obstacle1
	glPushMatrix();
	glTranslated(0.25, 0, 0.1);
	glScaled(0.006, 0.006, 0.005);
	model_obstacle.Draw();
	glPopMatrix();

	//obstacle2
	glPushMatrix();
	glTranslated(0.7, 0, -0.2);
	glScaled(0.006, 0.006, 0.005);
	model_obstacle.Draw();
	glPopMatrix();


	//light 1
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.85, 0.25, 0);
	glScaled(0.002, 0.002, 0.002);
	model_light.Draw();
	glPopMatrix();

	//light 2
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.85, 0.25, 0.8);
	glScaled(0.002, 0.002, 0.002);
	model_light.Draw();
	glPopMatrix();

	//light 3
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.15, 0.25, 0);
	glScaled(0.002, 0.002, 0.002);
	glRotated(-180, 0, 1, 0);
	model_light.Draw();
	glPopMatrix();

	//light 4
	glPushMatrix();
	glColor3d(255, 215, 0);
	glTranslated(0.15, 0.25, -0.6);
	glScaled(0.002, 0.002, 0.002);
	glRotated(-180, 0, 1, 0);
	model_light.Draw();
	glPopMatrix();


	glutPostRedisplay();
	glutSwapBuffers();
}


//=======================================================================
// Keyboard Function
//=======================================================================
void Keyboard(unsigned char key, int x, int y) {
	float d = 0.1;
	float a = 3.0;
	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 'i':
		camera.rotateX(a);
		break;
	case 'k':
		camera.rotateX(-a);
		break;
	case 'l':
		camera.rotateY(-a);
		break;
	case 'j':
		camera.rotateY(a);
		break;
	case 'n':
		if (pressedX == false) {
			playerBallX += 0.01;
			rotDegee = 90;
			ballTX = 0.05;
			ballTZ = 0.05;
			camera.moveX(-0.01);
		}
		break;
	case 'v':
		if (pressedX == false) {
			playerBallX -= 0.01;
			rotDegee = -90;
			ballTX = -0.05;
			ballTZ = 0.05;
			camera.moveX(0.01);
		}
		break;
	case 'g':
		if (pressedX == false) {
			playerBallZ -= 0.01;
			rotDegee = -180;
			ballTX = 0;
			ballTZ = 0;
			camera.moveZ(0.01);
		}
		break;
	case 'b':
		if (pressedX == false) {
			playerBallZ += 0.01;
			rotDegee = 0;
			ballTX = 0;
			ballTZ = 0.1;
			camera.moveZ(-0.01);
		}
		break;
	case 'x':
		pressedX = true;
		break;
	case '1':
		if (firstFrame) {
			camera.eye.x = 0.4f + playerBallX; camera.eye.y = 0.35f; camera.eye.z = 0.6f + playerBallZ;
			camera.center.x = 0.4f; camera.center.z = 0.25f; camera.center.z = -1.4f + playerBallZ;
			camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
		}
		else {
			camera.eye.x = 0.4f + playerBallX; camera.eye.y = 0.35f; camera.eye.z = 0.35f + playerBallZ;
			camera.center.x = 0.4f; camera.center.z = 0.25f; camera.center.z = -1.65f + playerBallZ;
			camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
		}
		break;
	case '3':
		camera.eye.x = 0.5f + playerBallX; camera.eye.y = 0.6f; camera.eye.z = 2.0f + playerBallZ;
		camera.center.x = 0.5f + playerBallX; camera.center.z = 0.5f; camera.center.z = 0.0f + playerBallZ;
		camera.up.x = 0.0f; camera.up.y = 1.0f; camera.up.z = 0.0f;
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}
	glutPostRedisplay();
}

//Mouse Function
void myMouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			camera.moveZ(0.1);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			camera.moveZ(-0.1);
		}
		break;
	default:
		break;
	}
}



void Special(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		camera.top();
		break;
	case GLUT_KEY_DOWN:
		camera.front();
		break;
	case GLUT_KEY_LEFT:
		camera.left();
		break;
	case GLUT_KEY_RIGHT:
		camera.right();
		break;
	}

	glutPostRedisplay();
}



//=======================================================================
// Assets Loading Function
//=======================================================================

void LoadAssets()
{
	// Loading Model files of environment 1
	model_ball.Load("Models/Pallone/Ball 3DS.3ds");
	model_player.Load("Models/player/Man footballer Messi N241113.3ds");
	model_net.Load("Models/net/Gate.3ds");
	model_keeper.Load("Models/player/Man footballer Messi N241113.3ds");
	model_flag.Load("Models/flags/flag^stok.3DS");
	model_stands.Load("Models/stands/Tribune sports N290116.3ds");
	model_cone.Load("Models/cones/Traffic_cone N031209.3ds");
	model_cup.Load("Models/cup/Trophy.3ds");
	model_banner.Load("Models/banner/Banner N180808.3DS");
	model_light.Load("Models/light/Lamppost 2.3ds");

	// Loading Model files of environnent 2
	model_player2.Load("Models/player2/Man footballer Messi N241113.3ds");
	model_keeper2.Load("Models/player2/Man footballer Messi N241113.3ds");
	model_rack.Load("Models/rack/Stand 1.3DS");
	model_manSitting.Load("Models/manSitting/Man keyboard N080814.3DS");
	model_manSitting2.Load("Models/manSitting2/Man sitting N200514.3DS");
	model_obstacle.Load("Models/obstacle/Simulator Box Man N170915.3ds");


	// Loading texture files of environment 1
	tex_ground.Load("Textures/247113.bmp");
	loadBMP(&tex, "Textures/247113.bmp", true);
	tex_score.Load("Textures/score.bmp");
	loadBMP(&tex, "Textures/score.bmp", true);
	tex_banner.Load("Textures/soccer-template-design-football-banner-sport-layout-design-illustration-vector.bmp");
	loadBMP(&tex, "Textures/soccer-template-design-football-banner-sport-layout-design-illustration-vector.bmp", true);

	// Loading texture files of environment 2
	tex_ground2.Load("Textures/ground2.bmp");
	loadBMP(&tex, "Textures/ground2.bmp", true);
	tex_score2.Load("Textures/score2.bmp");
	loadBMP(&tex, "Textures/score2.bmp", true);
	tex_banner2.Load("Textures/1000_F_208942624_dgTMHz3zixodsZkVcvpM6OyhzfYKwwOQ.bmp");
	loadBMP(&tex, "Textures/1000_F_208942624_dgTMHz3zixodsZkVcvpM6OyhzfYKwwOQ.bmp", true);


}

void print(float x, float y, char* string)
{
	int len, i;

	glRasterPos2f(x, y);

	len = (int)strlen(string);

	for (i = 0; i < len; i++)
	{

		glScaled(4, 0, 0);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void Anim() {

	// game timer 
//	timer -= 0.0005;

	// light animation 

	r += 0.00005 * rd;
	if (r < 0.009 || r>0.02)
		rd *= -1;



	glutPostRedisplay();
}
//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);


	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(50, 50);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay1);

	glutMouseFunc(myMouse);
	glutIdleFunc(Anim);

	glutKeyboardFunc(Keyboard);

	glutSpecialFunc(Special);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}