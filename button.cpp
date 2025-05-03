#include "button.h"

Button::Button() : textTexture(nullptr), renderer(nullptr), font(nullptr) {}

Button::Button(int x, int y, int w, int h, const std::string& text, SDL_Color color, SDL_Renderer* renderer, TTF_Font* font)
    : rect({x, y, w, h}), label(text), color(color), renderer(renderer), font(font), textTexture(nullptr)
{
    createTextTexture();
}

void Button::createTextTexture() {
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, label.c_str(), {255, 255, 255});
    if (surface != nullptr) {
        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void Button::render(SDL_Renderer* renderer) {
    // Vẽ nền nút
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Vẽ viền
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Vẽ chữ
    if (textTexture) {
        int textW, textH;
        SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
        SDL_Rect dst = {rect.x + (rect.w - textW) / 2, rect.y + (rect.h - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, textTexture, nullptr, &dst);
    }
}

bool Button::isInside(int x, int y) {
    return x >= rect.x && x <= rect.x + rect.w &&
           y >= rect.y && y <= rect.y + rect.h;
}

void Button::setText(const std::string& text) {
    label = text;
    createTextTexture();
}
