#define SDL_MAIN_HANDLED

#include<iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include <SDL_ttf.h>
#include <SDL_mixer.h>



using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
bool gameOver = false;
bool playerWon = false;
void renderGameOver(SDL_Renderer* renderer);

class Button {
public:
    SDL_Rect rect;
    string text;
    SDL_Color color;
    SDL_Color hoverColor;
    bool isHovered;

    Button(int x, int y, int w, int h, string t) : text(t) {
        rect = {x, y, w, h};
        color = {255, 255, 255, 255}; // Màu trắng
        hoverColor = {200, 200, 200, 255}; // Màu xám nhạt khi hover
        isHovered = false;
    }

    void render(SDL_Renderer* renderer, TTF_Font* font) {
        SDL_SetRenderDrawColor(renderer, isHovered ? hoverColor.r : color.r,
                                         isHovered ? hoverColor.g : color.g,
                                         isHovered ? hoverColor.b : color.b,
                                         isHovered ? hoverColor.a : color.a);
        SDL_RenderFillRect(renderer, &rect);

        // Vẽ viền nút
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rect);

        // Vẽ chữ
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), {0, 0, 0, 255});
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {rect.x + (rect.w - surface->w) / 2,
                             rect.y + (rect.h - surface->h) / 2,
                             surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    bool checkHover(int mouseX, int mouseY) {
        isHovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                     mouseY >= rect.y && mouseY <= rect.y + rect.h);
        return isHovered;
    }

    bool handleEvent(SDL_Event* e) {
        if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
            int mouseX = e->button.x;
            int mouseY = e->button.y;
            if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h) {
                return true;
            }
        }
        return false;
    }
};

class Menu {
public:
    Button playButton;
    TTF_Font* font;
    bool isActive;

    Menu() : playButton(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 50, 200, 100, "PLAY") {
        isActive = true;
        if (TTF_Init() == -1) {
            cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
        }
        font = TTF_OpenFont("WinkyRough-VariableFont_wght.ttf", 42); // Thay bằng font bạn có
        if (!font) {
            cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
        }
    }

    ~Menu() {
        TTF_CloseFont(font);
        TTF_Quit();
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Màu nền menu
        SDL_RenderClear(renderer);

        // Vẽ tiêu đề game
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "BATTLE CITY", {255, 255, 0, 255});
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect = {SCREEN_WIDTH/2 - titleSurface->w/2, 100, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        // Vẽ nút PLAY
        playButton.render(renderer, font);

        SDL_RenderPresent(renderer);
    }

    void handleEvents(SDL_Event* e) {
        if (e->type == SDL_MOUSEMOTION) {
            playButton.checkHover(e->motion.x, e->motion.y);
        }
    }
};





class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;

    Wall (int startX, int startY) {
        x = startX;
        y = startY;
        active = true;
        rect = {x, y,TILE_SIZE, TILE_SIZE};
    }

    void render (SDL_Renderer* renderer) {
        if (active) {
            SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};


class Bullet {
public:
    int x, y;
    int dx, dy;
    SDL_Rect rect;
    bool active;

    Bullet(int startX, int startY, int dirX, int dirY){
        x = startX;
        y = startY;
        dx = dirX;
        dy = dirY;
        active = true;
        rect = {x, y, 10, 10};
    }
    void move(){
        x += dx;
        y += dy;
        rect.x = x;
        rect.y = y;
        if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE || y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE){
            active = false;
        }
    }
    void render (SDL_Renderer* renderer){
        if (active) {
            SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
            SDL_RenderFillRect (renderer, &rect);
        }
    }
};



class PlayerTank {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet> bullets;

    PlayerTank (int startX, int startY){
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1;
    }
    void move (size_t dx, size_t dy, const vector<Wall>& walls ){
        int newX = x + dx;
        int newY = y + dy;
        this ->dirX = dx;
        this ->dirY = dy;

        SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE};
        for (size_t i = 0; i < walls.size(); i++){
            if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)){
                return;
            }
        }
        if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 && newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2){
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
    }

    void shoot(){
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5,this -> dirX, this -> dirY));
    }

    void updateBullets(){
        for (auto &bullet : bullets){
            bullet.move();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet &b){return !b.active;}), bullets.end());
    }

    void render (SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (auto &bullet : bullets){
            bullet.render(renderer);
        }
    }

    // Thêm phương thức kiểm tra sự sống
    bool isAlive() const {
        return health > 0;  // Giả sử có thuộc tính health

    }

