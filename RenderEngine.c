#include "RenderEngineAPI.h"

char frame[FRAMESIZE];
char charForFrameSpace = ' ';
char edgesChars[] = "-|+";
char buffer[FRAMESIZE]; 
int edges[EDGESCOUNT];
int alignmentMode = TOCENTER;
static int width = MAXWIDTH;
int terminalWidth = 120;

void StartEngine() {
	RenderFrameInit();
	PrintEdges();
	Render();
	time_t t;
	srand((unsigned) time(&t)); 
}

void RenderFrameInit() {
	fprintf(stdout, CSI"?25l"); // hide cursor
	fprintf(stdout, CSI"2J"); // clear screen
	memset(frame, charForFrameSpace, FRAMESIZE);
}

void PrintEdges() {
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
	
	// Initialize edges
	
	// top side
	for (int tsi = 0; tsi < MAXWIDTH; ++tsi) {
		edges[tsi] = tsi;
	}

	// bottom side
	int bsi=MAXWIDTH; 
	while (bsi < (MAXWIDTH*2)) {
		for (int i = (MAXWIDTH*(MAXHEIGHT-1)); i < (MAXWIDTH*MAXHEIGHT); ++i) {
			edges[bsi] = i;
			++bsi;
		}
	}

	// left side
	int lsi = (MAXWIDTH*2); 
	while(lsi < ((MAXWIDTH*2)+MAXHEIGHT)) {
		for (int i = 1; i <= MAXHEIGHT; ++i) {
			edges[lsi] = ((MAXWIDTH*i));
			++lsi;
		}
	}

	// right side
	int rsi = ((MAXWIDTH*2)+MAXHEIGHT); 
	while(rsi < ((MAXWIDTH*2)+(MAXHEIGHT*2))) {
		for (int i = 1; i <= MAXHEIGHT; ++i) {
			edges[rsi] = (MAXWIDTH*i)-1;
			++rsi;
		}
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

void Render() { 
    setvbuf(stdout, buffer, _IOFBF, FRAMESIZE);
	fprintf(stdout, CSI"H"); // rewrite frame

	for (int i = 0; i < FRAMESIZE; ++i) {
		if ((i % MAXWIDTH) == 0) {
			fprintf(stdout, "\n");
			
			if (alignmentMode == TOCENTER) {
				for (int i = 0; i < (terminalWidth - (MAXWIDTH+1))/2; ++i) {
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