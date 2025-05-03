#include "app.h"
#include "appconfig.h"
#include "engine.h"
#include "game.h"
#include "menu.h"

#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

App::App()
{
    m_window = nullptr; // khởi tại m_window là con trỏ không troe đến đâu cả
    is_shooting = false;  // Khởi tạo is_shooting là false

}

App::~App()
{
    if (m_app_state != nullptr)
        delete m_app_state; // giải phóng bộ nhớ của m_app_state nếu nó không phải nullptr
}
void App::run()
{
    is_running = true;

    // Khởi tạo SDL và các thư viện
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
            return;
        }

        m_window = SDL_CreateWindow("TANKS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  AppConfig::windows_rect.w, AppConfig::windows_rect.h,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(m_window == nullptr) return;
        if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return;
        if(TTF_Init() == -1) return;

        srand(time(NULL)); //Khởi tạo trình tạo số ngẫu nhiên giả

        if (!bg_music.load("C:\\Users\\ADMIN\\Downloads\\game\\bin\\Debug\\background.mp3")) {
            std::cerr << "Failed to load background music.\n";
        }
        else
        {
            bg_music.play();
        }

        if (!shootSound.load("C:\\Users\\ADMIN\\Downloads\\game\\bin\\Debug\\shoot.wav")) {
            std::cerr << "Failed to load shoot sound.\n";
        }

        // Khởi tạo engine
        Engine& engine = Engine::getEngine();
        engine.initModules();
        engine.getRenderer()->loadTexture(m_window);
        engine.getRenderer()->loadFont();

        // Khởi tạo trạng thái ban đầu
        m_app_state = new Menu;
        double FPS;
        Uint32 time1, time2, dt, fps_time = 0, fps_count = 0, delay = 15;
        time1 = SDL_GetTicks();
        while(is_running)
        {
            time2 = SDL_GetTicks();
            dt = time2 - time1;
            time1 = time2;

            if(m_app_state->finished())
            {
                AppState* new_state = m_app_state->nextState();
                delete m_app_state;
                m_app_state = new_state;
            }
            if(m_app_state == nullptr) break;

            eventProces();

            m_app_state->update(dt);
            m_app_state->draw();

            SDL_Delay(delay);

            //FPS
            fps_time += dt; fps_count++;
            if(fps_time > 200)
            {
                FPS = (double)fps_count / fps_time * 1000;
                if(FPS > 60) delay++;
                else if(delay > 0) delay--;
                fps_time = 0; fps_count = 0;
            }
        }

        engine.destroyModules();
    }

    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    Mix_Quit();
}

void App::eventProces()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            is_running = false;
        }
        else if(event.type == SDL_WINDOWEVENT)
        {
            if(event.window.event == SDL_WINDOWEVENT_RESIZED ||
               event.window.event == SDL_WINDOWEVENT_MAXIMIZED ||
               event.window.event == SDL_WINDOWEVENT_RESTORED ||
               event.window.event == SDL_WINDOWEVENT_SHOWN)
            {

                AppConfig::windows_rect.w = event.window.data1;
                AppConfig::windows_rect.h = event.window.data2;
                Engine::getEngine().getRenderer()->setScale((float)AppConfig::windows_rect.w / (AppConfig::map_rect.w + AppConfig::status_rect.w),
                                                            (float)AppConfig::windows_rect.h / AppConfig::map_rect.h);
            }
        }

        // Kiểm tra sự kiện khi người chơi bắn
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE) {  // Kiểm tra nếu phím Space được nhấn
                is_shooting = true;  // Cập nhật trạng thái bắn
            }
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_SPACE) {  // Kiểm tra nếu phím Space được thả ra
                is_shooting = false;  // Cập nhật trạng thái không bắn nữa
            }
        }


        m_app_state->eventProcess(&event);
    }
    if (is_shooting) {  // Kiểm tra nếu người chơi bắn
        shootSound.play();  // Phát âm thanh bắn
    }
}