private:
    int health;

};

class EnemyTank {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    vector<Bullet> bullets;
    Mix_Chunk* shootSound;

    EnemyTank(int startX, int startY, Mix_Chunk* sound){
        moveDelay = 15;
        shootDelay = 5;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = 1;
        active = true;
        shootSound = sound; // Lưu âm thanh vào biến shootSound
    }

    void move (const vector<Wall>& walls){
        if (--moveDelay > 0) return;
        moveDelay = 15;
        int r = rand() % 4;
        if (r == 0){
            this->dirX = 0;
            this->dirY = -5;
        }
        else if (r == 1){
            this->dirX = 0;
            this->dirY = 5;
        }
        else if (r == 2){
            this->dirX = -5;
            this->dirY = 0;
        }
        else if (r == 3){
            this->dirX = 5;
            this->dirY = 0;
        }
        int newX = x + this->dirX;
        int newY = y + this->dirY;

        SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
        for(const auto& wall : walls){
            if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)){
                return;
            }
        }
        if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 && newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2){
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
    }
    void shoot(){
        if (--shootDelay > 0) return;
        shootDelay = 5;
        // Thêm viên đạn của kẻ địch
        bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5, this->dirX, this->dirY));
        // Phát âm thanh bắn khi kẻ địch bắn
        if (shootSound) {
            Mix_PlayChannel(-1, shootSound, 0);  // Phát âm thanh bắn đạn
        }
    }

    void updateBullets(){
        for(auto &bullet : bullets){
            bullet.move();
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet &b) {return !b.active;}), bullets.end());
    }

    void render(SDL_Renderer* renderer){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (auto &bullet : bullets){
            bullet.render(renderer);
        }
    }
};

enum GameState {
    PLAYING,
    WIN,
    LOSE
};

class Game {
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector<Wall> walls;
    PlayerTank player;
    int enemyNumber = 3;
    vector<EnemyTank> enemies;

    bool gameOver = false;
    bool playerWon = false;

    Menu menu;
    bool inMenu;

    Mix_Music* backgroundMusic = nullptr;
    Mix_Chunk* shootSound = nullptr; //khai báo âm thanh bắn


    // function
    Game(): player(((MAP_WIDTH - 1)/2)*TILE_SIZE, (MAP_HEIGHT-2)*TILE_SIZE){
        running = true;
        if (SDL_Init(SDL_INIT_VIDEO)< 0){
            cerr << "SDL could not initialize! SDL_Error:"<< SDL_GetError() <<endl;
            running = false;
        }
        window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window){
            cerr << "Window could not be created! SDL_Error:" << SDL_GetError() << endl;
            running = false ;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer){
            cerr << "Renderer could not be created! SDL_Error:" << SDL_GetError() <<endl;
            running = false;
        }
        generateWalls();
        spawnEnemies();


        // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << endl;
        running = false;
    }

        // Load nhạc nền
    backgroundMusic = Mix_LoadMUS("background.mp3");
    if (!backgroundMusic) {
        cerr << "Failed to load background music! Mix_Error: " << Mix_GetError() << endl;
    }
    else {
        // Phát nhạc nền (lặp vô hạn)
        Mix_PlayMusic(backgroundMusic, -1);
    }


        // Load âm thanh bắn
    shootSound = Mix_LoadWAV("shoot.wav");
    if (!shootSound) {
        cerr << "Failed to load shoot sound! Mix_Error: " << Mix_GetError() << endl;
    }

    }


    void render() {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        for (int i = 1; i < MAP_HEIGHT - 1; ++i){
            for (int j = 1; j < MAP_WIDTH - 1; ++j){
                SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_RenderFillRect(renderer, &tile);
            }
        }
        for (size_t i = 0; i < walls.size(); i++){
            walls[i].render(renderer);
        }
        player.render(renderer);

        for (auto &enemy : enemies){
            enemy.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }
    void run() {
        while (running){
            handleEvents();
            update();
            render();
            SDL_Delay(16);

        // Kiểm tra điều kiện kẻ địch chết hết
        if (enemies.empty()) {
            gameOver = true;
            playerWon = true;
            running = false; // thoát game
    }

        // Kiểm tra điều kiện người chơi chết
        if (!player.isAlive()) {
            gameOver = true;
            playerWon = false;
            running = false; // thoát game
    }


        }
    }
    ~Game() {
        // Giải phóng nhạc nền
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
        // Giải phóng âm thanh bắn đạn
    if (shootSound) {
        Mix_FreeChunk(shootSound);
        shootSound = nullptr;
    }
        // Giải phóng âm thanh bắn của người chơi
    if (shootSound) {
        Mix_FreeChunk(shootSound);
        shootSound = nullptr;
    }

    Mix_CloseAudio();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    void generateWalls(){
        for (int i = 3; i < MAP_HEIGHT - 3; i+=2){
            for (int j = 3; j < MAP_WIDTH - 3; j+=2){
                Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
                walls.push_back(w);
            }
        }
    }
    void handleEvents(){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            } else if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                    case SDLK_UP: player.move(0, -5, walls);break;
                    case SDLK_DOWN: player.move(0, 5, walls);break;
                    case SDLK_LEFT: player.move(-5, 0, walls);break;
                    case SDLK_RIGHT: player.move(5, 0, walls);break;
                    case SDLK_SPACE: player.shoot();
                    if (shootSound) Mix_PlayChannel(-1, shootSound, 0); // Phát âm thanh bắn đạn
                     break;
                }
            }
        }
    }

    void update (){
        player.updateBullets ();

        for (auto& enemy : enemies){
            enemy.move(walls);
            enemy.updateBullets();
            if (rand() % 100 < 2){
                enemy.shoot();
            }
        }
        for (auto& enemy : enemies){
            for (auto& bullet : enemy.bullets){
                for (auto& wall : walls){
                    if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                        wall.active = false;
                        bullet.active = false;
                        break;
                    }
                }
            }
        }

        for (auto& bullet : player.bullets){
            for (auto& wall : walls){
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                    wall.active = false;
                    bullet.active = false;
                    break;
                }
            }
        }

        for (auto& bullet : player.bullets){
            for (auto& enemy : enemies){
                if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)){
                    enemy.active = false;
                    bullet.active = false;
                }
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank &e){return !e.active;}), enemies.end());

        if (enemies.empty()){
            running = false;
            gameOver = true;
            playerWon = true;
        }
        for (auto& enemy : enemies){
            for (auto& bullet : enemy.bullets){

                if (SDL_HasIntersection(&bullet.rect, &player.rect)){
                    running = false;
                    gameOver = true;
                    playerWon = false;
                    return;
                }
            }
        }
    }
    void spawnEnemies(){
        enemies.clear();
        for (int i = 0; i < enemyNumber; ++i){
            int ex, ey;
            bool validPosition = false;
            while (!validPosition){
                ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
                ey = (rand() % (MAP_HEIGHT - 2) +1 ) * TILE_SIZE;
                validPosition = true;
                for (const auto& wall : walls){
                    if (wall.active && wall.x == ex && wall.y == ey){
                        validPosition = false;
                        break;
                    }
                }
            }
            enemies.push_back(EnemyTank(ex, ey, shootSound));
        }
    }

    void renderGameOver(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    std::string message = playerWon ? "YOU WIN" : "YOU LOSE";
    SDL_Color textColor = {255, 255, 255, 255};

    TTF_Font* font = TTF_OpenFont("WinkyRough-VariableFont_wght.ttf", 72);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = {
        SCREEN_WIDTH / 2 - surface->w / 2,
        SCREEN_HEIGHT / 2 - surface->h / 2,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
}

};

void renderGameOver(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font* font = TTF_OpenFont("assets/arial.ttf", 48);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color color = {255, 255, 255, 255};
    std::string message = playerWon ? "YOU WIN" : "YOU LOSE";

    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect;
    dstRect.x = (SCREEN_WIDTH - surface->w) / 2;
    dstRect.y = (SCREEN_HEIGHT - surface->h) / 2;
    dstRect.w = surface->w;
    dstRect.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}





int main(int argc, char* argv[]){

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (TTF_Init() == -1) {
        cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Menu menu;
    SDL_Event event;
    bool startGame = false;

    while (!startGame) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return 0;
            menu.handleEvents(&event);
            if (menu.playButton.handleEvent(&event)) {
                startGame = true;
            }
        }
        menu.render(renderer);
        SDL_Delay(16);

    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Game game;
    if (game.running){
        game.run();
        // Sau khi game dừng, kiểm tra nếu có kết thúc game
    if (gameOver) {
        renderGameOver(game.renderer);  // <- gọi hàm hiển thị YOU WIN / LOSE
        SDL_Delay(3000); // dừng lại 3 giây để người chơi xem kết quả
        }
    }

    TTF_Quit();

    return 0;
}

