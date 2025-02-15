#include <SDL2/SDL.h>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <string>

// Enum for snake movement directions
enum class Direction {
    NONE, UP, DOWN, LEFT, RIGHT
};

// Represents a single segment of the snake's body
class SnakeSegment {
public:
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

// Main snake class managing movement and collision
class Snake {
public:
    std::vector<SnakeSegment> segments;  // Snake body segments
    Direction direction;                  // Current movement direction
    Direction nextDirection;             // Buffered next direction
    bool hasStarted;                     // Game state flag
    int score;                           // Current score

    // Initialize snake with single segment at (7,7)
    Snake() {
        segments.push_back(SnakeSegment(7, 7));
        direction = Direction::NONE;
        nextDirection = Direction::NONE;
        hasStarted = false;
        score = 0;
    }

    // Update snake position based on current direction
    void move() {
        if (!hasStarted || direction == Direction::NONE) {
            return;
        }

        // Apply buffered direction change
        if (nextDirection != Direction::NONE) {
            direction = nextDirection;
            nextDirection = Direction::NONE;
        }

        // Calculate new head position
        int newX = segments[0].x;
        int newY = segments[0].y;

        switch (direction) {
            case Direction::UP:    newY--; break;
            case Direction::DOWN:  newY++; break;
            case Direction::LEFT:  newX--; break;
            case Direction::RIGHT: newX++; break;
            case Direction::NONE:  break;
        }

        // Add new head and remove tail
        segments.insert(segments.begin(), SnakeSegment(newX, newY));
        segments.pop_back();
    }

    // Check for wall and self collisions
    bool checkCollision() {
        // Wall collision
        if (segments[0].x < 0 || segments[0].x >= 15 ||
            segments[0].y < 0 || segments[0].y >= 15) {
            return true;
        }

        // Self collision
        for (size_t i = 1; i < segments.size(); i++) {
            if (segments[0].x == segments[i].x && 
                segments[0].y == segments[i].y) {
                return true;
            }
        }
        return false;
    }
};

// Apple (food) class
class Apple {
public:
    int x, y;

    Apple() {
        randomize();
    }

    // Place apple at random position
    void randomize() {
        x = rand() % 15;
        y = rand() % 15;
    }
};

// Manages high score persistence
class ScoreManager {
public:
    // Read high score from file
    static int getHighScore() {
        std::ifstream file("highscore.txt");
        int highScore = 0;
        if (file.is_open()) {
            file >> highScore;
            file.close();
        }
        return highScore;
    }

    // Save new high score if better than current
    static void saveHighScore(int score) {
        int currentHigh = getHighScore();
        if (score > currentHigh) {
            std::ofstream file("highscore.txt");
            if (file.is_open()) {
                file << score;
                file.close();
            }
        }
    }
};

int main() {
    srand(time(nullptr));  // Initialize random seed

    // Initialize SDL and create window
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Snake Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        480, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Initialize game objects
    Snake snake;
    Apple apple;
    bool running = true;
    int gridSize = 32;  // Size of each grid cell
    int currentHighScore = ScoreManager::getHighScore();
    
    // Set initial window title with scores
    char windowTitle[100];
    snprintf(windowTitle, sizeof(windowTitle), "Snake Game - Score: 0 High Score: %d", currentHighScore);
    SDL_SetWindowTitle(window, windowTitle);

    // Main game loop
    while (running) {
        // Handle input events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                // Handle direction changes
                // Handle keyboard input for snake movement
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    
                        // Prevent 180-degree turns by checking if not moving down
                        if (snake.direction != Direction::DOWN) {
                            if (!snake.hasStarted) {
                                // If game hasn't started, set initial direction
                                snake.direction = Direction::UP;
                                snake.hasStarted = true;
                            } else {
                                // Buffer the direction change for next movement
                                snake.nextDirection = Direction::UP;
                            }
                        }
                        break;
                    case SDLK_DOWN:  
                        // Prevent 180-degree turns by checking if not moving up
                        if (snake.direction != Direction::UP) {
                            if (!snake.hasStarted) {
                                // If game hasn't started, set initial direction
                                snake.direction = Direction::DOWN;
                                snake.hasStarted = true;
                            } else {
                                // Buffer the direction change for next movement
                                snake.nextDirection = Direction::DOWN;
                            }
                        }
                        break;
                    case SDLK_LEFT:  
                        // Prevent 180-degree turns by checking if not moving right
                        if (snake.direction != Direction::RIGHT) {
                            if (!snake.hasStarted) {
                                // If game hasn't started, set initial direction
                                snake.direction = Direction::LEFT;
                                snake.hasStarted = true;
                            } else {
                                // Buffer the direction change for next movement
                                snake.nextDirection = Direction::LEFT;
                            }
                        }
                        break;
                    case SDLK_RIGHT: 
                        // Prevent 180-degree turns by checking if not moving left
                        if (snake.direction != Direction::LEFT) {
                            if (!snake.hasStarted) {
                                // If game hasn't started, set initial direction
                                snake.direction = Direction::RIGHT;
                                snake.hasStarted = true;
                            } else {
                                // Buffer the direction change for next movement
                                snake.nextDirection = Direction::RIGHT;
                            }
                        }
                        break;
                }
            }
        }

        snake.move();

        // Check for game over
        if (snake.checkCollision()) {
            ScoreManager::saveHighScore(snake.score);
            running = false;
            continue;
        }

        // Check for apple collision and update score
        if (snake.segments[0].x == apple.x && snake.segments[0].y == apple.y) {
            snake.segments.push_back(SnakeSegment(
                snake.segments.back().x,
                snake.segments.back().y
            ));
            snake.score++;
            if (snake.score > currentHighScore) {
                currentHighScore = snake.score;
                ScoreManager::saveHighScore(currentHighScore);
            }
            snprintf(windowTitle, sizeof(windowTitle), "Snake Game - Score: %d High Score: %d", 
                    snake.score, currentHighScore);
            SDL_SetWindowTitle(window, windowTitle);
            apple.randomize();
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw apple
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect appleRect = {
            apple.x * gridSize,
            apple.y * gridSize,
            gridSize,
            gridSize
        };
        SDL_RenderFillRect(renderer, &appleRect);

        // Draw snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (const auto& segment : snake.segments) {
            SDL_Rect segmentRect = {
                segment.x * gridSize,
                segment.y * gridSize,
                gridSize,
                gridSize
            };
            SDL_RenderFillRect(renderer, &segmentRect);
        }

        // Update screen and control game speed
        SDL_RenderPresent(renderer);
        SDL_Delay(170);
    }

    // Cleanup SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}