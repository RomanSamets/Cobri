#include "RenderEngineAPI.h"
#include "game.h"

INIT

int difficultMode = NORMAL;
int spawnFruitFrequency = 3; // every n seconds spawn fruit
int scoresValue = 0;

int startPosition = ((MAXWIDTH/2) + MAXWIDTH*(MAXHEIGHT/2-1));
int startDirection = RIGHT;
int direction;
char stoperChars[] = "|-_+#";
char snakeBody = '+';
char snakeHeadChar = 'o';
char fruitBody = '@';

time_t timer;
int t = 0;

void Start() {
    timer = time(NULL);
    srand(timer);
    direction = startDirection;
    InitializeSnake();
    AddSnakeBody();
    MainMenu();
}

void Update() {
    snakeHead->pos = startPosition;
    frame[snakeHead->pos] = snakeHeadChar;
    Render();

    char inputButton;
    for (;;) {
        timer = time(NULL);

        PrintScores();
        GoSnake(direction, difficultMode);

        if (kbhit()) {
            inputButton = getch();
            
            switch(inputButton) {
                case 'w':
                case 'W':
                case UP_ARROW:
                    direction = (direction != DOWN)? UP : DOWN;
                    break;

                case 's':
                case 'S':
                case DOWN_ARROW:
                    direction = (direction != UP)? DOWN : UP;
                    break;

                case 'a':
                case 'A':
                case LEFT_ARROW:
                    direction = (direction != RIGHT)? LEFT : RIGHT;
                    break;
                    
                case 'd':
                case 'D':
                case RIGHT_ARROW:
                    direction = (direction != LEFT)? RIGHT : LEFT;
                    break;

                case 'q':
                case 'Q':
                case ESCAPE:
                    exit(0);
                    break;
            }
        }
        if ((timer % spawnFruitFrequency) == 0) {
            ++t;
            GenerateFruit();   
        } else {
            t = 0;
        }
    }
}

void PrintScores() {
    int pad = MAXWIDTH+1;
    char* scoresStr = "Scores";
    char buf[3];
    sprintf(buf, "%d", scoresValue);
    
    for (int i = 0; i < strlen(scoresStr); ++i) {
        frame[pad+i] = scoresStr[i];
    }
    for (int i = 0; i < strlen(buf); ++i) {
        frame[pad+1+strlen(scoresStr)+i] = buf[i];        
    }
}

void ResetGame() {
    struct Snake* current = snakeHead;
    snakeHead->pos = startPosition;
    snakeHead->nextBody = NULL;
    direction = startDirection;
    scoresValue = 0;
    
    if (current->nextBody != NULL) {
        current = current->nextBody;
    }
    
    while(current->nextBody != NULL) {
        free(current);
        current = current->nextBody;
    }
    AddSnakeBody();
}

void InitializeSnake() {
    snakeHead = malloc(sizeof(struct Snake));
    snakeHead->pos = startPosition;
    snakeHead->nextBody = NULL;
}

void AddSnakeBody() {
    struct Snake* current = snakeHead;

    while(current->nextBody != NULL) {
        current = current->nextBody;
    }

    current->nextBody = malloc(sizeof(struct Snake));
    if (direction == RIGHT) {
        current->nextBody->pos = current->pos-2;
    }
    if (direction == LEFT) {
        current->nextBody->pos = current->pos+1;
    }
    if (direction == UP) {
        current->nextBody->pos = current->pos+MAXWIDTH;
    }
    if (direction == DOWN) {
        current->nextBody->pos = current->pos-MAXWIDTH;
    }
    current->nextBody->nextBody = NULL;

}

void GoSnake(int direction, int latency) {
    if (direction == UP) {
        Move(UP);
    }
    if (direction == DOWN) {
        Move(DOWN);    
    }
    if (direction == LEFT) {
        Move(LEFT);
    }
    if (direction == RIGHT) {
        Move(RIGHT);
    }
    _sleep(latency);
}

