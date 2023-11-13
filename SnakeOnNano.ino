#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display width and height, change these to match your display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BLOCK_SIZE 5 // Initial Block Sizes
#define SPEED_LEVEL 8 // Initial Speed Level
#define MAX_SPEED_LEVEL 15 //Upper Bound of Speed Level
#define MIN_SPEED_LEVEL 1 //Lower Bound of Speed Level
#define BORDER_SIZE 1 // Border Width
#define SNAKE_LENGTH 4 //Initial Length of Snake
#define SNAKE_SIZE 30 //Inital Size of Array Allocated for the Snake (Dynamically Allocated)

// Set up the display using the appropriate pins
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Joystick pins
const int joyXPin = A0;
const int joyYPin = A1;
const int joySwitchPin = A2;

// Food positions, Joystick Variables, Snake Speed (Refresh Rate) Variable
int foodX, foodY, joyX, joyY, snakeSpeed;

//Arrays used to store the Snakes Size (Dynamically Allocated)
int* snakeX; 
int* snakeY;

// Direction of snake movement
int xDir = 1; //1 = Right, 0 = Left
int yDir = 0; //1 = Down, 0 = Up

// Direction (of Joystick) variables
bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = true; // Assume the snake starts by moving right

bool bordersOn = true; // Initially, borders are on
int speedLevel = SPEED_LEVEL; // Start in the middle of the range
int snakeLength = SNAKE_LENGTH; //Initialize the length of the Snake
int blockSize = BLOCK_SIZE; //Initialize the Block Sizes

// Game states
enum GameState {
  PLAYING,
  GAME_OVER,
  PAUSED
};
// Current game state
GameState gameState = PLAYING;

// Menu items
enum MenuItem {
  ADJUST_SPEED,
  ADJUST_SIZE,
  TOGGLE_BORDERS,
  RETURN_GAME
};
// Current menu selection
MenuItem currentSelection = ADJUST_SPEED;


void setup() {

  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  // Allocate initial memory for the snake arrays
  snakeX = (int*)malloc(SNAKE_SIZE * sizeof(int));
  snakeY = (int*)malloc(SNAKE_SIZE * sizeof(int));

  // Set up the joystick
  pinMode(joyXPin, INPUT);
  pinMode(joyYPin, INPUT);
  pinMode(joySwitchPin, INPUT_PULLUP);

  //Initialize the Speed Level
  snakeSpeed = getDelayFromSpeedLevel(speedLevel);

  // Show welcome screen
  showWelcomeScreen();

  //Reset & Setup the display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Initialize snake position to the center of the screen
  snakeX[0] = SCREEN_WIDTH / 2;
  snakeY[0] = SCREEN_HEIGHT / 2;

  // Initialize the rest of the snake body offscreen or hidden
  for (int i = 1; i < SNAKE_SIZE; i++) {
    snakeX[i] = -1; // Offscreen
    snakeY[i] = -1; // Offscreen
  }

  // Place the food at a random location
  randomSeed(analogRead(0));
  placeFood();
}

void loop() {
  // Check if the joystick button is pressed to switch between states
  if (digitalRead(joySwitchPin) == LOW) {
    delay(200); // Debounce delay
    while (digitalRead(joySwitchPin) == LOW); // Wait for button release

    if (gameState == PAUSED) {
      // Only allow exiting the pause menu if on the "Return to game" option
      if (currentSelection == RETURN_GAME) {
        gameState = PLAYING;
      }
    } else if (gameState == PLAYING) {
      gameState = PAUSED;
    } else if (gameState == GAME_OVER) {
      resetGame(); // Call resetGame to reinitialize the game if in GAME_OVER state
    }
  }

  // Read joystick for menu navigation and adjustment
  joyX = analogRead(joyXPin);
  joyY = analogRead(joyYPin);

  switch(gameState) {
    case PLAYING:

      // Determine the direction based on the joystick position
      if(joyX < 300 && !movingLeft && !movingRight) { //Move Right
        xDir = 1;
        yDir = 0;
        movingRight = true;
        movingLeft = movingUp = movingDown = false;
      } else if(joyX >= 950 && !movingRight && !movingLeft) { //Move Left
        xDir = -1;
        yDir = 0;
        movingLeft = true;
        movingRight = movingUp = movingDown = false;
      } else if(joyY < 300 && !movingUp && !movingDown) { //Move Up
        yDir = -1;
        xDir = 0;
        movingUp = true;
        movingRight = movingLeft = movingDown = false;
      } else if(joyY >= 950 && !movingDown && !movingUp) { //Move Down
        yDir = 1;
        xDir = 0;
        movingDown = true;
        movingRight = movingLeft = movingUp = false;
      }

      // Update snake position
      moveSnake();

      // Check for collisions
      checkCollisions();

      // Draw everything
      display.clearDisplay();
      drawBorder();
      drawFood();
      drawSnake();
      display.display();

      // Delay between updates controls the speed of the snake
      delay(snakeSpeed);
      break;

    case GAME_OVER:
      handleGameOver();
      break;

    case PAUSED:
      handlePause();
      break;

  }
}

