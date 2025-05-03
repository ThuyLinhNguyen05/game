#ifndef SHOOT_H
#define SHOOT_H

#include <SDL_mixer.h>
#include <string>

class ShootSound {
public:
    ShootSound();      // Constructor
    ~ShootSound();     // Destructor

    bool load(const std::string& filepath);  // Tải âm thanh
    void play();                           // Phát âm thanh

private:
    Mix_Chunk* m_shootSound;  // Âm thanh bắn
};

#endif // SHOOT_H
