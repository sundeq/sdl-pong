#include <SDL2/SDL.h>
#include <math.h>

typedef struct GameObject GameObject;

struct GameObject {
	int r;
	int  g;
	int  b;
	int alpha;
	SDL_Rect *rect;
};

GameObject *initGameObject(int x, int y, int w, int h);
void freeGameObject(GameObject *paddle);
GameObject *setColor(GameObject *go, int r, int g, int b, int alpha);
void render(SDL_Renderer *renderer, GameObject *leftPaddle, GameObject *rightPaddle, GameObject *background);

int main() {

	SDL_Window *window;
	SDL_Renderer *renderer;

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Could not initialize SDL: %s", SDL_GetError());
		return 1;
	}
	
	int windowWidth = 1280;
	int windowHeight = 980;
	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	GameObject *background = initGameObject(0, 0, windowWidth, windowHeight);
	background = setColor(background, 147, 112, 219, 255);

	int paddleWidth = 10;
	int paddleHeight = 150;
	int sideOffset = 100;
	GameObject *leftPaddle = initGameObject(sideOffset, (windowHeight / 2) - (paddleHeight / 2), paddleWidth, paddleHeight);
	leftPaddle = setColor(leftPaddle, 255, 255, 255, 255);
	GameObject *rightPaddle = initGameObject(windowWidth-sideOffset, (windowHeight / 2) - (paddleHeight / 2), paddleWidth, paddleHeight);
	rightPaddle = setColor(rightPaddle, 255, 255, 255, 255);
	
	SDL_Event sdlEvent;

	int runGame = 1;

	while (runGame) {
		while (SDL_PollEvent(&sdlEvent)) {	
			if (sdlEvent.type == SDL_QUIT) {
				runGame = 0;
			}
		}

		render(renderer, leftPaddle, rightPaddle, background);
	}

	freeGameObject(background);
	freeGameObject(leftPaddle);
	freeGameObject(rightPaddle);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

GameObject *initGameObject(int x, int y, int w, int h) {
	GameObject *gameObject = (GameObject *) malloc(sizeof(GameObject));
	gameObject->rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	gameObject->rect->x = x;
	gameObject->rect->y = y;
	gameObject->rect->w = w;
	gameObject->rect->h = h;
	return gameObject;
}

void freeGameObject(GameObject *gameObject) {
	free(gameObject->rect);
	free(gameObject);
}

GameObject *setColor(GameObject *go, int  r, int g, int  b, int alpha) {
	go->r = r;
	go->g = g;
	go->b = b;
	go->alpha = alpha;
	return go;
}

void render(SDL_Renderer *renderer, GameObject *leftPaddle, GameObject *rightPaddle, GameObject *background) {
	
	SDL_SetRenderDrawColor(renderer,
			background->r,
			background->g,
			background->b,
			background->alpha);
	SDL_RenderFillRect(renderer, background->rect);

	SDL_SetRenderDrawColor(renderer,
			leftPaddle->r,
			leftPaddle->g,
			leftPaddle->b,
			leftPaddle->alpha);
	SDL_RenderFillRect(renderer, leftPaddle->rect);

	SDL_SetRenderDrawColor(renderer,
			rightPaddle->r,
			rightPaddle->g,
			rightPaddle->b,
			rightPaddle->alpha);
	SDL_RenderFillRect(renderer, rightPaddle->rect);

	SDL_RenderPresent(renderer);
}

