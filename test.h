#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define HEIGHT 45
#define WIDTH 71
#define pi 3.1415926
#define G 6.6743f
#define M 2.0f

struct point {
	float x = 0.0;
	float y = 0.0;
	float vect_x = 0.0;
	float vect_y = 0.0;

	point operator + (const point &a) {
		return point{ vect_x = vect_x + a.vect_x,	vect_y = vect_y + a.vect_y };
	}

	point operator / (const float &a) {
		return point{ vect_x = vect_x / a,	vect_y = vect_y / a};
	}

	point operator * (const float &a) {
		return point{ vect_x = vect_x * a,	vect_y = vect_y * a};
	}

	point operator - (const point &a) {
		return point{ vect_x = vect_x - a.vect_x,	vect_y = vect_y - a.vect_y };
	}

	point operator ++ (int) {
		return point{ x = x + vect_x,	y = y + vect_y };
	}

	void CalcPointPhysics(point *a, int n = 1) {
		for (int i = 0; i < n; i++) {
			float r = sqrt(((a + i)->x - this->x)*((a + i)->x - this->x) + ((a + i)->y - this->y)*((a + i)->y - this->y));
			if (r < 2.5f) {
				(a + i)->x = ((((a + i)->x - this->x) / r) * 2.5f) + this->x;
				(a + i)->y = ((((a + i)->y - this->y) / r) * 2.5f) + this->y;
			}
			(a + i)->vect_x -= (((a + i)->x - this->x) / r)*((G*M) / (r*r));
			(a + i)->vect_y -= (((a + i)->y - this->y) / r)*((G*M) / (r*r));
		}
	}

	float CalcDistPoint(point *a) {
		return (x - a->x)*(x - a->x) + (y - a->y)*(y - a->y); //R^2
	}
};

class Scene {
private:
	float zoom=1;
	char frame[HEIGHT+1][WIDTH*2+1];
	int MAX_P = 30;

	char pix(int n) {
		char a[] = { " .,:;oOQ#@" }; //to 0 from 9
		return a[n];
	}

	float Zoom(float a, float b) {
		return ((a-b)*zoom)+b;
	}	
	
	void Prin(const char *a) {
		COORD coord = { 0, 0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		puts(a);
	}

	char GetPixDraw(int x = 0, int y = 0) {
		if (((x < WIDTH) && (y < HEIGHT)) && (!(x < 0) && !(y < 0))) {
			return frame[(HEIGHT - 1) - y][x * 2];
		}
		return '\0';
	}

public:
	Scene() { //конструктор
		Format();
	}

	void Format() {
		for (int i = 0; i < HEIGHT+1; i++) {
			for (int j = 0; j < (WIDTH * 2); j++) {
				if (i < HEIGHT) {
					frame[i][j] = ' ';
				}
				else {
					frame[i][j] = '_';
				}					
			}
			frame[i][(WIDTH * 2)] = '\n';
		}
		frame[HEIGHT][WIDTH * 2 - 1] = '\0';
	}

	void ClearDraw() {
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < (WIDTH * 2); j += 2) {
					frame[i][j] = ' ';
			}
		}
	}

	void pixDro(int x = 0, int y = 0, int intensity = 9) 
	{
		if (((x < WIDTH) && (y < HEIGHT)) && (!(x < 0) && !(y < 0))) 
		{
			frame[(HEIGHT - 1) - y][x * 2] = pix(intensity);
		}
	}

	void CircleDraw(point *p, float r = 2.5f, int intensity = 9) { //небезобасное решение
		float fx = Zoom(p->x, WIDTH/2.0f), fy = Zoom(p->y, HEIGHT/2.0f), b;
		int x = fx, y = fy;
		r *= zoom;
		if (fx - x > 0.49) {
			x += 1;
		}
		if (fy - y > 0.49) {
			y += 1;
		}
		for (int i = int(y - r); i <= int(y + r); i++){
			for (int j = int(x - r); j <= int(x + r); j++){
				if (((j < WIDTH) && (i < HEIGHT)) && (!(j < 0) && !(i < 0)))
				{
					b = ((i - fy)*(i - fy)) + ((j - fx)*(j - fx));

					if (b < (r*r))
					{
						frame[(HEIGHT - 1) - i][j * 2] = pix(intensity);
					}
					//else if (b - (r*r) < 1.0f)				//делал для сглаживанния краёв, но из-за артефактов решил убрать
					//{
					//	if (GetPixDraw(j * 2, (HEIGHT - 1) - i)!='@') {
					//		frame[(HEIGHT - 1) - i][j * 2] = pix(int((1.0-(b-(r*r)))*9));
					//	}
					//}
				}
			}
		}
	}

	void ParticleDraw1(point *p, int n) {
		int x, y;
		int f[HEIGHT][WIDTH] = {0};
		for (int i = 0; i < n; i++) 
		{
			x = Zoom((p+i)->x, WIDTH/2.0f);
			y = Zoom((p+i)->y, HEIGHT/2.0f);
			if (Zoom((p + i)->x, WIDTH / 2.0f) - x > 0.49) {
				x += 1;
			}
			if (Zoom((p + i)->y, HEIGHT / 2.0f) - y > 0.49) {
				y += 1;
			}
			if (((x < WIDTH) && (y < HEIGHT)) && (!(x < 0) && !(y < 0))) 
			{
				if (f[(HEIGHT - 1) - y][x] <= MAX_P) 
				{
					f[(HEIGHT - 1) - y][x] += 1;
				}
			}
		}
		for (int i = 0; i < HEIGHT; i++) 
		{
			for (int j = 0; j < WIDTH; j++) 
			{
				frame[(HEIGHT - 1) - i][j * 2] = pix(int(f[(HEIGHT - 1) - i][j] * 0.3));
			}
		}
	}

	void ParticleDraw2(point *p, int n) {
		for (int i = 0; i < n; i++) 
		{
			if (GetPixDraw(int(Zoom((p + i)->x, WIDTH/2.0f)), int(Zoom((p + i)->y, HEIGHT/2.0f))) != ' ') 
			{
				pixDro(int(Zoom((p + i)->x, WIDTH/2.0f)), int(Zoom((p + i)->y, HEIGHT/2.0f)), 5);
			}
			else 
			{
				pixDro(int(Zoom((p + i)->x, WIDTH/2.0f)), int(Zoom((p + i)->y, HEIGHT/2.0f)), 1);
			}
		}
	}

	void Draw() {
		Prin(frame[0]);
	}

	void SetZoom(float a) {
		zoom = a;
	}
};


