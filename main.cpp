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
	vec2 resolution = vec2(float(width), float(height));

    float pixelRatio = 8.0f/16.0f;
    width = floor(width / pixelRatio);
    float screenRatio = float(width) / float(height);

	wstring screen(width * height, L' ');
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;

    SetWindow(width, height);
    for (int t = 0; t < 100000; t++){
		for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				wchar_t finalPixel = L' ';
				vec2 uv = vec2((x - width / 2) / float(width), (y - height / 2) / float(height));
				uv.x = uv.x * pixelRatio * screenRatio;
				if (uv.x * uv.x + uv.y * uv.y < 0.5f)
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
