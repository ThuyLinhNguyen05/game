// Texture.h
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <string>

class Texture {
public:
    // Constructor
    Texture(SDL_Renderer* renderer);

    // Destructor
    ~Texture();

    // Tải texture từ file ảnh
    bool loadFromFile(const std::string& path);

    // Render texture tại vị trí (x, y) với kích thước tùy chỉnh
    void render(int x, int y, int width = 64, int height = 64);

private:
    SDL_Renderer* m_renderer;   // Trình vẽ của SDL
    SDL_Texture* m_texture;     // Texture được tải
    int m_width, m_height;
};

#endif
