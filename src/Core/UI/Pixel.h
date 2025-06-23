#pragma once

#include <string>
#include <vector>
#include <sstream>

class Pixel {
public:
    struct RGB {
        int r = 255, g = 255, b = 255;
    };

    enum class TextStyle {
        Reset = 0, Bold = 1, Dim = 2, Italic = 3,
        Underline = 4, Blink = 5, Invert = 7,
        Hidden = 8, Strikethrough = 9
    };

private:
    std::wstring character;
    std::vector<TextStyle> styles;

    RGB fgRGB{}, bgRGB{};

public:
    Pixel();
    Pixel(int number);
    Pixel(const std::wstring& ch);
    Pixel(const std::wstring& ch, RGB fg);
    Pixel(const std::wstring& ch, RGB fg, RGB bg);

    void setChar(const std::wstring& ch);
    const std::wstring getChar() const;

    void setRGBColors(RGB fg, RGB bg);

    void addStyle(TextStyle style);
    void clearStyles();

    //std::wstring getAnsiCode() const;
    //std::wstring render() const;
};