void GenerateFruit() {
    if (t != 1) {return;}
    int fruitPos;

    regenerate:
    int x = rand() % (MAXWIDTH-1);
    if (x == 0) x = 1;
    int y = rand() % (MAXHEIGHT-1);
    if (y == 0) y = 1;

    fruitPos = x + (y*MAXWIDTH);

    // if (fruitPos == snakeHead->pos) {goto regenerate;}
    if (frame[fruitPos] == fruitBody) {goto regenerate;}
    if (y == (snakeHead->pos/MAXWIDTH)) {goto regenerate;}
    if (frame[fruitPos+1] == fruitBody) {goto regenerate;}
    if (frame[fruitPos-1] == fruitBody) {goto regenerate;}
    if (frame[fruitPos+MAXWIDTH] == fruitBody) {goto regenerate;}
    if (frame[fruitPos-MAXWIDTH] == fruitBody) {goto regenerate;}
    for (int i = (MAXWIDTH+1); i < (MAXWIDTH+1+6+3); ++i) {
        if (fruitPos == i) {goto regenerate;}
    }

    frame[fruitPos] = fruitBody;
}

void Move(int direction) {
    struct Snake* current = snakeHead;

    if (direction == UP) {
        int bufPos = snakeHead->pos;
        snakeHead->pos -= MAXWIDTH;
        for (int i = 0; i < strlen(stoperChars); ++i) {
            if (frame[snakeHead->pos] == stoperChars[i]) {
                GameOver();
            }
        }
        if (frame[snakeHead->pos] == fruitBody) {
            AddSnakeBody();
            ++scoresValue;
        }
        frame[snakeHead->pos] = snakeHeadChar;
        if (current->nextBody != NULL) {
            current = current->nextBody;
        }
        int temp = bufPos;
        while (current->nextBody != NULL) {
            temp = current->pos;
            current->pos = bufPos;
            bufPos = temp;
            frame[current->pos] = snakeBody;
            current = current->nextBody;
        }
        frame[temp] = charForFrameSpace;
    }

    if (direction == DOWN) {
        int bufPos = snakeHead->pos;
        snakeHead->pos += MAXWIDTH;
        for (int i = 0; i < strlen(stoperChars); ++i) {
            if (frame[snakeHead->pos] == stoperChars[i]) {
                GameOver();
            }
        }
        if (frame[snakeHead->pos] == fruitBody) {
            AddSnakeBody();
            ++scoresValue;
        }
        frame[snakeHead->pos] = snakeHeadChar;
        if (current->nextBody != NULL) {
            current = current->nextBody;
        }
        int temp = bufPos;
        while (current->nextBody != NULL) {
            temp = current->pos; 
            current->pos = bufPos;
            bufPos = temp;
            frame[current->pos] = snakeBody;
            current = current->nextBody;
        }
        frame[temp] = charForFrameSpace;
    }

    if (direction == LEFT) {
        int bufPos = snakeHead->pos;
        snakeHead->pos -= 1;
        for (int i = 0; i < strlen(stoperChars); ++i) {
            if (frame[snakeHead->pos] == stoperChars[i]) {
                GameOver();
            }
        }
        if (frame[snakeHead->pos] == fruitBody) {
            AddSnakeBody();
            ++scoresValue;
        }
        frame[snakeHead->pos] = snakeHeadChar;
        if (current->nextBody != NULL) {
            current = current->nextBody;
        }
        int temp = bufPos;
        while (current->nextBody != NULL) {
            temp = current->pos;
            current->pos = bufPos;
            bufPos = temp;
            frame[current->pos] = snakeBody;
            current = current->nextBody;
        }
        frame[temp] = charForFrameSpace;
    }

    if (direction == RIGHT) {
        int bufPos = snakeHead->pos;
        snakeHead->pos += 1;
        for (int i = 0; i < strlen(stoperChars); ++i) {
            if (frame[snakeHead->pos] == stoperChars[i]) {
                GameOver();
            }
        }
        if (frame[snakeHead->pos] == fruitBody) {
            AddSnakeBody();
            ++scoresValue;
        }
        frame[snakeHead->pos] = snakeHeadChar;
        if (current->nextBody != NULL) {
            current = current->nextBody;
        }
        int temp = bufPos;
        while (current->nextBody != NULL) {
            temp = current->pos; 
            current->pos = bufPos;
            bufPos = temp;
            frame[current->pos] = snakeBody;
            current = current->nextBody;
        }
        frame[temp] = charForFrameSpace;
    }

    PrintEdges();
    Render();
}

void GameOver() {
    GameOverMenu();
    PrintScores();
}

// 01100100 01110001 01101101 01110010 01100100 01110001 01101110 01100100 00100000 01101011 01110000 01110000