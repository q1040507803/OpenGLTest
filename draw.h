#include "image.h"
#include "3D-tree.h"

GLint WindW = 800;
GLint WindH = 600;
GLfloat oldx, oldy;//鼠标移动因子
GLfloat xangle = 15.0f;
GLfloat yangle = -15.0f;//旋转因子  
GLfloat enlarge = 1.0;

GLfloat PI = 3.14;
string way[3]; //提供生成规则 
string rule, temprule;

float len; //单步长
float len1;
float px = 0.0, py = 0.0;
//float Alphax;
float Alphay;
float Alphaz;
int k;   //树干树
int n;  //迭代次数

Node   stack[150];
int    stackpointer;

static GLuint textName[6];
const char* filename[6] = { "data\\background.bmp", "data\\stone.bmp", "data\\wall.bmp" ,"data\\tile.bmp","data\\floor.bmp","data\\grass.bmp" };
Image image[6];



void axis(double length)
{
	//draw a z-axis with cone at end
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length);
	glEnd();
	glTranslated(0, 0, length - 0.2);
	glutWireCone(0.04, 0.2, 12, 9);
	glPopMatrix();
}

void draw_axis()
{
	glPushMatrix();

	glColor3d(1.0, 0.0, 1.0);
	axis(20.0); //z-axis
	glRasterPos3f(0.0, 0.0, 2.3);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');

	glColor3d(0.0, 1.0, 0.0);
	glRotated(-90, 1.0, 0.0, 0.0);
	axis(20.0); //y-axis
	glRasterPos3f(0.0, 0.0, 2.3);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');

	glColor3d(0.0, 1.0, 0.0);
	glRotated(90, 0.0, 1.0, 0.0);
	axis(20.0); //x-axis
	glRasterPos3f(0.0, 0.0, 2.3);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');

	glPopMatrix();
}


void draw_Cube(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2,int num)
{//由立方体组成，可知六个参数即可构造八个顶点  
	int i, j;
	GLfloat vertex[8][3] = {
		//八个顶点，从v1-v8  
		x1,y1,z2,  //0
		x2,y1,z2,  //1
		x1,y2,z2,  //2
		x2,y2,z2,  //3
		x1,y1,z1,  //4
		x2,y1,z1,  //5
		x1,y2,z1,  //6
		x2,y2,z1,  //7
	};
	GLint surface[6][4] = {
		//v1对应0,以此类推  
		0,1,3,2,  //front
		4,5,7,6,  //back
		2,3,7,6,  //top
		0,1,5,4,  //bottom
		0,2,6,4,  //left
		1,3,7,5   //right
	};

	//将每个立方体分成六个面绘制 
	for (i = 0; i < 6; i++)
	{
	
		glBindTexture(GL_TEXTURE_2D, textName[num]);
		glBegin(GL_POLYGON);//指定绘制方式，填充多边形 GL_POLYGON 不填充多边形 GL_LINE_LOOP

		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex[surface[i][0]]);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex[surface[i][1]]);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex[surface[i][2]]);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertex[surface[i][3]]);
		glEnd();
	}

}

void draw_Trapezoid(GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2, GLfloat t,int num)
{//由立方体组成，可知六个参数即可构造八个顶点  
	int i, j;
	GLfloat vertex[8][3] = {
		//八个顶点，从v1-v8  
		x1,y1,z2,  //0
		x2,y1,z2,  //1
		x1+t,y2,z2+t,  //2
		x2-t,y2,z2+t,  //3
		x1,y1,z1,  //4
		x2,y1,z1,  //5
		x1+t,y2,z1-t,  //6
		x2-t,y2,z1-t,  //7
	};
	GLint surface[6][4] = {
		//v1对应0,以此类推  
		0,1,3,2,  //front
		4,5,7,6,  //back
		2,3,7,6,  //top
		0,1,5,4,  //bottom
		0,2,6,4,  //left
		1,3,7,5   //right
	};

	//将每个立方体分成六个面绘制 
	for (i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textName[num]);
		glBegin(GL_POLYGON);//指定绘制方式，填充多边形 GL_POLYGON 不填充多边形 GL_LINE_LOOP

		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex[surface[i][0]]);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex[surface[i][1]]);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex[surface[i][2]]);
		glTexCoord2f(1.0, 0.0); glVertex3fv(vertex[surface[i][3]]);
		glEnd();
	}

}

void draw_Triangle(GLfloat x1, GLfloat x2, GLfloat x3, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2, GLfloat z3,int num)
{
	int i, j;
	GLfloat vertex[5][3] = {
		x1,y1,z2,  //0
		x2,y1,z2,  //1
		x2,y1,z1,  //2
		x1,y1,z1,  //3
		x3,y2,z3,  //4
	};
	GLint surface[4][3] = {
		1, 0, 4,
		2, 1, 4,
		3, 2, 4,
		0, 3, 4,
	};

	for (i = 0; i < 4; i++)
	{
		
		glBindTexture(GL_TEXTURE_2D, textName[num]);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex3fv(vertex[surface[i][0]]);
		glTexCoord2f(0.0, 1.0); glVertex3fv(vertex[surface[i][1]]);
		glTexCoord2f(1.0, 1.0); glVertex3fv(vertex[surface[i][2]]);
		glEnd();
	}

}

