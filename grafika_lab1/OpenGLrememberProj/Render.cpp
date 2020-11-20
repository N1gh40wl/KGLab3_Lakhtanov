

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <corecrt_math.h>
#include <iostream>

#define PI 3.14159265

double vectorAngle(double a1, double b1, double c1, double a2, double b2, double c2) {
	double sclrmul = (a1 * a2) + (b1 * b2) + (c1 * c2);
	double mod1 = sqrt( a1 * a1 + b1 * b1 + c1 * c1);
	double mod2 = sqrt( a2 * a2 + b2 * b2 + c2 * c2);
	double cosa = sclrmul / (mod1 * mod2);
	double angle = acos(cosa) * 180.0 / PI;
	//if (angle > 50) angle = 100-angle;
	//angle = angle - 90;
	std::cout << angle<< "/n";
	return angle;
}


double length3d(double a1, double b1, double c1, double a2, double b2, double c2)
{
	double res;
	res = sqrt(((a2 - a1) * (a2 - a1)) + ((b2 - b1) * (b2 - b1)) + ((c2 - c1) * (c2 - c1)));
	return res;
}

double  normalize(double a1, double b1, double c1, int num)
{
	double locLength = length3d(0, 0, 0, a1, b1, c1);
	double inv_length = (1 / locLength);
	double x = a1 * inv_length;
	double y = b1 * inv_length;
	double z = c1 * inv_length;
	if (num == 0) return x;
	else if (num == 1) return y;
	else if (num == 2) return z;
}

double vector3d(double a1, double b1, double c1, double a2, double b2, double c2, int num) 
{
	if (num == 0) return a2 - a1;
	else if (num == 1) return b2 - b1;
	else if (num == 2) return c2 - c1;
}





double f(double p1, double p2, double p3, double p4, double t)
{
	//return p1 * (1 - t) * (1 - t) + 2 * p2 * t * (1 - t) + p3 * t * t; //ïîñ÷èòàííàÿ ôîðìóëà
	return (1 - t) * (1 - t) * (1 - t) * p1 + 3 * t * (1 - t) * (1 - t) * p2 + 3 * t * t * (1 - t) * p3 + t * t * t * p4;
}


double fder(double p1, double p2, double p3, double p4, double t){
	return (-3 * p1 + 6 * p1 * t - 3 * p1 * t * t + 3 * p2 - 12 * p2 * t + 9 * p2 * t * t + 6 * p3 * t - 9 * p3 * t * t + 3 * p4 * t * t);
}

double fErmit(double p1, double p4, double r1, double r4, double t)
{
	return p1 * (2 * t * t * t -3 * t * t + 1) + p4 * (-2 * t * t * t + 3 * t * t) + r1 * (t * t * t - 2 * t * t + t) + r4*(t * t * t - t * t);
}
double factorial(double num) {
	double res=1;
	for (int i = 1; i <= num; i++) {
		res *= i;
	}
	return res;
}
double Bern(double n, double i , double u ) {
	return ((factorial(n)/(factorial(i)* factorial(n-i)))*pow(u,i)*pow((1-u),(n-i)));
}

double f3d( double P[4][4][3], double u, double v,int num) {
	double res=0;
	double P2[3] = { P[1][3][0],P[1][3][1],P[1][3][2] };
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			res += Bern(3, i, u) * Bern(3, j, v) * P[i][j][num];
		}
	}
	return res;
}



double t_max =0;
int flag = 0;

