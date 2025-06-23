#include "Pixel.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <conio.h>
#include <thread>
#include <fcntl.h>
#include <io.h>

Pixel::Pixel() : character(L"  "), fgRGB({0, 0, 0}), bgRGB({0, 0, 0}) {

}

Pixel::Pixel(int number) : character(std::to_wstring(number)), fgRGB({0,0,0}), bgRGB({255,255,255}) {}

Pixel::Pixel(const std::wstring& ch) : character(ch), fgRGB({255,255,255}), bgRGB({0,0,0}) {}

Pixel::Pixel(const std::wstring & ch, RGB fg) : character(ch), fgRGB(fg) {}

Pixel::Pixel(const std::wstring & ch, RGB fg, RGB bg) : character(ch), fgRGB(fg), bgRGB(bg) {}

void Pixel::setChar(const std::wstring& ch) {
    character = ch;
}

const std::wstring Pixel::getChar() const {
    std::wstringstream wss;

    if(false)
    {
        wss << L"\x1b[38;2;" << fgRGB.r << L";" << fgRGB.g << L";" << fgRGB.b <<
            L"m\x1b[48;2;" << bgRGB.r << L";" << bgRGB.g << L";" << bgRGB.b <<
            L"m" << character << L"\x1b[0m";
        return wss.str(); 
    }else
    {
        return character;
    }
}

void Pixel::setRGBColors(RGB fg, RGB bg) {
    fgRGB = fg;
    bgRGB = bg;
}

void Pixel::addStyle(TextStyle style) {
    styles.push_back(style);
}

void Pixel::clearStyles() {
    styles.clear();
}

// std::wstring Pixel::getAnsiCode() const {
//     std::wstringstream ss;
//     ss << L"\x1b[";

//     for (size_t i = 0; i < styles.size(); ++i) {
//         ss << static_cast<int>(styles[i]);
//         if (i != styles.size() - 1)
//             ss << L";";
//     }

//     if (!styles.empty())
//         ss << L";";

//     if (useTrueColor) {
//         ss << L"38;2;" << fgRGB.r << L";" << fgRGB.g << L";" << fgRGB.b << L";";
//         ss << L"48;2;" << bgRGB.r << L";" << bgRGB.g << L";" << bgRGB.b;
//     } else {
//         ss << fgAnsi << L";" << bgAnsi;
//     }

//     ss << L"m";
//     return ss.str();
// }

// std::wstring Pixel::render() const {
//     return getAnsiCode() + character + L"\x1b[0m"; // reset
// }
