#ifndef RenderEngine_H_
#define RenderEngine_H_

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <conio.h>

#define CSI "\x1b["

#define UP_ARROW 72
#define DOWN_ARROW 80
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define ESCAPE 27
#define ENTER 13

#define MAXWIDTH (66+1)
#define MAXHEIGHT (20+1)
#define FRAMESIZE (MAXWIDTH*MAXHEIGHT)

#define TERMINAL_WIDTH 120

#define MAX_ASCII_TEXT_ROWS 6

#define FOREVER for(;;)

enum alignmentModes {
	TO_LEFT,
	TO_CENTER
};

enum edgesCharsElements {
	TOPBOTTOM,
	LEFTRIGHT,
	CORNERS
};

struct animation {
    int width;
    int height;
    int frames;
    char* body;
};

extern char frame[FRAMESIZE];
const extern char charForFrameSpace;
extern char* edgesChars;
extern int globalAlignmentMode;

void Start();
void Update();

void StartEngine();

void RenderFrameInit();
void Render();

void PrintEdges();
void PrintWord(char* word);
// void PrintAnimation(struct animation* anim, int startPos, int fps, float duration);

#endif // RenderEngine_H_