// snakeGraphic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vector>
#include <iostream>
#include <thread>
#include "Windows.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;
const int SNAKE_SPEED = 5; // Adjust the speed as needed

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_bkground = nullptr;
SDL_Texture* g_snake = nullptr;
SDL_Texture* g_food = nullptr;
SDL_Event g_event;
int lock;

enum Direction { UP, DOWN, LEFT, RIGHT };
Direction snakeDirection = RIGHT;
int snakeX = 300;
int snakeY = 420;
bool eaten;

std::vector<int> tailX;  // Snake tail segment X positions
std::vector<int> tailY;  // Snake tail segment Y positions
int tailLength = 5;  // Length of the snake's tail

// Define the playable area boundaries
const int PLAY_AREA_LEFT = 50;
const int PLAY_AREA_RIGHT = 623;
const int PLAY_AREA_TOP = 50;
const int PLAY_AREA_BOTTOM = 462;


int foodX = rand() % (PLAY_AREA_LEFT + PLAY_AREA_RIGHT - 1) + 1;
int foodY = rand() % (PLAY_AREA_TOP + PLAY_AREA_BOTTOM - 1) + 1;
int foodWidth, foodHeight;
int bkWidth, bkHeight;
int snakeWidth, snakeHeight;

SDL_Texture* LoadTexture(std::string filepath);
void ApplyTexture(SDL_Texture* texture, int x, int y, int width, int height);

bool CheckCollision() {
    // Calculate the distance between the snake and the food
    int distanceX = abs(snakeX - foodX);
    int distanceY = abs(snakeY - foodY);

    // If the snake is close enough to the food, consider it a collision
    return (distanceX < foodWidth  && distanceY < foodHeight);
}

void EatFood() {
    // Check if the snake has collided with the food
    if (CheckCollision()) {
        // Set the foodEaten flag to true
        eaten = true;

        // Update the score or perform any other necessary actions

        // Respawn the food at a random position within the play area
        foodX = rand() % (PLAY_AREA_RIGHT - PLAY_AREA_LEFT - foodWidth*3) + PLAY_AREA_LEFT;
        foodY = rand() % (PLAY_AREA_BOTTOM - PLAY_AREA_TOP - foodHeight*3) + PLAY_AREA_TOP;
    }
}




// Function to add a tail segment to the snake
void AddTailSegment() {
    // Get the position of the last tail segment
    int lastTailX = tailX.back();
    int lastTailY = tailY.back();

    // Add the last tail segment's position to the front of the tail vectors
    tailX.insert(tailX.begin(), lastTailX);
    tailY.insert(tailY.begin(), lastTailY);

    // Increase the tail length
    tailLength++;
}

// Function to update the tail position based on the snake's movement
void UpdateTailPosition() {
    // Move each tail segment to the position of the segment in front of it
    for (int i = tailLength - 1; i > 0; i--) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }

    // Move the first tail segment to the position of the snake's head
    tailX[0] = snakeX;
    tailY[0] = snakeY;
}

// Function to draw the snake's tail on the screen using number images
void DrawTail() {
    for (int i = 0; i < tailLength; i++) {
        // Construct the file path of the number image based on the tail segment number
        std::string filePath = "C:\\Users\\ASUS\\Downloads\\" + std::to_string(i) + ".png";

        // Load the number image
        SDL_Texture* numberTexture = LoadTexture(filePath);
        if (numberTexture != nullptr) {
            // Draw the number image at the tail segment position
            ApplyTexture(numberTexture, tailX[i], tailY[i],snakeWidth*3,snakeHeight *3);

            // Clean up the texture after drawing
            SDL_DestroyTexture(numberTexture);
        }
    }
}


bool Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    g_window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_renderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

SDL_Texture* LoadTexture(std::string filepath) {
    SDL_Surface* surface = IMG_Load(filepath.c_str());
    if (surface == nullptr) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        std::cerr << "Failed to create texture from surface: " << SDL_GetError() << std::endl;
    }

    return texture;
}

