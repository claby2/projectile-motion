#include <SDL2/SDL.h>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int OBJECT_SIZE = 15;
const float GRAVITY = 0.00005;
const float INITIAL_VELOCITY = 0.1;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    gWindow = SDL_CreateWindow("Projectile Motion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
}

float getAngle(int mouseX, int mouseY, float projectileX, float projectileY) {
    return atan2((float)mouseY - projectileY, (float)mouseX - projectileX);
}

class Projectile {
    public:
        float trueX, trueY;   
        bool isThrown;
        Projectile() {
            x = SCREEN_WIDTH / 4;
            y = SCREEN_HEIGHT / 2;
            isThrown = false;
        }
        void resetPosition() {
            x = SCREEN_WIDTH / 4;
            y = SCREEN_HEIGHT / 2;
            isThrown = false;
        }
        void throwProjectile(int mouseX, int mouseY) {
            float angle = getAngle(mouseX, mouseY, trueX, trueY);
            xVel = -1 * (INITIAL_VELOCITY * std::cos(angle));
            yVel = -1 * (INITIAL_VELOCITY * std::sin(angle));

            isThrown = true;
        }
        void move() {
            if(isThrown) {
                yVel -= GRAVITY;
                x -= xVel;
                y -= yVel;
            }
        }
        void getTrueCoords() {
            trueX = x + (OBJECT_SIZE / 2);
            trueY = y + (OBJECT_SIZE / 2);
        }
        void render() {
            SDL_Rect projectileRect = {x, y, OBJECT_SIZE, OBJECT_SIZE};
            SDL_RenderDrawRect(gRenderer, &projectileRect);
        }

    private:
        float x, y, xVel, yVel;     
};

void drawTrajectory(int mouseX, int mouseY, float projectileX, float projectileY) {
    float angle = getAngle(mouseX, mouseY, projectileX, projectileY);
    angle = -1 * angle;

    float tanAngle = std::tan(angle);
    float coef = ((GRAVITY / (2 * (INITIAL_VELOCITY * INITIAL_VELOCITY) * (std::cos(angle) * std::cos(angle)))));

    float previousY = 0;
    for(float x = 0; x < SCREEN_WIDTH; x++) {
        if((mouseX >= projectileX && x >= projectileX) || (mouseX <= projectileX && x <= projectileX)) {
            float coordX = x - projectileX;
            float y = (tanAngle * coordX) - ((coef) * (coordX * coordX));
            y = -1 * y;
            if(x > 0) {
                SDL_RenderDrawLine(gRenderer, x - 1, previousY + projectileY, x, y + projectileY);
            }
            previousY = y;
        }
    }
}

int main(int argc, char* args[]) {
    init();

    bool quit = false;
    SDL_Event event;

    SDL_Rect startPos = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, OBJECT_SIZE, OBJECT_SIZE};

    Projectile projectile;

    while(!quit) {
        int mouseX, mouseY;
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = true;
            }
            if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                switch(event.key.keysym.sym) {
                    case SDLK_SPACE : projectile.resetPosition(); break;
                }
            }
            SDL_GetMouseState(&mouseX, &mouseY);
            if(event.type  == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                projectile.resetPosition();
                projectile.getTrueCoords();
                projectile.throwProjectile(mouseX, mouseY);
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(gRenderer);
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        SDL_RenderDrawRect(gRenderer, &startPos);

        drawTrajectory(mouseX, mouseY, startPos.x + (OBJECT_SIZE / 2), startPos.y + (OBJECT_SIZE / 2));

        projectile.getTrueCoords();
        projectile.render();
        projectile.move();

        SDL_RenderPresent(gRenderer);
    }

    close();
}