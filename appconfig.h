#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "player.h"
#include <iostream>
#include <SDL.h>
#include <vector>

using namespace std;
// Lớp tập hợp các tham số ứng dụng (thông số cửa sổ, cài đặt game, các tùy chọn chung của game)
class AppConfig
{
public:
;
    static string texture_path; // đường dẫn texture đến các đối tượng
    static string levels_path; //đường đến thư mục chứa các màn chơi
    static string font_name; //đường dẫn đến font chữ
    static string game_over_text; // chữ hiển thị sau khi chơi thua
    static SDL_Rect map_rect; //kích thước của bản đồ trò chơi
    static SDL_Rect windows_rect;// kích thước và vị trí của vùng hiển thị trạng thái game so với bản đồ
    static SDL_Rect status_rect;// kích thước cửa sổ ứng dụng
    static SDL_Rect tile_rect; // kích thước 1 ô tile trên bản đồ
    static vector<SDL_Point> player_starting_point; //hai vị trí bắt đầu của người chơi
    static vector<SDL_Point> enemy_starting_point; // ba vị trí bắt đầu của kẻ địch
    static vector <Player::PlayerKeys> player_keys; //điều khiển xe tăng của người chơi
    static unsigned level_start_time; //thời gian hiển thị số thứ tự vòng chơi khi bắt đầu
    static unsigned slip_time; //thời gian trượt trên bảng (mini giây)
    static unsigned enemy_start_count; //số lượng kẻ đihjc cần tiêu diệt trong một vòng chơi
    static unsigned enemy_redy_time; //thời gian để một kẻ địch mới xuất hiện trên bản đồ(mini giây)
    static unsigned player_bullet_max_size; //số lượng đạn tối đa mà người chơi có thể bắn ra(mặc định)
    static unsigned score_show_time; //thời gian hiển thị điểm số sau khi đếm điểm xong
    static unsigned tank_shield_time; // thời gian hiệu lực lá chắn
    static unsigned tank_frozen_time; //thời gian kẻ địch bị đóng băng
    static unsigned level_end_time; //thời gian sau khi tiêu diệt dịch sang cửa sổ kết quả
    static unsigned protect_eagle_time; // thời gian lớp bảo vệ bằng đá quanh chim đại bàng
    static unsigned player_reload_time; // thời gian tối thiểu 2 lần bắn đạng của người chơi
    static int enemy_max_count_on_map; // số lượng xe tăng tối da xuất hiện đồng thời trên bản đồ
    static double game_over_entry_speed; //tốc độ di chuyển dòng chữ game over
    static double tank_default_speed; // tốc độ mặc định của xe tăng
    static double bullet_default_speed; // tốc độ mặc định của viên đạn
    static bool show_enemy_target; // biến lưu thông tin liệu chức năng hiển thị mục tiêu của kẻ địch có được bật hay không
    static const SDL_Scancode P1_FIRE_KEY;
    static const SDL_Scancode P2_FIRE_KEY;
};
#endif //APPCONFIG_H
