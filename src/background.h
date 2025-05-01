#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL_mixer.h>
#include <string>

class BackgroundMusic {
public:
    BackgroundMusic();
    ~BackgroundMusic();

    bool load(const std::string& filepath); // Load file nhạc
    void play();                            // Phát nhạc
    void stop();                            // Dừng nhạc
    void pause();                           // Tạm dừng
    void resume();                          // Tiếp tục

private:
    Mix_Music* m_music;
};

#endif // BACKGROUND_H
