#include <iostream>
#include <windows.h>
#include <math.h>

using namespace std;

void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

int main(){

    int k = 8;
    int width = 16 * k;
    int height = 9 * k;

    float pixelRatio = 24.0f/11.0f;
    width = floor(width * pixelRatio);
    float screenRatio = float(height) / float(width);

    SetWindow(width, height);
    for (int i = 0; i < 100000; i++)
        printf("fdfs");

    return 0;
}
