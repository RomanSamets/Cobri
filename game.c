#include "RenderEngine.h"
#include "game.h"
#include "menu.h"

struct system_options {
    int direction;
    int scoresValue;
    int isSpawnFruit;
    const char snakeBody;
    const char snakeHeadChar;
    const char fruitBody;
    char* stopChars;
} system_options = {
    .direction = START_DIRECTION,
    .scoresValue = 0,
    .isSpawnFruit = false,
    .snakeBody = '+',
    .snakeHeadChar = 'o',
    .fruitBody = '@',
    .stopChars = "|-_+#"
};

struct user_options {
    int difficultMode;
    int spawnFruitFrequency; // every n seconds spawn fruit
} user_options = {
    .difficultMode = DEFAULT_DIFFICULT_MODE,
    .spawnFruitFrequency = DEFAULT_FRUIT_SPAWN_FREQUENCY
};

struct Snake* snakeHead;

void Start() {
    InitializeSnake();
    AddSnakeBody();
    MainMenu();
}

void Update() {
    char inputButton;
    time_t timer;

    FOREVER {
        timer = time(NULL);
        PrintScores();

        Move();

        if (kbhit()) {
            inputButton = getch();
            
            switch(inputButton) {
                case 'd':
                case 'D':
                case RIGHT_ARROW:
                    system_options.direction = (system_options.direction != LEFT)? RIGHT : LEFT;
                    break;

                case 'a':
                case 'A':
                case LEFT_ARROW:
                    system_options.direction = (system_options.direction != RIGHT)? LEFT : RIGHT;
                    break;

                case 'w':
                case 'W':
                case UP_ARROW:
                    system_options.direction = (system_options.direction != DOWN)? UP : DOWN;
                    break;

                case 's':
                case 'S':
                case DOWN_ARROW:
                    system_options.direction = (system_options.direction != UP)? DOWN : UP;
                    break;

                case 'q':
                case 'Q':
                case ESCAPE:
                    exit(EXIT_SUCCESS);
                    break;
            }
        }

        if ((timer % user_options.spawnFruitFrequency) == 0) {
            ++system_options.isSpawnFruit;
            GenerateFruit();   
        } else {
            system_options.isSpawnFruit = false;
        }
    }
}

void PrintScores() {
    int padding = MAXWIDTH+1;
    char* scoresStr = "Scores";
    char scoresValueStr[10];
    sprintf(scoresValueStr, "%d", system_options.scoresValue);
    
    for (int i = 0; i < strlen(scoresStr); ++i) {
        frame[padding+i] = scoresStr[i];
    }
    for (int i = 0; i < strlen(scoresValueStr); ++i) {
        frame[padding+1+strlen(scoresStr)+i] = scoresValueStr[i];        
    }
}

void ResetGame() {
    struct Snake* current = snakeHead;

    snakeHead->position = START_POSITION;
    snakeHead->nextBody = NULL;
    system_options.direction = START_DIRECTION;
    system_options.scoresValue = 0;
    
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
    snakeHead->position = START_POSITION;
    snakeHead->nextBody = NULL;
}

void AddSnakeBody() {
    struct Snake* current = snakeHead;

    while(current->nextBody != NULL) {
        current = current->nextBody;
    }

    current->nextBody = malloc(sizeof(struct Snake));

    switch(system_options.direction) {
        case RIGHT:
            current->nextBody->position = current->position-2;
            break;

        case LEFT:
            current->nextBody->position = current->position+1;
            break;

        case UP:
            current->nextBody->position = current->position+MAXWIDTH;
            break;

        case DOWN:
            current->nextBody->position = current->position-MAXWIDTH;
            break;
    }
    current->nextBody->nextBody = NULL;
}

void GenerateFruit() {
    if (system_options.isSpawnFruit != true) {return;}
    int fruitPosition;

    regenerate:
    int x = ((rand() % (MAXWIDTH-1)) == 0)? 1 : rand() % (MAXWIDTH-1);
    int y = ((rand() % (MAXHEIGHT-1)) == 0)? 1 : rand() % (MAXHEIGHT-1);

    fruitPosition = x + (y*MAXWIDTH);

    if    ( frame[fruitPosition] == system_options.fruitBody          || 
            y == (snakeHead->position/MAXWIDTH)                       ||
            frame[fruitPosition+1] == system_options.fruitBody        ||
            frame[fruitPosition-1] == system_options.fruitBody        ||
            frame[fruitPosition+MAXWIDTH] == system_options.fruitBody ||
            frame[fruitPosition-MAXWIDTH] == system_options.fruitBody ) {
        goto regenerate;
    }

    for (int i = (MAXWIDTH+1); i < (MAXWIDTH+1+6+3); ++i) {
        if (fruitPosition == i) {goto regenerate;}
    }

    frame[fruitPosition] = system_options.fruitBody;
}

