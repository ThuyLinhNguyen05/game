// PlayerTank.h
#pragma once

#include <SDL.h>
#include <vector>

// Forward declaration để tránh lỗi circular include
class Wall;

class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;

    // Constructor
    PlayerTank(int startX, int startY);

    // Phương thức di chuyển
    void move(int dx, int dy, const std::vector<Wall>& walls);

    // Phương thức vẽ
    void render(SDL_Renderer* renderer);
};
