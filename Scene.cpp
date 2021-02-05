#include "test.h"
#include <ctime>
#include <cwchar>

using namespace std;

float random(float low, float high)
{
	return low + float(rand()) / (float(RAND_MAX / (high - low)));
}

void ConsoleSizeFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

int main() {
	system("mode con cols=142 lines=50");
	ConsoleSizeFont();


	Scene test;

	const int n = 20000;
	point p[n*2];
	point mass = { WIDTH, HEIGHT+28, -0.27, -0.2};
	point mass2 = { 0.0, -28, 0.27, 0.2};
	float r = 0.0f;
	float R = 20.0f;

	for (int i = 0; i < n; i++) {
		float k = random(2.5f, R);
		float c = float(((2.0f*pi) / n)*i);
		float v = sqrt((G*M) / k);

		p[i].x = cos(c) * k + mass.x;
		p[i].y = sin(c) * k + mass.y;

		p[i].vect_x = -(sin(c) * v) + mass.vect_x;
		p[i].vect_y = (cos(c) * v) + mass.vect_y;
	}

	for (int i = n; i < n*2; i++) {
		float k = random(2.5f, R);
		float c = float(((2.0f*pi) / n)*i);
		float v = sqrt((G*M) / k);

		p[i].x = cos(c) * k + mass2.x;
		p[i].y = sin(c) * k + mass2.y;

		p[i].vect_x = -(sin(c) * v) + mass2.vect_x;
		p[i].vect_y = (cos(c) * v) + mass2.vect_y;
	}
	
	while (true) {
		test.ClearDraw();
		test.ParticleDraw1(p, n*2);
		test.CircleDraw(&mass, 3.5f);
		test.CircleDraw(&mass2, 3.5f);
		test.Draw();
		//printf("\n x %f\tvx %f\t|\tx %f\tvx %f", mass.x, mass.vect_x, mass2.x, mass2.vect_x);
		//printf("\n y %f\tvy %f\t|\ty %f\tvy %f", mass.y, mass.vect_y, mass2.y, mass2.vect_y);

		if (mass.CalcDistPoint(&mass2) < 1000 * 2) {
			test.SetZoom(1);
			//printf(" Zoom: 1");
		}
		else {
			test.SetZoom(0.8);
			//printf(" Zoom: 0.8");
		}

		mass++;
		mass2++;
		
		for (int i = 0; i < n*2; i++) {
			p[i].x += p[i].vect_x;
			p[i].y += p[i].vect_y;
		}

		mass.CalcPointPhysics(p, n*2);
		mass2.CalcPointPhysics(p, n*2);
		mass.CalcPointPhysics(&mass2);
		mass2.CalcPointPhysics(&mass);

		Sleep(30);
	}

	system("pause");
	return 0;
}