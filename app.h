#ifndef APP_H
#define APP_H

#include "appstate.h"

class App
{
public:
    App(); // Constructor, tạo đối tượng
    ~App(); // Destructor, hủy đối tượng

    void run(); // Hàm chạy app

    void eventProces(); // Hàm xử lý sự kiện
private:
    bool is_running; // Biến kiếm tra ứng dụng hiện có đang chạy không
    AppState* m_app_state; // Biến xử lý ứng dụng
    SDL_Window* m_window; // Cửa số SDL
};

#endif // APP_H
