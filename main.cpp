#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <SDL_video.h>

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

    ImageLoader::loadImage("0", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\Map1\\SP_DUDE1.png" );
    ImageLoader::loadImage("1", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\Map1\\STONE2.png" );
    ImageLoader::loadImage("2", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\Map1\\STONGARG.png" );
    ImageLoader::loadImage("3", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\Map1\\SW1STARG.png" );
    ImageLoader::loadImage("p", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\player\\doomHand.png" );
    ImageLoader::loadImage("e1", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\enemies\\e1.png");
    ImageLoader::loadImage("ha", "C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\player\\handAttack.png");

    Raycaster r = { renderer };
    r.load_map("C:\\Users\\caste\\OneDrive\\Documentos\\Universidad\\semestre6\\graficosxcomputador\\DoomClone\\assets\\map.txt");

    Uint32 frameStart, frameTime;
    std::string title = "FPS: ";

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
                switch(event.key.keysym.sym ){
                    case SDLK_LEFT:
                        r.player.a += 3.14/25;
                        break;
                    case SDLK_RIGHT:
                        r.player.a -= 3.14/25;
                        break;
                    case SDLK_UP:
                        r.player.x += speed * cos(r.player.a);
                        r.player.y += speed * sin(r.player.a);
                        break;
                    case SDLK_DOWN:
                        r.player.x -= speed * cos(r.player.a);
                        r.player.y -= speed * sin(r.player.a);
                        break;
                    case SDLK_a:
                        r.player.a += 3.14/25;
                        break;
                    case SDLK_d:
                        r.player.a -= 3.14/25;
                        break;
                    case SDLK_w:
                        r.player.x += speed * cos(r.player.a);
                        r.player.y += speed * sin(r.player.a);
                        break;
                    case SDLK_s:
                        r.player.x -= speed * cos(r.player.a);
                        r.player.y -= speed * sin(r.player.a);
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
