#include "ScreenHandler.h"
#include <iostream>

ScreenHandler::ScreenHandler() : hOutput(GetStdHandle(STD_OUTPUT_HANDLE))
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOutput, &csbi);

    int newWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    screenWidth = newWidth/2;
    screenHeight = newHeight;
    screenBuffer.resize(screenHeight, std::vector<Pixel>(screenWidth - 1));
}

void ScreenHandler::putPixel(int x, int y, Pixel newPixel)
{
    screenBuffer[y][x] = newPixel;
}

void ScreenHandler::draw()
{
    for(int i = 0; i < screenBuffer.size(); i++)
    {
        for(int j = 0; j< screenBuffer[i].size(); j++)
        {
            std::wcout << screenBuffer[i][j].getChar();
        }
        if(i != (screenBuffer.size() - 1))
            std::wcout << L"\n";
    }
    SetConsoleCursorPosition(hOutput,COORD{0,0});
}

void ScreenHandler::resizeIfNeeded() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOutput, &csbi);

    int newWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int newHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (newWidth != screenWidth || newHeight != screenHeight) {
        screenWidth = newWidth / 2;
        screenHeight = newHeight;

        screenBuffer.resize(screenHeight);
        for (auto& row : screenBuffer) {
            row.resize(screenWidth - 1);
        }
    }

    // if (newWidth != screenWidth || newHeight != screenHeight) {
    //     if((newWidth < maxWidth))
    //     {
    //         screenWidth = newWidth/2;
    //     }
    //     if((newHeight < maxHeight))
    //     {
    //         screenHeight = newHeight;
    //     }
    //     if(newWidth < maxWidth || newHeight < maxHeight)
    //         screenBuffer.resize(screenHeight, std::vector<Pixel>(screenWidth - 1));
    // }
}

// void ScreenHandler::drawPixel(Pixel newPixel)
// {
//     std::wcout << newPixel.getChar();
// }

// void ScreenHandler::drawPixel(int x, int y, Pixel newPixel)
// {
//     putPixel(x,y, newPixel);
//     draw();
// }

void ScreenHandler::clear()
{
    for (int y = 0; y < screenHeight - 1; ++y) {
        for (int x = 0; x < screenWidth - 1; ++x) {
            screenBuffer[y][x] = Pixel(L"  ");
        }
    }
    SetConsoleCursorPosition(hOutput,COORD{0,0});
}

void ScreenHandler::setMaxScreenSize(int height, int width)
{
    maxHeight = height;
    maxWidth = width;
}

void ScreenHandler::setMaxScreenSizeToCurrent()
{
    maxHeight = screenHeight;
    maxWidth = screenWidth;
}
