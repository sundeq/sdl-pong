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
void render(SDL_Renderer *renderer, GameObject *leftPaddle, GameObject *rightPaddle, GameObject *background, GameObject *ball);
int collidesWithTop(GameObject *go);
int collidesWithBottom(int windowHeight, GameObject *go);

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
	int ballSide = 10;
	int sideOffset = 100;

	GameObject *leftPaddle = initGameObject(
			sideOffset,
			(windowHeight / 2) - (paddleHeight / 2),
			paddleWidth,
			paddleHeight);
	leftPaddle = setColor(leftPaddle, 255, 255, 255, 255);
	int leftPaddleDY;
	
	GameObject *rightPaddle = initGameObject(
			windowWidth-sideOffset,
			(windowHeight / 2) - (paddleHeight / 2),
			paddleWidth,
			paddleHeight);
	rightPaddle = setColor(rightPaddle, 255, 255, 255, 255);
	int rightPaddleDY;

	GameObject *ball = initGameObject(
			(windowWidth / 2) - (ballSide / 2),
			(windowHeight / 2) - (ballSide / 2),
			ballSide,
			ballSide);
	ball = setColor(ball, 255, 255, 255, 255);
	int ballSpeed;
	int ballDX;
	int ballDY;
	
	SDL_Event sdlEvent;

	const Uint8 *keyboardState;

	int runGame = 1;

	while (runGame) {
	
		leftPaddleDY = 0;
		rightPaddleDY = 0;

		keyboardState = SDL_GetKeyboardState(NULL);

		if (keyboardState[SDL_SCANCODE_W]) {
			leftPaddleDY = -1;
		}

		if (keyboardState[SDL_SCANCODE_S]) {
			leftPaddleDY = 1;
		}
		
		if (keyboardState[SDL_SCANCODE_I]) {
			rightPaddleDY = -1;
		}

		if (keyboardState[SDL_SCANCODE_K]) {
			rightPaddleDY = 1;
		}

		while (SDL_PollEvent(&sdlEvent)) {	
			if (sdlEvent.type == SDL_QUIT) {
				runGame = 0;
			}
		}

		if (collidesWithTop(leftPaddle) && leftPaddleDY == -1) {
			leftPaddleDY = 0;
		}
		
		if (collidesWithBottom(windowHeight, leftPaddle) && leftPaddleDY == 1) {
			leftPaddleDY = 0;
		}
		
		if (collidesWithTop(rightPaddle) && rightPaddleDY == -1) {
			rightPaddleDY = 0;
		}
		
		if (collidesWithBottom(windowHeight, rightPaddle) && rightPaddleDY == 1) {
			rightPaddleDY = 0;
		}
		
		leftPaddle->rect->y += leftPaddleDY;
		rightPaddle->rect->y += rightPaddleDY;
		render(renderer, leftPaddle, rightPaddle, background, ball);
	}

	freeGameObject(background);
	freeGameObject(leftPaddle);
	freeGameObject(rightPaddle);
	freeGameObject(ball);

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

void render(SDL_Renderer *renderer, GameObject *leftPaddle, GameObject *rightPaddle, GameObject *background, GameObject *ball) {
	
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
	
	SDL_SetRenderDrawColor(renderer,
			ball->r,
			ball->g,
			ball->b,
			ball->alpha);
	SDL_RenderFillRect(renderer, ball->rect);

	SDL_RenderPresent(renderer);
}

int collidesWithTop(GameObject *go) {
	return go->rect->y == 1;	
}

int collidesWithBottom(int windowHeight, GameObject *go) {
	return go->rect->y + go->rect->h == windowHeight-1;
}
