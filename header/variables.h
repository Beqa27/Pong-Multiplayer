// ALL THE VARIABLES IMMA PUT HERE


// Screen stuff
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600



//ball relasted stuff
SDL_Rect ballRect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50, 50 }; 
int ballSpeed = 5;
const char* ballImagePath = "assets/ball.png";
bool gameRunning = true;
int screenWidth = 800;
int screenHeight = 600;

SDL_Rect paddleLeft = { 50, screenHeight/2 - 50, 10, 100 };
SDL_Rect paddleRight = { screenWidth - 60, screenHeight/2 - 50, 10, 100 };
