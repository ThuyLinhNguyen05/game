#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void loadTexture(SDL_Window* window);
    void loadFont();
    void clear();
    void flush();
    void drawObject(const SDL_Rect *texture_src, const SDL_Rect *window_dest);
    void setScale(float xs, float ys);
    void drawText(const SDL_Point* start, std::string text, SDL_Color text_color, int font_size = 1);
    void drawRect(const SDL_Rect* rect, SDL_Color rect_color, bool fill = false);

    SDL_Renderer* getRenderer(); // lấy renderer gốc
    TTF_Font* getFont();         // lấy font đang dùng

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    SDL_Texture* m_text_texture;
    TTF_Font* m_font1;
    TTF_Font* m_font2;
    TTF_Font* m_font3;
};

#endif // RENDERER_H