void Move() {
    struct Snake* current = snakeHead;
    int headPosition = 0;
    int temp = 0;

    switch(system_options.direction) {
        case RIGHT:
            headPosition = snakeHead->position;
            snakeHead->position += 1;
            for (int i = 0; i < strlen(system_options.stopChars); ++i) {
                if (frame[snakeHead->position] == system_options.stopChars[i]) {
                    GameOver();
                }
            }
            if (frame[snakeHead->position] == system_options.fruitBody) {
                AddSnakeBody();
                ++system_options.scoresValue;
            }
            frame[snakeHead->position] = system_options.snakeHeadChar;
            if (current->nextBody != NULL) {
                current = current->nextBody;
            }
            temp = headPosition;
            while (current->nextBody != NULL) {
                temp = current->position; 
                current->position = headPosition;
                headPosition = temp;
                frame[current->position] = system_options.snakeBody;
                current = current->nextBody;
            }
            frame[temp] = charForFrameSpace;
            break;

        case LEFT:
            headPosition = snakeHead->position;
            snakeHead->position -= 1;
            for (int i = 0; i < strlen(system_options.stopChars); ++i) {
                if (frame[snakeHead->position] == system_options.stopChars[i]) {
                    GameOver();
                }
            }
            if (frame[snakeHead->position] == system_options.fruitBody) {
                AddSnakeBody();
                ++system_options.scoresValue;
            }
            frame[snakeHead->position] = system_options.snakeHeadChar;
            if (current->nextBody != NULL) {
                current = current->nextBody;
            }
            temp = headPosition;
            while (current->nextBody != NULL) {
                temp = current->position;
                current->position = headPosition;
                headPosition = temp;
                frame[current->position] = system_options.snakeBody;
                current = current->nextBody;
            }
            frame[temp] = charForFrameSpace;
            break;

        case UP:
            headPosition = snakeHead->position;
            snakeHead->position -= MAXWIDTH;
            for (int i = 0; i < strlen(system_options.stopChars); ++i) {
                if (frame[snakeHead->position] == system_options.stopChars[i]) {
                    GameOver();
                }
            }
            if (frame[snakeHead->position] == system_options.fruitBody) {
                AddSnakeBody();
                ++system_options.scoresValue;
            }
            frame[snakeHead->position] = system_options.snakeHeadChar;
            if (current->nextBody != NULL) {
                current = current->nextBody;
            }
            temp = headPosition;
            while (current->nextBody != NULL) {
                temp = current->position;
                current->position = headPosition;
                headPosition = temp;
                frame[current->position] = system_options.snakeBody;
                current = current->nextBody;
            }
            frame[temp] = charForFrameSpace;
            break;

        case DOWN:
            headPosition = snakeHead->position;
            snakeHead->position += MAXWIDTH;
            for (int i = 0; i < strlen(system_options.stopChars); ++i) {
                if (frame[snakeHead->position] == system_options.stopChars[i]) {
                    GameOver();
                }
            }
            if (frame[snakeHead->position] == system_options.fruitBody) {
                AddSnakeBody();
                ++system_options.scoresValue;
            }
            frame[snakeHead->position] = system_options.snakeHeadChar;
            if (current->nextBody != NULL) {
                current = current->nextBody;
            }
            temp = headPosition;
            while (current->nextBody != NULL) {
                temp = current->position; 
                current->position = headPosition;
                headPosition = temp;
                frame[current->position] = system_options.snakeBody;
                current = current->nextBody;
            }
            frame[temp] = charForFrameSpace;
            break;
    }

    PrintEdges();
    Render();
    _sleep(user_options.difficultMode);
}

void GameOver() {
    GameOverMenu();
    PrintScores();
}

int main(int argc, char const *argv[]) {
    StartEngine();          
    Start();

    return EXIT_SUCCESS;       
}

// 01100100 01110001 01101101 01110010 01100100 01110001 01101110 01100100 00100000 01101011 01110000 01110000