void plane() {
	double A1[] = { 0,1,0 };
	double A2[] = { -0.2,0.8,0 };
	double A3[] = { -0.2,-0.8,0 };
	double A4[] = { 0,-1,0 };
	double A5[] = { 0.2,-0.8,0 };
	double A6[] = { 0.2,0.8,0 };

	double B1[] = { 0.2,0.2,0 };
	double B2[] = { 0.2,-0.2,0 };
	double B3[] = { 1,-0.2,0 };
	double B4[] = { 1,0,0 };

	double B5[] = { -0.2,0.2,0 };
	double B6[] = {-0.2,-0.2,0 };
	double B7[] = { -1,-0.2,0 };
	double B8[] = { -1,0,0 };

	double C1[] = { 0,-1,0.4 };
	double C2[] = { 0,-0.8,0.4 };
	double C3[] = { 0,-0.6,0 };
	double C4[] = { 0,-1,0 };


	glColor3d(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3dv(A1);
	glVertex3dv(A2);
	glVertex3dv(A3);
	glVertex3dv(A4);

	glVertex3dv(A4);
	glVertex3dv(A5);
	glVertex3dv(A6);
	glVertex3dv(A1);

	glColor3d(1, 0, 1);
	glVertex3dv(B1);
	glVertex3dv(B2);
	glVertex3dv(B3);
	glVertex3dv(B4);

	glColor3d(0, 1, 1);
	glVertex3dv(B5);
	glVertex3dv(B6);
	glVertex3dv(B7);
	glVertex3dv(B8);

	glColor3d(1, 1, 0);
	glVertex3dv(C1);
	glVertex3dv(C2);
	glVertex3dv(C3);
	glVertex3dv(C4);

	glEnd();

}


void Render(double delta_time)
{

	if (flag == 0) t_max += delta_time / 10; //t_max ñòàíîâèòñÿ = 1 çà 5 ñåêóíä
	if (t_max > 1) flag = 1;// ôëàæîê )
	if (flag == 1) t_max -= delta_time / 10; //èäåì íàçàä
	if (t_max < 0) flag = 0;// ôëàæîê )	

	//ÊÐÈÂÀß ÁÅÇÜÅ
	double P1[] = { 0,0,0 }; //Íàøè òî÷êè, ìàññèâ÷èê double
	double P2[] = { 2,4,20 };
	double P3[] = { 15,2,2 };
	double P4[] = { -5,10,4 };

	double Q1[] = { 0,0, 0 };
	double Q2[] = { 1, -1, 0 };
	double Q3[] = { 1,  1, 0 };
	double Q4[] = { -1,  1, 0 };

	double P[3];
	double PP[3];

	double Q11[3];
	double Q12[3];

	P[0] = f(P1[0], P2[0], P3[0], P4[0], t_max);
	P[1] = f(P1[1], P2[1], P3[1], P4[1], t_max);
	P[2] = f(P1[2], P2[2], P3[2], P4[2], t_max);

	double t_res = 0.001;

	PP[0] = f(P1[0], P2[0], P3[0], P4[0], t_max + t_res);
	PP[1] = f(P1[1], P2[1], P3[1], P4[1], t_max + t_res);
	PP[2] = f(P1[2], P2[2], P3[2], P4[2], t_max + t_res);

	while (length3d(P[0], P[1], P[2], PP[0], PP[1], PP[2]) <= 1) {
		PP[0] = f(P1[0], P2[0], P3[0], P4[0], t_max + t_res);
		PP[1] = f(P1[1], P2[1], P3[1], P4[1], t_max + t_res);
		PP[2] = f(P1[2], P2[2], P3[2], P4[2], t_max + t_res);
		t_res += 0.001;
	}


	//glColor3d(1, 0, 0);
	//glBegin(GL_QUADS);
	//
	//
	//
	//	Q1[0] = f(P12[0], P22[0], P32[0], P42[0], t_max);
	//	Q1[1] = f(P12[1], P22[1], P32[1], P42[1], t_max);
	//	Q1[2] = f(P12[2], P22[2], P32[2], P42[2], t_max);
	//	glVertex3dv(Q1);
	//
	//	Q2[0] = f(P11[0], P21[0], P31[0], P41[0], t_max);
	//	Q2[1] = f(P11[1], P21[1], P31[1], P41[1], t_max);
	//	Q2[2] = f(P11[2], P21[2], P31[2], P41[2], t_max);
	//	glVertex3dv(Q2);
	//							  
	//	Q3[0] = f(P11[0], P21[0], P31[0], P41[0], (t_max + t_res));
	//	Q3[1] = f(P11[1], P21[1], P31[1], P41[1], (t_max + t_res));
	//	Q3[2] = f(P11[2], P21[2], P31[2], P41[2], (t_max + t_res));
	//	glVertex3dv(Q3);
	//
	//	Q4[0] = f(P12[0], P22[0], P32[0], P42[0], (t_max + t_res));
	//	Q4[1] = f(P12[1], P22[1], P32[1], P42[1], (t_max + t_res));
	//	Q4[2] = f(P12[2], P22[2], P32[2], P42[2], (t_max + t_res));
	//	glVertex3dv(Q4);
	//
	//
	//glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINE_STRIP); //ïîñòðîèì îòðåçêè P1P2 è P2P3
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glEnd();
	glLineWidth(3); //øèðèíà ëèíèè
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += 0.001)
	{
		P[0] = f(P1[0], P2[0], P3[0], P4[0], t);
		P[1] = f(P1[1], P2[1], P3[1], P4[1], t);
		P[2] = f(P1[2], P2[2], P3[2], P4[2], t);
		glVertex3dv(P); //Ðèñóåì òî÷êó P
	}
	glEnd();



	

	glColor3d(1, 0, 1);
	glLineWidth(1); //âîçâðàùàåì øèðèíó ëèíèè = 1
	//íàðèñóåì âñå òî÷êè
	glPointSize(10);
	//glBegin(GL_POINTS);
	//glVertex3dv(P);
	//glEnd();
	glColor3d(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P4);
	glEnd();
	//ÊÎÍÅÖ ÊÐÈÂÎÉ ÁÅÇÜÅ

	// ÀÍÈÌÀÖÈß ÑÀÌÎËÅÒÈÊÀ



	PP[0] = f(P1[0], P2[0], P3[0], P4[0], t_max + t_res);
	PP[1] = f(P1[1], P2[1], P3[1], P4[1], t_max + t_res);
	PP[2] = f(P1[2], P2[2], P3[2], P4[2], t_max + t_res);
	double Vec[3];


	//Vec[0] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 0);
	//Vec[1] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 1);
	//Vec[2] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 2);

	//Vec[0] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 0);
	//Vec[1] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 1);
	//Vec[2] = vector3d(P[0], P[1], P[2], PP[0], PP[1], PP[2], 2);

	P[0] = fder(P1[0], P2[0], P3[0], P4[0],t_max);
	P[1] = fder(P1[1], P2[1], P3[1], P4[1],t_max);
	P[2] = fder(P1[2], P2[2], P3[2], P4[2],t_max);

	Vec[0] =normalize(P[0], P[1], P[2], 0);
	Vec[1] =normalize(P[0], P[1], P[2], 1);
	Vec[2] =normalize(P[0], P[1], P[2], 2);


	



	P[0] = f(P1[0], P2[0], P3[0], P4[0], t_max);
	P[1] = f(P1[1], P2[1], P3[1], P4[1], t_max);
	P[2] = f(P1[2], P2[2], P3[2], P4[2], t_max);
	int angle_key = 0;


	double angle = vectorAngle(0, 1, 0, Vec[0], Vec[1], 0);
	if (Vec[0] < 0) { 
		if (angle_key == 0)
			angle_key = 1;
	else if (angle_key == 1)
			angle_key = 0;  };
	if (angle_key == 1) angle = angle * 1;
	if (angle_key == 0) angle = angle * -1;
	glPushMatrix();
	glTranslated(P[0], P[1], P[2]);
	glRotated(angle, 0, 0, 1);//ðûñêàíüå
	//angle_key = 0;
	//angle = vectorAngle(1, 0, 0, Vec[0], 0, Vec[2]);
	//if (Vec[2] < 0) {
	//	if (angle_key == 0)
	//		angle_key = 1;
	//	else if (angle_key == 1)
	//		angle_key = 0;
	//};
	//if (angle_key == 1) angle = angle * 1;
	//if (angle_key == 0) angle = angle * -1;
	//
	//glRotated(angle, 0, 1, 0);//êðåí


	

	angle_key = 1;
	angle = vectorAngle(0, 1, 0,0, Vec[1], Vec[2]);
	if (Vec[2] < 0) {
		if (angle_key == 0)
			angle_key = 1;
		else if (angle_key == 1)
			angle_key = 0;
	};
	if (angle_key == 1) angle = angle * 1;
	if (angle_key == 0) angle = angle * -1;
	glRotated(angle, 1, 0, 0);//òàíãàæ
	//glColor3d(1, 0, 0);
	plane();
	// ÊÎÍÅÖ ÀÍÈÌÀÖÈÈ ÑÀÌÎËÅÒÈÊÀ

	//ÍÀ×ÀËÎ ÊÐÈÂÎÉ ÝÐÌÈÒÀ
	glPopMatrix();
	glPushMatrix();
	glTranslated(4, 0, 0);
	double E1[] = { 0,0,0 };
	double E4[] = { 2,4,5 };
	double R1[] = { 1,0,1 };
	double R4[] = { 0,-10,1 };
	glColor3d(1, 0, 0);

	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1; t += 0.001)
	{
		P[0] = fErmit(E1[0], E4[0], R1[0], R4[0], t);
		P[1] = fErmit(E1[1], E4[1], R1[1], R4[1], t);
		P[2] = fErmit(E1[2], E4[2], R1[2], R4[2], t);
		glVertex3dv(P); //Ðèñóåì òî÷êó P
	}
	glEnd();


	glColor3d(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3dv(E1);
	glVertex3dv(R1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3dv(E4);
	glVertex3dv(R4);
	glEnd();
	glColor3d(0, 1, 0);
	glBegin(GL_POINTS);
	glVertex3dv(E1);
	glVertex3dv(E4);
	glEnd();
	// ÊÎÍÅÖ ÊÐÈÂÎÉ ÝÐÌÈÒÀ
	glPopMatrix();

	// ÍÀ×ÀËÎ ÏÎÂÅÐÕÍÎÑÒÈ ÁÅÇÜÅ
	glPushMatrix();
	glTranslated(8, 0, 2.5);
	double Points[4][4][3] = {
		{{0,9,1}, {3,9,0}, {6,9,0}, {9,9,1}},
		{{0,6,0}, {3,6,0}, {6,6,0}, {9,6,0}},
		{{0,3,0}, {3,3,0}, {6,3,-4}, {9,3,0}},
		{{0,0,1}, {3,0,0}, {6,0,0}, {9,0,1}},
	};
	int i = 0;
	int j = 0;
	glPointSize(6);
	glColor3d(0, 0, 0);
	glBegin(GL_POINTS);
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			glVertex3dv(Points[i][j]);
		}
		glVertex3dv(Points[i][j]);
	}
	glEnd();

	i = 0;
	j = 0;
	glColor3d(0, 0, 0);
	
	while (i<4) {
		glBegin(GL_LINE_STRIP);
		while (j<4){
			glVertex3dv(Points[i][j]);
			j += 1;
		}
		j = 0;
		i += 1;
		glEnd();
	}
	i = 0;
	j = 0;
	while (i < 4) {
		glBegin(GL_LINE_STRIP);
		while (j < 4) {
			glVertex3dv(Points[j][i]);
			j += 1;
		}
		j = 0;
		i += 1;
		glEnd();
	}



	glPointSize(4);
	glColor3d(0, 1, 1);
	glBegin(GL_POINTS);
	double Points2[4][4][3];
	 i = 0;
	 j = 0;
	for (double u = 0; u <= 1; u += 0.1) {// Ðèñóåì ïîâåðõíîñòü òî÷êàìè
		j = 0;
		for (double v = 0; v <= 1; v += 0.1) {
			P[0] = f3d(Points,u,v,0);
			P[1] = f3d(Points, u, v, 1);
			P[2] = f3d(Points, u, v, 2);
			glVertex3dv(P); //Ðèñóåì òî÷êó P
			//Points2[i][j][0] = P[0];
			//Points2[i][j][1] = P[1];
			//Points2[i][j][2] = P[2];
			j += 1;
		}
		i += 1;
	}
	glEnd();
	glPopMatrix();

	glTranslated(8, 0, 0.5);
	glColor3d(1, 1, 0);


	double u = 0;
	double v = 0;
	glColor3d(1,0, 0);

	while (u <= 1) {
		glBegin(GL_LINE_STRIP);
		while (v <= 1) {
			P[0] = f3d(Points, u, v, 0);
			P[1] = f3d(Points, u, v, 1);
			P[2] = f3d(Points, u, v, 2);
			glVertex3dv(P); //Ðèñóåì òî÷êó P
			v += 0.1;
		}
		v = 0;
		u += 0.1;
		glEnd();
	}
	u = 0;
	v = 0;
	while (u <= 1) {
		glBegin(GL_LINE_STRIP);
		while (v <= 1) {
			P[0] = f3d(Points, v,u, 0);
			P[1] = f3d(Points, v,u, 1);
			P[2] = f3d(Points, v,u, 2);
			glVertex3dv(P); //Ðèñóåì òî÷êó P
			v += 0.1;
		}
		v = 0;
		u += 0.1;
		glEnd();
	}

	// ÊÎÍÅÖ ÏÎÂÅÐÕÍÎÑÒÈ ÁÅÇÜÅ
}
	



