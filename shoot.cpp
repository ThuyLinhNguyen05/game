#include "shoot.h"
#include <iostream>

ShootSound::ShootSound() {
    m_shootSound = nullptr;
}

ShootSound::~ShootSound() {
    if (m_shootSound) {
        Mix_FreeChunk(m_shootSound);
        m_shootSound = nullptr;
    }
}

bool ShootSound::load(const std::string& filepath) {
    m_shootSound = Mix_LoadWAV(filepath.c_str());  // Tải file âm thanh
    if (!m_shootSound) {
        std::cerr << "Failed to load shoot sound: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void ShootSound::play() {
    if (m_shootSound) {
        Mix_PlayChannel(-1, m_shootSound, 0);  // Phát âm thanh bắn, không lặp lại (-1: kênh tự động)
    }
}
