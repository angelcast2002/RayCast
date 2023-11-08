#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "color.h"
#include "raycaster.h"
#include "imageloader.h"
#include <ctime>
#include <sstream>
#include "map.h"
#include <iostream>

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

std::vector<Map> maps;
Color w = { 255, 255, 255, 255 };

void clear () {
    // sky color
    SDL_SetRenderDrawColor(renderer, 56, 56, 56, 255);
    SDL_RenderClear(renderer);
}

void draw_floor () {
    // floor color
    SDL_SetRenderDrawColor(renderer, 112, 112, 112, 255);
    SDL_Rect rect = { 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2 };
    SDL_RenderFillRect(renderer, &rect);
}

void draw_hand (std::string image) {
    int height = 43 * 3;
    int width = 64 * 3;

    ImageLoader::render(renderer, image, SCREEN_WIDTH / 2.0f - width / 2.0f, SCREEN_HEIGHT - height, width, height);
}

bool loadFont() {
    font = TTF_OpenFont(R"(..\assets\font\DooM.ttf)", 25); // Carga una fuente TrueType
    if (!font) {
        std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void drawText(const char* text, int x, int y, Color color){
    // Dentro de este bucle, puedes renderizar elementos de bienvenida (texto, imágenes, etc.).
    // Usar SDL_RenderCopy y SDL_RenderPresent para mostrar elementos gráficos.

    // Puedes utilizar TTF_RenderText_Solid para renderizar texto con SDL_ttf.
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, { color.r, color.g, color.b});
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Define la posición del texto en la pantalla.
    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    // Renderiza el texto en la pantalla.
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_RenderPresent(renderer);
}

void loadEffect(const char* path) {
    // wav
    Mix_Chunk* soundEffect = Mix_LoadWAV(path);

    if (soundEffect == nullptr) {
        // Maneja el error de carga del efecto de sonido
        std::cout << "Error al cargar el efecto de sonido: " << Mix_GetError() << std::endl;
    }

    // Reproducir el efecto de sonido
    if (Mix_PlayChannel(-1, soundEffect, 0) == -1) {
        // Maneja el error de reproducción del efecto de sonido
        std::cout << "Error al reproducir el efecto de sonido: " << Mix_GetError() << std::endl;
    }
}

Mix_Chunk* loadMusic(const char* path) {
    // Cargar un archivo WAV
    Mix_Chunk* soundEffect = Mix_LoadWAV(path);

    if (soundEffect == nullptr) {
        // Maneja el error de carga del efecto de sonido
        std::cout << "Error al cargar el efecto de sonido: " << Mix_GetError() << std::endl;
    } else {
        // Ajustar el volumen del efecto de sonido (escala de 0 a 128)
        Mix_VolumeChunk(soundEffect, 64);

        // Reproducir el efecto de sonido
        if (Mix_PlayChannel(-1, soundEffect, -1) == -1) {
            // Maneja el error de reproducción del efecto de sonido
            std::cout << "Error al reproducir el efecto de sonido: " << Mix_GetError() << std::endl;
        }
    }

    return soundEffect;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    ImageLoader::init();
    std::clock_t start_time = std::clock();

    if (Mix_Init(MIX_INIT_MP3) < 0) {
        std::cout << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }

    window = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Uint32 frameStart, frameTime;
    std::string title = "FPS: ";

    TTF_Init();
    if (!loadFont()) {
        return 1;
    }

    bool selectLevel = true;
    bool showWelcome = true;
    Mix_Chunk * mainTheme = loadMusic(R"(..\assets\music\menuTheme.wav)");
    clear();
    drawText("DooM", SCREEN_WIDTH_2 - 25 * 2, SCREEN_HEIGHT_2 - SCREEN_HEIGHT_2 * 0.30f, w);
    drawText("-> Military Base", 150, SCREEN_HEIGHT_2, w);
    drawText("Inferno", 150, SCREEN_HEIGHT_2 + SCREEN_HEIGHT_2 * 0.30f, w);
    bool oldSelectLevel;
    while (showWelcome) {
        oldSelectLevel = selectLevel;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                showWelcome = false;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectLevel = true;
                        loadEffect(R"(..\assets\sounds\selectSound.wav)");
                        clear();
                        break;
                    case SDLK_DOWN:
                        selectLevel = false;
                        loadEffect(R"(..\assets\sounds\selectSound.wav)");
                        clear();
                        break;
                    case SDLK_RETURN:
                        showWelcome = false;
                        loadEffect(R"(..\assets\sounds\enterSound.wav)");
                        break;
                }
                break;
            }
        }
        if (oldSelectLevel != selectLevel) {
            if (selectLevel){
                drawText("DooM", SCREEN_WIDTH_2 - 25 * 2, SCREEN_HEIGHT_2 - SCREEN_HEIGHT_2 * 0.30f, w);
                drawText("-> Military Base", 150, SCREEN_HEIGHT_2, w);
                drawText("Inferno", 150, SCREEN_HEIGHT_2 + SCREEN_HEIGHT_2 * 0.30f, w);
            } else {
                drawText("DooM", SCREEN_WIDTH_2 - 25 * 2, SCREEN_HEIGHT_2 - SCREEN_HEIGHT_2 * 0.30f, w);
                drawText("Military Base", 150, SCREEN_HEIGHT_2, w);
                drawText("-> Inferno", 150, SCREEN_HEIGHT_2 + SCREEN_HEIGHT_2 * 0.30f, w);
            }
        }
    }
    Mix_FreeChunk(mainTheme);

    clear();

    ImageLoader::loadImage("p", R"(..\assets\player\doomHand.png)");
    ImageLoader::loadImage("e1", R"(..\assets\enemies\e1.png)");
    ImageLoader::loadImage("ha", R"(..\assets\player\handAttack.png)");
    ImageLoader::loadImage("pMM" , R"(..\assets\player\doomGuy.png)");

    Raycaster r = { renderer };

    Map map1;
    map1.key = 1;
    map1.map = R"(..\assets\Map1\map1.txt)";
    map1.winPoints = {
            {43, 376},
            {552, 74},
            {557, 373},
            {38, 157},
    };
    map1.isWin = false;
    map1.end = {551, 551};
    map1.music = R"(..\assets\music\map1.wav)";
    map1.switchSound = R"(..\assets\sounds\map1SwitchS.wav)";
    map1.finalDoorSound = R"(..\assets\sounds\finalDoorActivated.wav)";
    maps.push_back(map1);

    Map map2;
    map2.key = 2;
    map2.map = R"(..\assets\Map2\map2.txt)";
    map2.winPoints = {
            {564, 132},
            {318, 159},
            {41, 368},
            {401, 411},
    };
    map2.isWin = false;
    map2.end = {531, 369};
    map2.music = R"(..\assets\music\map2.wav)";
    map2.switchSound = R"(..\assets\sounds\map2SwitchS.wav)";
    map2.finalDoorSound = R"(..\assets\sounds\finalDoorActivated.wav)";
    maps.push_back(map2);

    int currentKey = 2;
    if (selectLevel) {
        currentKey = 1;
    }

    Map currentMap;

    Mix_Chunk* mapMusic;
    for (auto& map : maps) {
        if (map.key == currentKey) {
            currentMap = map;
            mapMusic = loadMusic(currentMap.music);
            break;
        }
    }

    switch (currentMap.key) {
        case 1:
            ImageLoader::loadImage("0", R"(..\assets\Map1\SP_DUDE1.png)");
            ImageLoader::loadImage("1", R"(..\assets\Map1\STONE2.png)");
            ImageLoader::loadImage("2", R"(..\assets\Map1\STONGARG.png)");
            ImageLoader::loadImage("3", R"(..\assets\Map1\SW1STARG.png)");
            ImageLoader::loadImage("f", R"(..\assets\Map1\SW1PIPE.png)");
            break;
        case 2:
            ImageLoader::loadImage("0", R"(..\assets\Map2\BIGDOOR3.png)");
            ImageLoader::loadImage("1", R"(..\assets\Map2\BIGDOOR4.png)");
            ImageLoader::loadImage("3", R"(..\assets\Map2\BIGDOOR6.png)");
            ImageLoader::loadImage("f", R"(..\assets\Map2\BIGDOOR7.png)");
            break;
    }

    r.load_map(currentMap.map);

    float deltaAngle = 3.14/25;
    int speed = 10;

    bool running = true;
    while(running) {
        frameStart = SDL_GetTicks();
        std::clock_t current_time = std::clock();
        double elapsed_time = (current_time - start_time) / static_cast<double> (CLOCKS_PER_SEC);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                float newPlayerX = r.player.x;
                float newPlayerY = r.player.y;
                switch(event.key.keysym.sym ){
                    case SDLK_LEFT:
                        r.player.a += deltaAngle;
                        break;
                    case SDLK_RIGHT:
                        r.player.a -= deltaAngle;
                        break;
                    case SDLK_UP:
                        newPlayerX += speed * cos(r.player.a);
                        newPlayerY += speed * sin(r.player.a);
                        break;
                    case SDLK_DOWN:
                        newPlayerX -= speed * cos(r.player.a);
                        newPlayerY -= speed * sin(r.player.a);
                        break;
                    case SDLK_a:
                        r.player.a += deltaAngle;
                        break;
                    case SDLK_d:
                        r.player.a -= deltaAngle;
                        break;
                    case SDLK_w:
                        newPlayerX += speed * cos(r.player.a);
                        newPlayerY += speed * sin(r.player.a);
                        break;
                    case SDLK_s:
                        newPlayerX -= speed * cos(r.player.a);
                        newPlayerY -= speed * sin(r.player.a);
                        break;
                    case SDLK_SPACE:
                        r.player.attack = true;
                        if (!currentMap.winPoints.empty()) {
                            for (auto it = currentMap.winPoints.begin(); it != currentMap.winPoints.end(); ) {
                                if (((*it)[0] - 20 < static_cast<int>(r.player.x) && static_cast<int>(r.player.x) < (*it)[0] + 20) &&
                                     ((*it)[1] - 20 < static_cast<int>(r.player.y) && static_cast<int>(r.player.y) < (*it)[1] + 20)) {
                                    currentMap.winPoints.erase(it);
                                    loadEffect(currentMap.switchSound);
                                    break;
                                } else {
                                    ++it;
                                }
                            }
                        } else {
                            if ((currentMap.end[0] - 20 < static_cast<int>(r.player.x) && static_cast<int>(r.player.x) < currentMap.end[0] + 20) &&
                                (currentMap.end[1] - 20 < static_cast<int>(r.player.y) && static_cast<int>(r.player.y) < currentMap.end[1] + 20)) {
                                loadEffect(R"(..\assets\sounds\map2SwitchS.wav)");
                                std::cout << "You win!" << std::endl;
                                running = false;
                            }
                        }
                        break;
                    default:
                        break;
                }

                int i = static_cast<int>(newPlayerX / BLOCK);
                int j = static_cast<int>(newPlayerY / BLOCK);

                if (map[j][i] == ' ') {
                    r.player.x = newPlayerX;
                    r.player.y = newPlayerY;
                }
            }
        }

        clear();
        draw_floor();
        r.render();

        if (!currentMap.isWin) {
            if (currentMap.winPoints.empty()) {
                loadEffect(currentMap.finalDoorSound);
                currentMap.isWin = true;
            }
        }


        if (r.player.attack) {
            draw_hand("ha");
            if (elapsed_time > 0.7f) {
                loadEffect(R"(..\assets\sounds\punch.wav)");
                r.player.attack = false;
                start_time = std::clock();
            }
        } else {
            draw_hand("p");
        }

        // render

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime > 0) {
            std::ostringstream titleStream;
            titleStream << "FPS: " << 1000.0 / frameTime;
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    Mix_FreeChunk(mapMusic);

    bool salir = true;
    drawText("You won!", SCREEN_WIDTH_2 - 25 * 3, SCREEN_HEIGHT_2 - SCREEN_HEIGHT_2 * 0.30f, w);
    drawText("Press ESC to exit", SCREEN_WIDTH_2 - 25 * 6, SCREEN_HEIGHT_2 - SCREEN_HEIGHT_2 * 0.20f, w);
    Mix_Chunk * winTheme = loadMusic(R"(..\assets\music\menuTheme.wav)");

    while (salir) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                salir = true;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym ){
                    case SDLK_ESCAPE:
                        salir = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    Mix_FreeChunk(winTheme);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
