#include "RenderEngine.h"
#include "game.h"
#include "menu.h"

#include "words.h"

static char* mainMenuOptionsStr = "Play Exit";
static char* endMenuOptionsStr = "Replay Menu";
static int optCount = 0;

void MainMenu() {
	PrintWord(gameName);

	int selectedOption = 0;

	mainMenu.alignment = TO_CENTER;
	strcpy(mainMenu.options, mainMenuOptionsStr);
	
	PrintMenuOptions(selectedOption, mainMenu);
	Render();

	char inputButton;
	FOREVER {
		if (kbhit()) {
			inputButton = getch();
			switch(inputButton) {
				case 'w':
				case 'W':
				case UP_ARROW:
					--selectedOption;
					if (selectedOption < 0) {selectedOption = 0;continue;}

					PrintMenuOptions(selectedOption, mainMenu);
					Render();
					break;

				case 's':
				case 'S':
				case DOWN_ARROW:
					++selectedOption;
					if (selectedOption > optCount-1) {selectedOption = optCount-1;continue;}

					PrintMenuOptions(selectedOption, mainMenu);
					Render();
					break;

				case ENTER:
					if (selectedOption == 0) {
						memset(frame, charForFrameSpace, FRAMESIZE);
						PrintEdges();
						Render();
						ResetGame();
						Update();
					}
				
					if (selectedOption == 1) {printf(CSI"?25h");exit(0);}
					break;
			}
		}
	}
}

void GameOverMenu() {
	memset(frame, charForFrameSpace, FRAMESIZE);
	PrintEdges();
	PrintWord(gameOver);
	PrintScores();

	int selectedOption = 0;

	endMenu.alignment = TO_CENTER;
	strcpy(endMenu.options, endMenuOptionsStr);

	PrintMenuOptions(selectedOption, endMenu);
	Render();

	char inputButton;
	FOREVER {
		if (kbhit()) {
			inputButton = getch();

			switch(inputButton) {
				case 'w':
				case 'W':
				case UP_ARROW:
					--selectedOption;
					if (selectedOption < 0) {selectedOption = 0;continue;}

					PrintMenuOptions(selectedOption, endMenu);
					Render();
					break;

				case 's':
				case 'S':
				case DOWN_ARROW:
					++selectedOption;
					if (selectedOption > optCount-1) {selectedOption = optCount-1;continue;}

					PrintMenuOptions(selectedOption, endMenu);
					Render();
					break;

				case ENTER:
					if (selectedOption == 0) {
						memset(frame, charForFrameSpace, FRAMESIZE);
						PrintEdges();
						Render();
						ResetGame();
						Update();
					}
					
					if (selectedOption == 1) {
						memset(frame, charForFrameSpace, FRAMESIZE);
						PrintEdges();
						Render();
						MainMenu();
					}
					break;
			}
		}
	}
}
















void PrintMenuOptions(int selectedOption, struct Menu menu) {
	int textPad = 0;
	if (menu.alignment == TO_CENTER) {
		textPad = MAXWIDTH / 2; 		
	}
	
	if (menu.alignment == TO_LEFT) {
		textPad = MAXWIDTH/10;
	}

	int offsetBetweenObjects = 9;
	int offsetBetweenOptions = 0;
	int offsetSelectedSymbol;

	menu.optionsCount = 0;

	char* selectIndicatorSymbol = ">";
	offsetSelectedSymbol = -2 - (strlen(selectIndicatorSymbol)-1);

	char* word = strtok(menu.options, " ");
	while (word != NULL) {
		if (selectedOption == menu.optionsCount) {
			for (int i = 0; i < strlen(selectIndicatorSymbol); ++i) {
				frame[textPad + i + offsetSelectedSymbol+MAXWIDTH*(offsetBetweenObjects+offsetBetweenOptions)-((globalAlignmentMode == TO_CENTER)? (strlen(word)/2) : 0)] = selectIndicatorSymbol[i];				
			}
		} else {
			for (int i = 0; i < strlen(selectIndicatorSymbol); ++i) {
				frame[textPad + i + offsetSelectedSymbol+MAXWIDTH*(offsetBetweenObjects+offsetBetweenOptions)-((globalAlignmentMode == TO_CENTER)? (strlen(word)/2) : 0)] = charForFrameSpace;			
			}
		}

		for (int o = textPad; o < (textPad+strlen(word)); ++o) {
			frame[o+MAXWIDTH*(offsetBetweenObjects+offsetBetweenOptions)-((globalAlignmentMode == TO_CENTER)? (strlen(word)/2) : 0)] = word[o-textPad];
		}

		offsetBetweenOptions+=2;

		word = strtok(NULL, " ");

		menu.optionsCount+=1;
		optCount = menu.optionsCount;
	}
}