//void ApplyTexture(SDL_Texture* texture, int x, int y,int size) {
    //SDL_Rect destRect = { x, y, 0,0 };
    //SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);
    //SDL_RenderCopy(g_renderer, texture, NULL, &destRect);
//}
void ApplyTexture(SDL_Texture* texture, int x, int y, int width, int height) {
    SDL_Rect destRect = { x, y, width, height };
    SDL_RenderCopy(g_renderer, texture, NULL, &destRect);
}

void CleanUp() {
    SDL_DestroyTexture(g_bkground);
    SDL_DestroyTexture(g_snake);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
}

void HandleInput() {
    while (SDL_PollEvent(&g_event)) {
        if (g_event.type == SDL_QUIT) {
            SDL_Quit();
            //exit(0);
        }
        else if (g_event.type == SDL_KEYDOWN) {
            switch (g_event.key.keysym.sym) {
            case SDLK_w:
                snakeDirection = UP;
                lock = 's';
                break;
            case SDLK_UP:
                snakeDirection = UP;
                lock = 's';
                break;
            case SDLK_a:
                snakeDirection = LEFT;
                lock = 'd';
                break;
            case SDLK_LEFT:
                snakeDirection = LEFT;
                lock = 'd';
                break;
            case SDLK_s:
                snakeDirection = DOWN;
                lock = 'w';
                break;
            case SDLK_DOWN:
                snakeDirection = DOWN;
                lock = 'w';
                break;
            case SDLK_d:
                snakeDirection = RIGHT;
                lock = 'a';
                break;
            case SDLK_RIGHT:
                snakeDirection = RIGHT;
                lock = 'a';
                break;
            }
        }
    }
}

void MoveSnake() {
    switch (snakeDirection) {
    case UP:
        if (snakeY > PLAY_AREA_TOP )//&& lock != 'w')
            snakeY -= SNAKE_SPEED;
        break;
    case DOWN:
        if (snakeY < PLAY_AREA_BOTTOM) //&& lock != 's')
            snakeY += SNAKE_SPEED;
        break;
    case LEFT:
        if (snakeX > PLAY_AREA_LEFT) //&& lock != 'd')
            snakeX -= SNAKE_SPEED;
        break;
    case RIGHT:
        if (snakeX < PLAY_AREA_RIGHT) //&& lock != 'a')
            snakeX += SNAKE_SPEED;
        break;
    }
}

int main(int argc, char** argv) {
    if (!Init()) {
        return 1;
    }

    g_bkground = LoadTexture("C:\\Users\\ASUS\\Downloads\\playingBackground.png");
    if (g_bkground == nullptr) {
        return 1;
    }

    SDL_QueryTexture(g_bkground, NULL, NULL, &bkWidth, &bkHeight);
    
    g_food = LoadTexture("C:\\Users\\ASUS\\Downloads\\Food.png");
    if (g_food == nullptr) {
        return 1;
    }

    SDL_QueryTexture(g_food, NULL, NULL, &foodWidth, &foodHeight);
   
    g_snake = LoadTexture("C:\\Users\\ASUS\\Downloads\\2.png");

    SDL_QueryTexture(g_snake, NULL, NULL, &snakeWidth, &snakeHeight);

    if (g_snake == nullptr) {
        return 1;
    }

    while (true) {
        HandleInput();
        MoveSnake(); // Move the snake

        SDL_RenderClear(g_renderer);
        
        ApplyTexture(g_bkground, 0, 0, bkWidth, bkHeight);
       
        ApplyTexture(g_food, foodX, foodY, foodWidth * 3, foodHeight * 3);
        ApplyTexture(g_snake, snakeX, snakeY, snakeWidth * 3, snakeHeight * 3);
        SDL_RenderPresent(g_renderer);
        if (CheckCollision()) {
            EatFood();
            AddTailSegment();
            UpdateTailPosition();
            DrawTail();

        }
        
        SDL_Delay(10); // Adjust delay for smoother movement
    }

    CleanUp();
    return 0;
}
