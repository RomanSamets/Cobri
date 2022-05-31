#ifndef game_H_
#define game_H_

#include "menu.h"

#define EASY 200
#define NORMAL 100
#define HARD 50

struct Snake {
    int pos;
    struct Snake* nextBody;
};

struct Snake* snakeHead;

enum movements {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

void Move(int direction);
void ResetGame();
void GameOver();
void InitializeSnake();
void AddSnakeBody();
void GoSnake(int direction, int latency);
void GenerateFruit();
void PrintScores();

#endif // game_H_