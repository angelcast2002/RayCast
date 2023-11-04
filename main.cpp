#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_mixer.h>

#include "getPath.h"
#include "color.h"
#include "raycaster.h"
#include "imageloader.h"
#include <ctime>
#include <sstream>

SDL_Window* window;
SDL_Renderer* renderer;


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

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    ImageLoader::init();
    std::clock_t start_time = std::clock();

    window = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    ImageLoader::loadImage("0", getCharFileName("assets\\Map1\\SP_DUDE1.png"));
    ImageLoader::loadImage("1", getCharFileName("assets\\Map1\\STONE2.png"));
    ImageLoader::loadImage("2", getCharFileName("assets\\Map1\\STONGARG.png"));
    ImageLoader::loadImage("3", getCharFileName("assets\\Map1\\SW1STARG.png"));
    ImageLoader::loadImage("p", getCharFileName("assets\\player\\doomHand.png"));
    ImageLoader::loadImage("e1", getCharFileName("assets\\enemies\\e1.png"));
    ImageLoader::loadImage("ha", getCharFileName("assets\\player\\handAttack.png"));
    ImageLoader::loadImage("pMM" , getCharFileName("assets\\player\\doomGuy.png"));

    Raycaster r = { renderer };
    r.load_map(getCharFileName("assets\\map.txt"));

    Uint32 frameStart, frameTime;
    std::string title = "FPS: ";

    if (Mix_Init(MIX_INIT_MP3) < 0) {
        std::cout << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error al inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }

    Mix_Music* music = Mix_LoadMUS(getCharFileName("assets\\sound\\doom.mp3"));

    if (music == nullptr) {
        // Maneja el error de carga de música
        std::cout << "Error al cargar la musica: " << Mix_GetError() << std::endl;
    }

    if (Mix_PlayMusic(music, -1) == -1) {
        // Maneja el error de reproducción de música
        std::cout << "Error al reproducir la musica: " << Mix_GetError() << std::endl;
    }


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
                bool collision = false;
                float newPlayerX = r.player.x;
                float newPlayerY = r.player.y;
                int i, j;
                switch(event.key.keysym.sym ){
                    case SDLK_LEFT:
                        r.player.a += 3.14/25;
                        break;
                    case SDLK_RIGHT:
                        r.player.a -= 3.14/25;
                        break;
                    case SDLK_UP:
                        // Realiza el cálculo de nueva posición deseada
                        newPlayerX = r.player.x + speed * cos(r.player.a);
                        newPlayerY = r.player.y + speed * sin(r.player.a);

                        // Convierte las coordenadas a índices de matriz
                        i = static_cast<int>(newPlayerX / BLOCK);
                        j = static_cast<int>(newPlayerY / BLOCK);

                        // Verifica si la nueva posición colisiona con una pared en el mapa
                        if (map[j][i] == ' ') {
                            r.player.x = newPlayerX;
                            r.player.y = newPlayerY;
                        }
                        break;
                    case SDLK_DOWN:
                        // Realiza el cálculo de nueva posición deseada
                        newPlayerX = r.player.x - speed * cos(r.player.a);
                        newPlayerY = r.player.y - speed * sin(r.player.a);

                        // Convierte las coordenadas a índices de matriz
                        i = static_cast<int>(newPlayerX / BLOCK);
                        j = static_cast<int>(newPlayerY / BLOCK);

                        // Verifica si la nueva posición colisiona con una pared en el mapa
                        if (map[j][i] == ' ') {
                            r.player.x = newPlayerX;
                            r.player.y = newPlayerY;
                        }
                        break;
                    case SDLK_a:
                        r.player.a += 3.14/25;
                        break;
                    case SDLK_d:
                        r.player.a -= 3.14/25;
                        break;
                    case SDLK_w:
                        // Realiza el cálculo de nueva posición deseada
                        newPlayerX = r.player.x + speed * cos(r.player.a);
                        newPlayerY = r.player.y + speed * sin(r.player.a);

                        // Convierte las coordenadas a índices de matriz
                        i = static_cast<int>(newPlayerX / BLOCK);
                        j = static_cast<int>(newPlayerY / BLOCK);

                        // Verifica si la nueva posición colisiona con una pared en el mapa
                        if (map[j][i] == ' ') {
                            r.player.x = newPlayerX;
                            r.player.y = newPlayerY;
                        }
                        break;
                    case SDLK_s:
                        // Realiza el cálculo de nueva posición deseada
                        newPlayerX = r.player.x - speed * cos(r.player.a);
                        newPlayerY = r.player.y - speed * sin(r.player.a);

                        // Convierte las coordenadas a índices de matriz
                        i = static_cast<int>(newPlayerX / BLOCK);
                        j = static_cast<int>(newPlayerY / BLOCK);

                        // Verifica si la nueva posición colisiona con una pared en el mapa
                        if (map[j][i] == ' ') {
                            r.player.x = newPlayerX;
                            r.player.y = newPlayerY;
                        }
                        break;
                    case SDLK_SPACE:
                        r.player.attack = true;
                        break;
                    default:
                        break;
                }
            }
        }

        clear();
        draw_floor();
        r.render();

        if (r.player.attack) {
            draw_hand("ha");
            if (elapsed_time > 0.7f) {
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

    SDL_DestroyWindow(window);
    SDL_Quit();
}
