#include <GL/glut.h>
#include "draw.h"

void readImage(char* fname, int m)
{
	FILE* pfile = NULL;
	fopen_s(&pfile, fname, "rb");
	if (pfile == 0) exit(0);
	//读取图像大小

	fseek(pfile, 0x0012, SEEK_SET);
	fread(&image[m].imagewidth, sizeof(image[m].imagewidth), 1, pfile);
	fread(&image[m].imageheight, sizeof(image[m].imageheight), 1, pfile);
	//计算像素数据长度
	image[m].pixellength = image[m].imagewidth * 3;
	while (image[m].pixellength % 4 != 0) image[m].pixellength++; // 补充数据，直到pixellength是的倍数
	image[m].pixellength *= image[m].imageheight;
	//读取像素数据
	image[m].pixeldata = (GLubyte*)malloc(image[m].pixellength);
	if (image[m].pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(image[m].pixeldata, image[m].pixellength, 1, pfile);

	//关闭文件
	fclose(pfile);

}

void draw_Table()
{

	glEnable(GL_TEXTURE_2D);
	//桌子
	draw_Cube(-15.0, 15.0, -2.0, -0.0, -15.0, 15.0, 4);
	draw_Cube(11.0, 13.0, -15.0, -2.0, 11.0, 13.0, 4);
	draw_Cube(-11.0, -13.0, -15.0, -2.0, 11.0, 13.0, 4);
	draw_Cube(11.0,  13.0, -15.0, -2.0, -11.0, -13.0, 4);
	draw_Cube(-11.0,-13.0, -15.0, -2.0, -11.0, -13.0, 4);
	//椅子
	draw_Cube(2.5,-2.5, -15.0, -5.0, -20.0, -25.0, 4);
	draw_Cube(2.5,-2.5, -15.0, -5.0, 20.0, 25.0, 4);


	glDisable(GL_TEXTURE_2D);


	glEnable(GL_TEXTURE_2D);	
	//草地
	draw_Cube(-30.0, 80.0, -15.0, -17.0, -80.0, 30.0, 5);

	glDisable(GL_TEXTURE_2D);
}

void draw_Tower()
{

	glEnable(GL_TEXTURE_2D);
	//底座
	draw_Cube(50.0, 80.0, -10.0, -15.0, -45.0, -15.0,1);
	//第一层柱子
	draw_Cube(57.0, 60.0, 5.0, -10.0, -23.0, -20.0,2);
	draw_Cube(57.0, 60.0, 5.0, -10.0, -40.0, -37.0,2);
	draw_Cube(70.0, 73.0, 5.0, -10.0, -40.0, -37.0,2);
	draw_Cube(70.0, 73.0, 5.0, -10.0, -23.0, -20.0,2);
	//第二层
	draw_Trapezoid(50.0, 80.0, 5.0, 10.0, -15.0, -45.0, 5.0,3);
	//第二层柱子
	draw_Cube(57.0, 60.0, 10.0, 25.0, -23.0, -20.0, 2);
	draw_Cube(57.0, 60.0, 10.0, 25.0, -40.0, -37.0, 2);
	draw_Cube(70.0, 73.0, 10.0, 25.0, -40.0, -37.0, 2);
	draw_Cube(70.0, 73.0, 10.0, 25.0, -23.0, -20.0,2);
	//顶部
	draw_Triangle(50.0,80.0,65.0,25.0,35.0,-45.0,-15.0,-30.0,3);

	glDisable(GL_TEXTURE_2D);
}

void  init()
{
	LSystemRule();
	glClearColor(0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-80.0, 80.0, -80.0, 80.0, -80.0, 80.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < 6; i++)
	{
		//读取图像文件
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		readImage((char*)filename[i], i);

		glGenTextures(1, &textName[i]);
		glBindTexture(GL_TEXTURE_2D, textName[i]);
		//加载纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[i].imagewidth, image[i].imageheight, 0,
			GL_BGR_EXT, GL_UNSIGNED_BYTE, image[i].pixeldata);

		//设置纹理过滤
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//设置纹理环境
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  //纹理
	}

	glDisable(GL_BLEND);
}

void  display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glLoadIdentity();
	glColor4f(1.0, 1.0, 1.0, 1.0);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	draw_background();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	glRotatef(xangle, 1.0, 0.0, 0.0);
	glRotatef(yangle, 0.0, 1.0, 0.0);//旋转
	glScalef(enlarge, enlarge, enlarge);
	drawManyTree();
	draw_Table();
	draw_Tower();
	//draw_axis();

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	GLfloat f = 80.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-f, f, -f*(GLfloat)h / (GLfloat)w,
		f*(GLfloat)h / (GLfloat)w, -f, f);
	else
		glOrtho(-f*(GLfloat)w / (GLfloat)h,
		f*(GLfloat)w / (GLfloat)h, -f, f, -f, f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int btn, int state, int x, int y)
{
	if ((btn == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
		oldx = x; oldy = y;
	}
}

void motion(int x, int y)
{
		GLint deltax = oldx - x;
		GLint deltay = oldy - y;
		yangle += 360 * (GLfloat)deltax / (GLfloat)WindW;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
		xangle += 360 * (GLfloat)deltay / (GLfloat)WindH;
		oldx = x;//记录此时的鼠标坐标，更新鼠标坐标,否则旋转将不可控 
		oldy = y;
		glutPostRedisplay();
}

void key(unsigned char key, GLint x, GLint y)
{
	if (key == 'x')
		enlarge *= 2;
	if (key == 'X') 
		enlarge *= 2;
	if (key == 'z') 
		enlarge /= 2;
	if (key == 'Z') 
		enlarge /= 2;
	display();
	glutPostRedisplay();
}

int  main(int  argc, char  **argv)
{
	glutInit(&argc, argv);    
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Blooming Tree");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);  
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(key);

	glutMainLoop(); 
	return 0; 
}
