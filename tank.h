#ifndef TANK_H
#define TANK_H

#include "object.h"
#include "bullet.h"
#include "type.h"

#include <vector>

typedef unsigned TankStateFlags;

class Tank : public Object //Quán lý cơ chế cơ bản của xe tăng: di chuyển, bắn
{
public:
    Tank(); //Constructor mặc định
    Tank(double x, double y, SpriteType type); //Constructor với vị trí và loại xe tăng
    virtual ~Tank(); //Destructor
    void draw(); //Vẽ xe tăng và các thành phần liên quan
    void update(Uint32 dt); //Cập nhật trạng thái xe tăng theo thời gian
    virtual Bullet* fire(); //Bắn đạn
    SDL_Rect nextCollisionRect(Uint32 dt); //Tính toán va chạm tiếp theo
    void setDirection(Direction d); //Đặt hướng di chuyển
    void collide(SDL_Rect &intersect_rect); //Xử lý va chạm

    virtual void respawn(); //Hồi sinh xe tăng
    virtual void destroy(); //Hủy xe tăng

    void setFlag(TankStateFlag flag); //Bặt cờ trạng thái
    void clearFlag(TankStateFlag flag); //Tắt cờ trạng thái
    bool testFlag(TankStateFlag flag);//Kiểm tra cờ trạng thái

    //Các biến thành viên
    double default_speed;
    double speed;
    bool stop;
    Direction direction;
    std::vector<Bullet*> bullets;
    int lives_count;

protected:
    TankStateFlags m_flags;
    Sint32 m_slip_time;
    Direction new_direction;
    unsigned m_bullet_max_size;
    Object* m_shield;
    Object* m_boat;
    Uint32 m_shield_time;
    Uint32 m_frozen_time;
};

#endif // TANK_H
