#include "background.h"
#include <iostream>

BackgroundMusic::BackgroundMusic() {
    m_music = nullptr;
}

BackgroundMusic::~BackgroundMusic() {
    if (m_music) {
        Mix_FreeMusic(m_music);
        m_music = nullptr;
    }
}

bool BackgroundMusic::load(const std::string& filepath) {
    m_music = Mix_LoadMUS(filepath.c_str());
    if (!m_music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void BackgroundMusic::play() {
    if (m_music) {
        Mix_PlayMusic(m_music, -1); // -1: lặp vô hạn
    }
}

void BackgroundMusic::stop() {
    Mix_HaltMusic();
}

void BackgroundMusic::pause() {
    if (Mix_PlayingMusic()) {
        Mix_PauseMusic();
    }
}

void BackgroundMusic::resume() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}
