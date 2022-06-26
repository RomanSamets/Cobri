#include "RenderEngine.h"

char frame[FRAMESIZE];
const char charForFrameSpace = ' ';
int globalAlignmentMode = TO_CENTER;

void StartEngine() {
	RenderFrameInit();
	PrintEdges();
	Render();
}

void RenderFrameInit() {
	fprintf(stdout, CSI"?25l"); // hide cursor
	fprintf(stdout, CSI"2J"); // clear screen
	memset(frame, charForFrameSpace, FRAMESIZE);
}

void PrintEdges() {
	char* edgesChars = "-|+";

	// Corners
	frame[0] = edgesChars[CORNERS]; // left top
	frame[MAXWIDTH-1] = edgesChars[CORNERS]; // right top
	frame[MAXWIDTH*(MAXHEIGHT-1)] = edgesChars[CORNERS]; // left bottom
	frame[MAXWIDTH*MAXHEIGHT-1] = edgesChars[CORNERS]; // right bottom

	for (int i = 1; i < MAXWIDTH-1; ++i) {
		frame[i] = edgesChars[TOPBOTTOM];
	}
	
	for (int i = 1; i < MAXHEIGHT-1; ++i) {
		frame[MAXWIDTH*i] = edgesChars[LEFTRIGHT];
	}
	
	for (int i = 1; i < MAXHEIGHT-1; ++i) {
		frame[MAXWIDTH*i+(MAXWIDTH-1)] = edgesChars[LEFTRIGHT];
	}
	
	for (int i = (MAXWIDTH*(MAXHEIGHT-1))+1; i < ((MAXWIDTH*MAXHEIGHT)-1); ++i) {
		frame[i] = edgesChars[TOPBOTTOM];
	}
}

// TODO: add universality with len. Input strRowLen can be different value. 
void PrintAnimation(struct animation* anim, int startPos, int fps, float duration) { // duration in sec
    float timeInitial, timeMeasured, timeDelta = 0;
    timeInitial = (float)clock();
    while (timeDelta < duration) {
        for (int c = 0; c < anim->frames; ++c) {
            for (int h = 0; h < anim->height; ++h) {
                for (int w = 0; w < anim->width; ++w) {
                    frame[startPos+ w+h*MAXWIDTH] = anim->body[w+(h*anim->width)+(c*(anim->width*anim->height))];      
                }
            }
            _sleep((1000/fps));
            Render(); 
        }
        timeMeasured = (float)clock();
        timeDelta = ((timeMeasured - timeInitial) / (float)CLOCKS_PER_SEC);
    }
}

void PrintWord(char* word) {
    int rows = MAX_ASCII_TEXT_ROWS;
    int wordLen = strlen(word);
	int oneLineLen = wordLen/rows;
    int wordPadding = (MAXWIDTH - (wordLen/rows) +1) / 2 + MAXWIDTH; //  // (/2) it's alignment to center 1 for left 	

    int ly = 0;
    int lw = 0;
    for (int w = 0; w < wordLen; ++w) {
        if ((w % oneLineLen)==0) { // && w != 0 // 
            lw = 0;
            ly += MAXWIDTH;
        }
        if (word[w] == '.' && charForFrameSpace != '.') {
        	frame[lw + ly + wordPadding] = charForFrameSpace;
        } else {
        	frame[lw + ly + wordPadding] = word[w];
        }
        
        ++lw;
    }
}

void Render() { 
	static int width = MAXWIDTH;
	static char buffer[FRAMESIZE]; 
    setvbuf(stdout, buffer, _IOFBF, FRAMESIZE);
	fprintf(stdout, CSI"H");

	for (int i = 0; i < FRAMESIZE; ++i) {
		if ((i % MAXWIDTH) == 0) {
			fprintf(stdout, "\n");
			
			if (globalAlignmentMode == TO_CENTER) {
				for (int i = 0; i < (TERMINAL_WIDTH - (MAXWIDTH+1))/2; ++i) {
					fprintf(stdout," ");
				}
			}
			
			width += width;
		}
		fprintf(stdout, "%c", frame[i]);
	}
	
	width=MAXWIDTH;
	fflush(stdout);
}