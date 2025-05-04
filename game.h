#ifndef GAME_H
#define GAME_H
#include "appstate.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "brick.h"
#include "eagle.h"
#include <vector>
#include <string>


class Game : public AppState
{
public:

    Game();

    Game(int players_count);

    Game(std::vector<Player*> players, int previous_level);

    ~Game();

    bool finished() const;

    void draw();

    void update(Uint32 dt);

    void eventProcess(SDL_Event* ev);

    void loadLevel(std::string path);

    AppState* nextState();

    void clearLevel();

    void nextLevel();

    void generateEnemy();

    void generateBonus();


    void checkCollisionTankWithLevel(Tank* tank, Uint32 dt);

    void checkCollisionTwoTanks(Tank* tank1, Tank* tank2, Uint32 dt);

    void checkCollisionBulletWithLevel(Bullet* bullet);

    void checkCollisionBulletWithBush(Bullet* bullet);

    void checkCollisionPlayerBulletsWithEnemy(Player* player, Enemy* enemy);

    void checkCollisionEnemyBulletsWithPlayer(Enemy* enemy, Player* player);

    void checkCollisionTwoBullets(Bullet* bullet1, Bullet* bullet2);




    int m_level_columns_count;
    int m_level_rows_count;

    std::vector< std::vector <Object*> > m_level;

    std::vector<Object*> m_bushes;
    std::vector<Enemy*> m_enemies;
    std::vector<Player*> m_players;
    std::vector<Player*> m_killed_players;

    Eagle* m_eagle;
    int m_current_level;
    int m_player_count;
    int m_enemy_to_kill;
    bool m_level_start_screen;
    bool m_protect_eagle;

    Uint32 m_level_start_time;
    Uint32 m_enemy_redy_time;
    Uint32 m_level_end_time;
    Uint32 m_protect_eagle_time;

    bool m_game_over;
    double m_game_over_position;
    bool m_finished;
    bool m_pause;
    int m_enemy_respown_position;

    void setLevel(int level);
    bool is_single_player = true;  // mặc định là chơi 1 người


};

#endif // GAME_H
