#ifndef ENGINE_H
#define ENGINE_H

#include "renderer.h"
#include "spriteconfig.h"

class Engine
{
public:
    Engine();

    static Engine& getEngine(); //engine có thể được truy cập từ bất kỳ module nào chỉ bằng cách sử dụng namespace thay vì con trỏ

    static std::string intToString(int num);

    void initModules();

    void destroyModules();

    Renderer* getRenderer() const;

    SpriteConfig* getSpriteConfig() const;
private:
    Renderer* m_renderer;
    SpriteConfig* m_sprite_config;
};

#endif // ENGINE_H
