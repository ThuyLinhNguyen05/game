#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
