#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <map>
#include <string>
#include "color.h"

class ImageLoader {
private:
    static std::map<std::string, SDL_Surface*> imageSurfaces;
public:
    // Initialize SDL_image
    static void init() {
        int imgFlags = IMG_INIT_PNG; // or IMG_INIT_JPG, depending on your needs
        if (!(IMG_Init(imgFlags) & imgFlags)) {
            throw std::runtime_error("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
        }
    }

    // Load an image from a given path and store with a key
    static void loadImage(const std::string& key, const char* path) {
        SDL_Surface* newSurface = IMG_Load(path);
        if (!newSurface) {
            throw std::runtime_error("Unable to load image! SDL_image Error: " + std::string(IMG_GetError()));
        }
        imageSurfaces[key] = newSurface;
    }

    // Get the color of the pixel at (x, y) from an image with a specific key
    static Color getPixelColor(const std::string& key, int x, int y) {
        auto it = imageSurfaces.find(key);
        if (it == imageSurfaces.end()) {
            throw std::runtime_error("Image key not found!");
        }

        SDL_Surface* targetSurface = it->second;
        Uint8* pixelData = static_cast<Uint8*>(targetSurface->pixels);

        int bpp = targetSurface->format->BytesPerPixel;
        Uint8* pixel = pixelData + y * targetSurface->pitch + x * bpp;

        Uint32 pixelColor;
        memcpy(&pixelColor, pixel, bpp);

        SDL_Color color;
        SDL_GetRGB(pixelColor, targetSurface->format, &color.r, &color.g, &color.b);
        return Color{color.r, color.g, color.b};
    }


    static void render(SDL_Renderer* renderer, const std::string& key, int x, int y, int w = -1, int h = -1) {
        auto it = imageSurfaces.find(key);
        if (it == imageSurfaces.end()) {
            throw std::runtime_error("Image key not found!");
        }

        SDL_Surface* targetSurface = it->second;

        // Convert surface to texture
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, targetSurface);
        if (!texture) {
            throw std::runtime_error("Unable to create texture from surface! SDL Error: " + std::string(SDL_GetError()));
        }

        // Set render destination and render the texture
        SDL_Rect destRect;
        if (w == -1 && h == -1) {
            destRect = { x, y, targetSurface->w, targetSurface->h };
        } else {
            destRect = { x, y, w, h };
        }
        SDL_RenderCopy(renderer, texture, NULL, &destRect);

        // Free the created texture
        SDL_DestroyTexture(texture);
    }

    // Clean up
    static void cleanup() {
        for (auto& pair : imageSurfaces) {
            if (pair.second) {
                SDL_FreeSurface(pair.second);
            }
        }
        imageSurfaces.clear();
        IMG_Quit();
    }
};

std::map<std::string, SDL_Surface*> ImageLoader::imageSurfaces;