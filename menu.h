#ifndef menu_H_
#define menu_H_

#define MAXASCIITEXTROWS 6

void PrintWord(char* word);

extern char* gameName;
extern char* gameOver;

static char* mainMenuOptionsStr = "Play Exit";
static char* endMenuOptionsStr = "Replay Menu";

static int optCount = 0;

static struct Menu {
	int optionsCount;
	int alignment;
	char options[20];
} mainMenu, endMenu;

void MainMenu();
void ResetGame();
void PrintScores();
void GameOverMenu();
void PrintMenuOptions(int selectedOption, struct Menu menu);

#endif // menu_H_