void moveSnake() {
  // Shift the snake body to follow the head
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move the snake head
  snakeX[0] += xDir * blockSize; // Ensure that the head moves by one segment size
  snakeY[0] += yDir * blockSize;
}

void placeFood() {
  foodX = random(BORDER_SIZE, SCREEN_WIDTH - blockSize - BORDER_SIZE);
  foodY = random(BORDER_SIZE, SCREEN_HEIGHT - blockSize - BORDER_SIZE);
}

void checkCollisions() {
  // Check for collision with self
  for(int i = 1; i < snakeLength; i++) {
    if(snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      gameState = GAME_OVER;
    }
  }

  // Check if snake has eaten the food
  if(abs(snakeX[0] - foodX) < blockSize && abs(snakeY[0] - foodY) < blockSize) {
    // Try to increase the arrays' sizes
    int* newSnakeX = (int*)realloc(snakeX, (snakeLength + 1) * sizeof(int));
    int* newSnakeY = (int*)realloc(snakeY, (snakeLength + 1) * sizeof(int));

    if (newSnakeX != NULL && newSnakeY != NULL) {
      // Reallocation was successful
      snakeX = newSnakeX;
      snakeY = newSnakeY;
      snakeLength++; // Increase the length of the snake
    } else {
      // Reallocation failed - handle the error, e.g., by ending the game
      gameState = GAME_OVER;
      return;
    }
    
    //Increase the Games Speed
    if(speedLevel < MAX_SPEED_LEVEL){
      speedLevel++;
      snakeSpeed = getDelayFromSpeedLevel(speedLevel);
    }
    placeFood(); // Place a new food pellet
  }

  // Check for collision with the border or loop around if borders are off
  if (!bordersOn) {
    if (snakeX[0] < BORDER_SIZE) snakeX[0] = SCREEN_WIDTH - BORDER_SIZE - blockSize;
    else if (snakeX[0] >= SCREEN_WIDTH - BORDER_SIZE) snakeX[0] = BORDER_SIZE;

    if (snakeY[0] < BORDER_SIZE) snakeY[0] = SCREEN_HEIGHT - BORDER_SIZE - blockSize;
    else if (snakeY[0] >= SCREEN_HEIGHT - BORDER_SIZE) snakeY[0] = BORDER_SIZE;
  } else {
    // Existing collision with border code
    if(snakeX[0] < BORDER_SIZE || snakeX[0] >= SCREEN_WIDTH - BORDER_SIZE ||
       snakeY[0] < BORDER_SIZE || snakeY[0] >= SCREEN_HEIGHT - BORDER_SIZE) {
      gameState = GAME_OVER;
    }
  }
}

void drawFood() {
  display.fillRect(foodX + BORDER_SIZE, foodY + BORDER_SIZE, blockSize, blockSize, SSD1306_WHITE);
}

void drawSnake() {
  for(int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i] + BORDER_SIZE, snakeY[i] + BORDER_SIZE, blockSize, blockSize, SSD1306_WHITE);
  }
}

void drawBorder() {
  if (bordersOn) {
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  }
}

void handleGameOver() {
  display.clearDisplay();
  display.setTextSize(2); // Set text size. Adjust as needed for your display
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0); // Set the position. Adjust as needed for your display
  display.println("GAME OVER!");
  display.setTextSize(1); // Reset text size for smaller text
  display.setCursor(0, 25); // Adjust cursor position for the next text
  display.print("Final Length: ");
  display.print(snakeLength); // Display the final length of the snake
  display.println();
  display.println("Press joystick to    replay!");
  display.display();
}

