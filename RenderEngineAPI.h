#ifndef RenderEngineAPI_H_
#define RenderEngineAPI_H_

#define CSI "\x1b["

#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define INIT int main(int argc, char const *argv[]) { \
	StartEngine();   \
	Start();         \
	return 0;        \
}

#define UP_ARROW 72
#define DOWN_ARROW 80
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define ESCAPE 27
#define ENTER 13

#define MAXWIDTH (66+1)
#define MAXHEIGHT (20+1)
#define FRAMESIZE (MAXWIDTH*MAXHEIGHT)
#define EDGESCOUNT (((MAXHEIGHT+MAXWIDTH)*2))

enum alignmentModes {
	TOLEFT,
	TOCENTER
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

extern char charForFrameSpace;

extern int edges[EDGESCOUNT];
extern char edgesChars[];

extern int alignmentMode;
extern int terminalWidth;

void Start();
void Update();

void StartEngine();
void RenderFrameInit();
void PrintAnimation(struct animation* anim, int startPos, int fps, float duration);
void Render();
void PrintEdges();

#endif // RenderEngineAPI_H_