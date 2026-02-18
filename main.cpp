#include <iostream>
#include <windows.h>
#include <math.h>
#include "VecFunc.h"

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

	wstring screen(width * height, L' ');
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;

    SetWindow(width, height);
    for (int t = 0; t < 100000; t++){
		for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				wchar_t finalPixel = L' ';
				vec2 uv = vec2(x - width / 2, y - height / 2);
				if (uv.x * uv.x + uv.y * uv.y < 20)
					finalPixel = L'#';
				
				screen[y * width + x] = finalPixel;
			}
		}
		
		WriteConsoleOutputCharacterW(
            hConsole,
            screen.c_str(),
            width * height,
            {0, 0},
            &written
        );
		Sleep(100);
	}
    return 0;
}
