#ifndef APP_H
#define APP_H

#include "appstate.h"
#include "background.h"
#include "shoot.h"

class App
{
public:
    App(); // Constructor, tạo đối tượng
    ~App(); // Destructor, hủy đối tượng

    void run(); // Hàm chạy app

    void eventProces(); // Hàm xử lý sự kiện
private:
    bool is_running; // Biến kiếm tra ứng dụng hiện có đang chạy không
    bool is_shooting;
    AppState* m_app_state; // Biến xử lý ứng dụng
    SDL_Window* m_window; // Cửa số SDL
    BackgroundMusic bg_music; // Nhạc nền
    ShootSound shootSound;  // Đối tượng âm thanh bắn
};

#endif // APP_H
