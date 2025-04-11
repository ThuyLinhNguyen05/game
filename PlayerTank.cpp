// PlayerTank.cpp
#include "PlayerTank.h"
#include "Wall.h" // Include Wall.h nếu có, hoặc forward declaration

PlayerTank::PlayerTank(int startX, int startY) {
    x = startX;
    y = startY;
    dirX = 0;
    dirY = -1;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
}

void PlayerTank::move(int dx, int dy, const std::vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void PlayerTank::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
