// Texture.cpp
#include "Texture.h"
#include <SDL_image.h>
#include <iostream>

Texture::Texture(SDL_Renderer* renderer)
    : m_renderer(renderer), m_texture(nullptr), m_width(0), m_height(0) {}

Texture::~Texture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }
}

bool Texture::loadFromFile(const std::string& path) {
    // Nếu đã có texture cũ, giải phóng nó
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    // Tải texture từ file ảnh
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!m_texture) {
        std::cout << "Unable to load texture! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Tạo texture từ surface
    m_texture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
    if (!m_texture) {
        std::cout << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loadedSurface);
        return false;
    }

    // Lưu kích thước ảnh
    m_width = loadedSurface->w;
    m_height = loadedSurface->h;

    // Giải phóng surface sau khi tạo texture xong
    SDL_FreeSurface(loadedSurface);
    return true;
}

void Texture::render(int x, int y, int width, int height) {
    if (m_texture) {
        SDL_Rect renderQuad = { x, y, width, height };
        SDL_RenderCopy(m_renderer, m_texture, nullptr, &renderQuad);
    }
}
