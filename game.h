#ifndef game_H_
#define game_H_

#define EASY 200
#define NORMAL 100
#define HARD 50

#define START_DIRECTION RIGHT
#define START_POSITION ((MAXWIDTH/2) + MAXWIDTH*(MAXHEIGHT/2-1))
#define DEFAULT_DIFFICULT_MODE NORMAL
#define DEFAULT_FRUIT_SPAWN_FREQUENCY 3

struct Snake {
    int position;
    struct Snake* nextBody;
};

extern struct Snake* snakeHead;

enum movements {
	RIGHT,
	LEFT,
	UP,
	DOWN
};

void Move();
void ResetGame();
void GameOver();
void InitializeSnake();
void AddSnakeBody();
void GenerateFruit();
void PrintScores();

#endif // game_H_