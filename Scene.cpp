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
	point p[n];
	point mass = { WIDTH/2, HEIGHT/2};
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
	
	while (true) {
		test.ClearDraw();
		test.ParticleDraw1(p, n);
		test.Draw();

		mass++;
		
		for (int i = 0; i < n; i++) {
			p[i].x += p[i].vect_x;
			p[i].y += p[i].vect_y;
		}

		mass.CalcPointPhysics(p, n);

		Sleep(30);
	}

	system("pause");
	return 0;
}