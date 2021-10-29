#pragma once
#include "Engine.h"
#include <map>
#include <string>

class Font
{
private:
    Font();

    std::string fontnamec_ = "./data/font/chinese.ttf";
    std::string fontnamee_ = "./data/font/english.ttf";

    int stat_message_ = 0;

    // TODO: 缓存画过的字符,好像不区分字体，这儿有点问题, 对应为{字符: {字号: 纹理}}
    std::map<uint32_t, std::map<int, BP_Texture*>> buffer_;    

public:
    static Font* getInstance()
    {
        static Font f;
        return &f;
    }
    static BP_Rect getBoxSize(int textLen, int size, int x, int y);
    void setStatMessage(int s) { stat_message_ = s; }
    void draw(const std::string& text, int size, int x, int y, BP_Color color = { 255, 255, 255, 255 }, uint8_t alpha = 255);
    void drawWithBox(const std::string& text, int size, int x, int y, BP_Color color = { 255, 255, 255, 255 }, uint8_t alpha = 255, uint8_t alpha_box = 255);
    void drawText(const std::string& fontname, std::string& text, int size, int x, int y, uint8_t alpha, int align, BP_Color c);
    void clearBuffer();
    int getBufferSize();
    static int getTextDrawSize(const std::string& text);
};
