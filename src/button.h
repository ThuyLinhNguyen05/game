#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Button {
public:
    Button();
    Button(int x, int y, int w, int h, const std::string& text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font);

    void render(SDL_Renderer* renderer);
    bool isInside(int x, int y);
    void setText(const std::string& text);

private:
    SDL_Rect rect;
    std::string label;
    SDL_Color color;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;
    TTF_Font* font;

    void createTextTexture();
};

#endif
