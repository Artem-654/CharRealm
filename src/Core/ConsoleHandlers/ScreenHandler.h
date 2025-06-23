#pragma once

#include <windows.h>
#include <vector>
#include "../UI/Pixel.h"

class ScreenHandler {
    HANDLE hOutput;
    std::vector<std::vector<Pixel>> screenBuffer;
    int screenWidth, screenHeight, maxWidth, maxHeight;
public:
    ScreenHandler();
    void putPixel(int x, int y, Pixel newPixel);
    void draw();
    void resizeIfNeeded();
    //void drawPixel(Pixel newPixel);
    void drawPixel(int x, int y, Pixel newPixel);
    void clear();
    void setMaxScreenSize(int height, int width);
    void setMaxScreenSizeToCurrent();
};