void handlePause() {
  display.clearDisplay();
  display.setTextSize(2); // Set text size. Adjust as needed for your display
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 0); // Set the position. Adjust as needed for your display
  display.println("PAUSED");

  display.setTextSize(1);
  // Display the snake speed option
  display.print("Speed Level = ");
  if (currentSelection == ADJUST_SPEED) display.print("<-");
  display.print(speedLevel);
  if (currentSelection == ADJUST_SPEED) display.print("->");
  display.println();

  // Display the snake size option
  display.print("Snake Size  = ");
  if (currentSelection == ADJUST_SIZE) display.print("<-");
  display.print(blockSize);
  if (currentSelection == ADJUST_SIZE) display.print("->");
  display.println();

  // Display the borders toggle option
  display.print("Borders = ");
  if (currentSelection == TOGGLE_BORDERS) display.print("<-");
  display.print(bordersOn ? "On" : "Off");
  if (currentSelection == TOGGLE_BORDERS) display.print("->");
  display.println();

  // Display the return option
  if (currentSelection == RETURN_GAME) display.println("> Return to game");
  else display.println("  Return to game");

  display.display();

    if (joyY < 300) { // Joystick moved up
      currentSelection = (currentSelection == ADJUST_SPEED) ? RETURN_GAME : (MenuItem)(currentSelection - 1);
      delay(200); // Debounce delay
    } else if (joyY >= 950) { // Joystick moved down
      currentSelection = (currentSelection == RETURN_GAME) ? ADJUST_SPEED : (MenuItem)(currentSelection + 1);
      delay(200); // Debounce delay
    } else if (joyX < 300) { // Joystick moved left
      if (currentSelection == ADJUST_SPEED && speedLevel < MAX_SPEED_LEVEL) {
        speedLevel++;
        snakeSpeed = getDelayFromSpeedLevel(speedLevel); // Update the actual speed (delay)
      } else if (currentSelection == ADJUST_SIZE && blockSize < 15) {
        blockSize += 1;
      } else if (currentSelection == TOGGLE_BORDERS){
        bordersOn = !bordersOn; // Toggle the state
      }
      delay(200); // Debounce delay
    } else if (joyX >= 950) { // Joystick moved right
      if (currentSelection == ADJUST_SPEED && speedLevel > MIN_SPEED_LEVEL) {
        speedLevel--;
        snakeSpeed = getDelayFromSpeedLevel(speedLevel); // Update the actual speed (delay)
      } else if (currentSelection == ADJUST_SIZE && blockSize  >= 1) {
        blockSize  -= 1;
      }else if (currentSelection == TOGGLE_BORDERS){
        bordersOn = !bordersOn; // Toggle the state
      }
      delay(200); // Debounce delay
    }
}

void resetGame() {
  // Free the old arrays
  free(snakeX);
  free(snakeY);

  // Allocate memory again for the snake arrays with the initial size
  snakeX = (int*)malloc(SNAKE_SIZE * sizeof(int));
  snakeY = (int*)malloc(SNAKE_SIZE * sizeof(int));

  // Set the initial position of the snake's head to the center of the screen
  snakeX[0] = SCREEN_WIDTH / 2;
  snakeY[0] = SCREEN_HEIGHT / 2;

  // Initialize the rest of the snake body
  for (int i = 1; i < SNAKE_SIZE; i++) {
    snakeX[i] = -1;//snakeX[0]; // Align with the head's X
    snakeY[i] = -1;//snakeY[0]; // Align with the head's Y
  }

  // Reset the snake's direction to right
  xDir = 1;
  yDir = 0;
  movingUp = false;
  movingDown = false;
  movingLeft = false;
  movingRight = true;

  placeFood();     // Place new food
  snakeLength = SNAKE_LENGTH; // Reset the length of the snake
  gameState = PLAYING; // Set the game state to playing
  speedLevel = SPEED_LEVEL;
  // snakeSpeed = SNAKE_SPEED; // Reset the speed of the snake
  snakeSpeed = getDelayFromSpeedLevel(speedLevel);
  blockSize = BLOCK_SIZE; // Reset the block size
}

int getDelayFromSpeedLevel(int level) {
  // Map the speed level to a delay value
  // Level 1 is the slowest speed (250 delay)
  // Level 10 is the fastest speed (5 delay)
  return map(level, MIN_SPEED_LEVEL, MAX_SPEED_LEVEL, 225, 5);
}

void showWelcomeScreen() {

  display.clearDisplay();
  display.setTextSize(2); // Set text size. Adjust as needed for your display
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(33, 0); // Set the position. Adjust as needed for your display
  display.println("SNAKE");
  display.setTextSize(1); // Reset text size for smaller text
  display.setCursor(20, 25); // Adjust cursor position for the next text
  display.println("Press joystick");
  display.println("       to start");
  display.display();

  // Wait for the joystick button to be pressed
  while(digitalRead(joySwitchPin) == HIGH); // Loop until the button is pressed

  // Optional: wait for the joystick button to be released
  while(digitalRead(joySwitchPin) == LOW); // Loop until the button is released

  delay(250);
}
