#ifndef menu_H_
#define menu_H_

static struct Menu {
	int optionsCount;
	int alignment;
	char options[20];
} mainMenu, endMenu;

void MainMenu();
void GameOverMenu();
void PrintMenuOptions(int selectedOption, struct Menu menu);

#endif // menu_H_