void draw_background()
{
	glBindTexture(GL_TEXTURE_2D, textName[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f( 120, -150.0, -76.8);
	glTexCoord2f(0.0, 1.0); glVertex3f( 120,  150.0, -76.8);
	glTexCoord2f(1.0, 1.0); glVertex3f(-120,  150.0, -76.8);
	glTexCoord2f(1.0, 0.0); glVertex3f(-120, -150.0, -76.8);
	glEnd();

}


void LSystemRule()
{
	//初始化
	way[0] = "FA[+X][-&X]-%XB"; //"FA[+X][-&X]-%XB";
	len = 10;
	Alphaz = 30;
	Alphay = 60;
	n = 8;
	k = 0;
	stackpointer = 0;//栈顶指针
	for (int i = 0; i < 150; i++)
	{
		stack[i].x = 0.0;
		stack[i].y = 0.0;
		stack[i].z = 0.0;
		stack[i].directionz = 0;
		stack[i].directiony = 0;
	}
	temprule = way[0];
	for (int i = 1; i <= n; i++)
	{
		int curlen = temprule.length();
		int pos = 0, j = 0;
		while (j < curlen)
		{
			if (temprule[j] == 'X')//迭代，将其中的F替换成文法模型
			{
				rule += way[0];
				j++;
				pos = rule.length() - 1;
			}
			else//保留转角
			{
				rule += temprule[j];
				pos++;
				j++;
			}
		}
		temprule = rule;
		rule.clear();
	}
	rule = temprule;//迭代好之后的文法规则
}

void drawTree()
{
	Node  Nextnode, Curnode;
	Curnode.x = 0.0;
	Curnode.y = 0.0;
	Curnode.z = 0.0;
	Curnode.directionz = 85;
	Curnode.directiony = 0;
	int length = rule.length();
	int  i = 0;

	glTranslatef(0.0, -15.0, 0.0);

	glPushMatrix();
	while (i < length)
	{
		switch (rule[i])
		{
		case 'F':
			if (len <= 0.9)
			{
				glLineWidth(1.5);

				glColor3f((float)255 / 255, (float)245 / 255, (float)240 / 255); //pear tree
			}
			else if (len > 0.9 && len < 4.0)
			{
				glLineWidth(2);
				glColor3f((float)255 / 255, (float)245 / 255, (float)240 / 255); //pear tree
				px = 0.25;
			}
			else
			{
				glLineWidth(5);
				//glColor3f((float)62/255, (float)42/255, (float)45/255);
				glColor3f((float)99 / 255, (float)81 / 255, (float)56 / 255);
				px = 0.4;
			}
			Nextnode.x = Curnode.x + len * cos(Curnode.directionz * PI / 180) * cos(Curnode.directiony * PI / 180);
			Nextnode.y = Curnode.y + len * sin(Curnode.directionz * PI / 180);
			Nextnode.z = Curnode.z - len * cos(Curnode.directionz * PI / 180) * sin(Curnode.directiony * PI / 180);
			Nextnode.directionz = Curnode.directionz;
			Nextnode.directiony = Curnode.directiony;
			glBegin(GL_LINE_LOOP);
			glVertex3f(Curnode.x, Curnode.y, Curnode.z);
			glVertex3f(Curnode.x + px, Curnode.y, Curnode.z);
			glVertex3f(Curnode.x + px, Nextnode.y, Curnode.z);
			glVertex3f(Nextnode.x, Nextnode.y, Nextnode.z);

			glEnd();
			Curnode = Nextnode;
			k++;
			break;
		case 'X':
			glColor3f((float)34 / 255, (float)139 / 255, (float)34 / 255); //green
			glLineWidth(1);
			Nextnode.x = Curnode.x + len1 * cos(Curnode.directionz * PI / 180) * cos(Curnode.directiony * PI / 180);
			Nextnode.y = Curnode.y + len1 * sin(Curnode.directionz * PI / 180);
			Nextnode.z = Curnode.z - len1 * cos(Curnode.directionz * PI / 180) * sin(Curnode.directiony * PI / 180);
			Nextnode.directionz = Curnode.directionz;
			Nextnode.directiony = Curnode.directiony;
			glBegin(GL_LINES);
			glVertex3f(Curnode.x, Curnode.y, Curnode.z);
			glVertex3f(Nextnode.x, Nextnode.y, Nextnode.z);
			glEnd();
			Curnode = Nextnode;
			break;
		case 'A':
			len = len * 7 / 10;
			len1 = len;
			break;
		case 'B':
			len = len * 10 / 7;
			break;
		case '[':
			stack[stackpointer] = Curnode;
			stackpointer++;
			break;
		case ']':
			Curnode = stack[stackpointer - 1];
			stackpointer--;
			break;
		case '+':
			Curnode.directionz = Curnode.directionz + Alphaz;
			break;
		case '-':
			Curnode.directionz = Curnode.directionz - Alphaz;
			break;
		case '&':
			Curnode.directiony = Curnode.directiony + Alphay;
			break;
		case '%':
			Curnode.directiony = Curnode.directiony - Alphay;
			break;
		default:
			;
		}
		i++;
	}
	glPopMatrix();

}

void drawManyTree()
{


	glPushMatrix();
	glTranslatef(40, 0, -50.5);
	drawTree();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(40, 0, -10.5);
	drawTree();
	glPopMatrix();

}
