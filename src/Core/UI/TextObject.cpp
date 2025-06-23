#include "TextObject.h"
#include "Console.h" // Assume Console has drawText(x, y, str)

TextObject::TextObject()
{
    
}

TextObject::TextObject(int x, int y, const std::wstring& text)
: x(x), y(y), text(text) 
{
    textPosX = x + 1;
    textPosY = y + 1;
    height = 3;
    width = text.size();
    border = 1;
}

void TextObject::drawText(ScreenHandler& screenHandler)
{
    // Draw text inside the bordered box
    for (size_t i = 0, j = 0; i < text.length()/2 && i < static_cast<size_t>(width - 2); ++i, j = j+2) {
        std::wstring ws(1, text[j]);
        if (j + 1 < text.length())
            ws += text[j + 1];
        else
            ws += L' ';
        screenHandler.putPixel(x + 1 + i, y + 1, Pixel(ws));
    }
}

void TextObject::setText(const std::wstring& newText) {
    text = newText;
}

void TextObject::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void TextObject::render(ScreenHandler& screenHandler) {

    switch(border)
    {
    case 1:
    {
        Pixel borderPixel(L"##");
        for (int i = 0; i < (width/2)+2; ++i) {
            screenHandler.putPixel(x + i, y, borderPixel);                      // Top border
            screenHandler.putPixel(x + i, y + height - 1, borderPixel);         // Bottom border
        }
        if(width&1)
        {
            screenHandler.putPixel(x + (width/2)+2, y, Pixel(L"# "));                      // Top border
            screenHandler.putPixel(x + (width/2)+2, y + height - 1, Pixel(L"# "));         // Bottom border
        }
        for (int j = 1; j < height - 1; ++j) {
            screenHandler.putPixel(x, y + j, borderPixel);                      // Left border
            if(width&1)
            {
                screenHandler.putPixel(x + (width/2)+1, y + j, Pixel(L" #"));        // Right border
                screenHandler.putPixel(x + (width/2)+2, y + j, Pixel(L"# "));
            }
            else
                screenHandler.putPixel(x + (width/2)+1, y + j, borderPixel);        // Right border
        }
        break;   
    }
    default:
        break;
    }

    drawText(screenHandler);
}

