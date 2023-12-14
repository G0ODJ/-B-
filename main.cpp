#include<iostream>
#include<vector>
#include<GL/freeglut.h>

using namespace std;

struct point{
	double x, y;
};

double X[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 0.0, 1.0, 2.0, 3.0 };
double Y[] = { 6.0, 0.0, 6.0, 0.0, 6.0, 0.0, 6.0, 6.0, 0.0, 6.0, 0.0 };
vector<point> pts;


void init(void)
{
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void display(void)
{
	int i;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 6.0, 0.0, 6.0); //窗口坐标范围

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glColor3f(0.0f, 1.0f, 0.0f); //绿色
	for (i = 0; i <= 6; i++) //7条水平线
	{
		glBegin(GL_LINES);
		glVertex2d(0.0, i*1.0);
		glVertex2d(10.0, i*1.0);
		glEnd();
	}
	glBegin(GL_LINES); //7条竖线
	for (i = 0; i <= 6; i++)
	{
		glVertex2d(i*1.0, 0.0);
		glVertex2d(i*1.0, 10.0);
	}
	glEnd();

	//在对角线画点
	glColor3f(1.0f, 1.0f, 1.0f); //白色
	glPointSize(10.0f); //点大小
	glBegin(GL_POINTS);
	for (i = 0; i < 7; i++)
		glVertex2d(X[i], Y[i]);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < pts.size(); i++)
	{
		glVertex2d(pts[i].x, pts[i].y);
	}
	glEnd();


	glFlush();
}

double GetBaseFunVal(double u, int i, int k, vector<double> m_aNode)
{
	double Val = 0.0;
	double Val1 = 0.0;
	double Val2 = 0.0;
	if (k == 0)
	{
		if (u >= m_aNode[i] && u < m_aNode[i + 1])
			return 1.0;
		else
		{
			return 0.0;
		}
	}
	if (k>0)
	{
		if (u < m_aNode[i] || u > m_aNode[i + k + 1])
			return 0.0;
		else
		{
			double alpha = 0.0;
			double beta = 0.0;
			double dTemp = 0.0;
			dTemp = m_aNode[i + k] - m_aNode[i];
			if (dTemp == 0.0)
			{
				alpha = 0;
			}
			else
				alpha = (u - m_aNode[i]) / dTemp;

			dTemp = m_aNode[i + k + 1] - m_aNode[i + 1];
			if (dTemp == 0.0)
			{
				beta = 0.0;
			}
			else
				beta = (m_aNode[i + k + 1] - u) / dTemp;
			Val1 = alpha * GetBaseFunVal(u, i, k - 1, m_aNode);
			Val2 = beta * GetBaseFunVal(u, i + 1, k - 1, m_aNode);
			Val = Val1 + Val2;
		}
	}
	return Val;
}


int main(int argc, char **argv)
{
	int n(6);//控制点个数
	//原来程序 int n(7);
	int k(3);//阶数

	//均匀
	//vector<double> U{ 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

	//准均匀（端点处连接）
	//vector<double> U{ 0.0, 0.0, 0.0, 0.0, 1 / 4.0, 2 / 4.0, 3 / 4.0, 1.0, 1.0, 1.0, 1.0 };

	//尖角
	//vector<double> U{ 0, 1.0 / 8, 2.0 / 8, 3.0 / 8, 4.0 / 8, 4.0 / 8, 4.0 / 8, 5.0 / 8, 6.0 / 8, 7.0 / 8, 1.0 };


	//循环控制点
	n += 3;
	vector<double> U{ -3 / 7.0, -2 / 7.0, -1 / 7.0, 
		0.0, 1 / 7.0, 2 / 7.0, 3 / 7.0, 4 / 7.0, 5 / 7.0, 6 / 7.0, 1.0, 
		8 / 7.0, 9 / 7.0, 10 / 7.0 };





	point pt;


	for (int i = k; i <= n; i++)
		for (double u = U[i]; u <= U[i + 1]; u += 0.01)
		{
		pt.x = 0;
		pt.y = 0;
		for (int j = 0; j <= n; j++)
		{
			pt.x += X[j] * GetBaseFunVal(u, j, k, U);
			pt.y += Y[j] * GetBaseFunVal(u, j, k, U);
		}
		pts.emplace_back(pt);
		}




	//画图象
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("bSpline");
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	system("pause");
}