/** Team Upsilon
 * Aidan Becher
 * Anjali Angal
 * Beryl Sin
 */

// variables to set state of game
bool correctCommand = false; // check if command was done correctly
bool isGame = false; // check if game runs

// variable for storing the number of points
int points = 0; // current points
const int totalPoints = 99; // points to win
int hex10 = 0;
int hex01 = 0;

// variable for keeping track of the time (in seconds)
int t0 = 0; // accumulated time
int timeLimit = 0; // time to do command

// variable for keeping track of patty flip
int prevState = 0;

// variables to hold the pins to be used for inputs/outputs
uint8_t pin1 = 5; // start button

uint8_t pin2 = A5; // patty
uint8_t pin3 = 2; // karate chop
uint8_t pin4 = 7; // jellyfish

uint8_t pin5 = 10; // pwm for speaker

// for hex10
uint8_t pin6 = A4; 
uint8_t pin7 = A3;
uint8_t pin8 = A2;
uint8_t pin9 = A1;

// for hex01
uint8_t pin10 = A0; 
uint8_t pin11 = 4; 
uint8_t pin12 = 8; 
uint8_t pin13 = 12; 
  
void setup() {
  // reading pin values
  pinMode(pin1, INPUT); // for checking if the start button is pressed

  pinMode(pin2, INPUT); // for patty flipping
  pinMode(pin3, INPUT); // for karate chopping
  pinMode(pin4, INPUT); // for jellyfish catching

  pinMode(pin5, OUTPUT); // for speaker

  pinMode(pin6, OUTPUT); // hex10 - 3
  pinMode(pin7, OUTPUT); // hex10 - 2
  pinMode(pin8, OUTPUT); // hex10 - 1
  pinMode(pin9, OUTPUT); // hex10 - 0

  pinMode(pin10, OUTPUT); // hex01 - 3
  pinMode(pin11, OUTPUT); // hex01 - 2
  pinMode(pin12, OUTPUT); // hex01 - 1
  pinMode(pin13, OUTPUT); // hex01 - 0
}

int elapsedTime = 0;

void loop() {
  // check if the user started the game
  // sometimes karate chop button triggers game start; make sure start trigger is held for a duration before triggering start
  bool start = false;
  if(digitalRead(pin1) == HIGH) {
    start = true;
    for (int i = 0; i < 50; i++) {
      if (digitalRead(pin1) == LOW) {
        start = false;
        break;
      }
    }
  }
  if(start) {
    // indicate new game has started, reset points and time
    reset();
    speakerStart(pin5);
    while(isGame) {
      int aCommand = randomCommand(); // have Squidward play a random command
      bool isCommand = true; // boolean value for continuing to check move
      correctCommand = false; // boolean value for checking if correct move is made
      bool isWin = (points >= totalPoints); // check points
      t0 = millis(); // update time tracker
      
      // check if command is played during time frame given
      // if done, grant a point and play clarinet sound
      // if not done, squidward says "time's up! you have xx points"
      // game ends, play patrick sound
      // set isGame to false if game ends
      while(!correctCommand && isCommand && !isWin) {
        isCommand = isDone(aCommand); // check move and see if its correct
      
        // check to see if the time is within time limit
     
        //elapsedTime = millis() - t0;
        if (elapsedTime > timeLimit) {
          // end game stuff, time's up version
          endGame(0);
          isCommand = false;
        }
        
        isWin = (points >= totalPoints);  // update points
      }
      isGame = isCommand && !isWin; 
    }
  }
}

void reset() {
  isGame = true;

  points = 0; // points accumulated
  hex10 = 0;
  hex01 = 0;
  updateHexDisplay();

  timeLimit = 5000; // 5 seconds time limit

  prevState = analogRead(pin2); // patty flip state reset

}

// function that makes Squidward play a random command
int randomCommand() {
  // returns a random value between 0 and 2
  // 0 = flip patty
  // 1 = karate chop
  // 2 = catch jellyfish
  int randomNumber = random(0, 3); 

  // uncomment a line to test specific command
  // randomNumber = 0; // for patty (switch)
  // randomNumber = 1 // for karate chop (button 1)
  // randomNumber = 2 // for jellyfish (button 2)
  playCommand(randomNumber);
  return randomNumber;
}

void playCommand(int randomNumber) {
  prevState = analogRead(pin2);
  switch(randomNumber) {
    case 0:
      delay(250);
      speakerCommand1(pin5);
      break;
    case 1:
      delay(250);
      speakerCommand2(pin5);
      break;
    case 2:
      delay(250);
      speakerCommand3(pin5);
      break;
  }
}

boolean checkPattyFlip() { // 800 // 200
  char side;
  if ((100 < prevState) && (prevState < 450)) {
    side = 't';
  }
  else {
    side = 'b';
  }

  if (side == 't') {
    if ((450 < analogRead(pin2)) && (analogRead(pin2) < 900)) {
      for (int i = 0; i < 100; i++) {
          if (!((450 < analogRead(pin2)) && (analogRead(pin2) < 900))) {
            return false;
          }
        }
        return true;
    }
    else {
      return false;
    }
  }
  else {
    if ((100 < analogRead(pin2)) && (analogRead(pin2) < 450)) {
        for (int i = 0; i < 1; i++) {
          if (!((100 < analogRead(pin2)) && (analogRead(pin2) < 450))) {
            return false;
          }
        }
        return true;
    }
    else {
      return false;
    }
  }
}

// isDone checks if the correct command is played
bool isDone(int aCommand) {
  // read input pins
  bool checkSpongeBob = checkPattyFlip();
  bool checkSandy = checkKarateChopChop();
  bool checkPatrick = (digitalRead(pin4) == HIGH);
  elapsedTime = millis() - t0;
  // check to see if the command is done
  switch(aCommand) {
    case 0:
      if(checkSandy || checkPatrick) {
        // end game stuff, wrong move version
        endGame(1);
        return false;
      }
      else if(checkSpongeBob) {
        // correct stuff
        isCorrect();
      }
      break;
    case 1:
      if(checkSpongeBob || checkPatrick) {
        // end game stuff, wrong move version
        endGame(1);
        return false;
      }
      else if(checkSandy) {
        // correct stuff
        isCorrect();
      }
      break;
    case 2:
      if(checkSpongeBob || checkSandy) {
        // end game stuff, wrong move version
        endGame(1);
        return false;
      }
      else if(checkPatrick) {
        // correct stuff
        isCorrect();
      }
      break;
  }
  return true;
}

bool checkKarateChopChop() {
  if (digitalRead(pin3) == HIGH) {
    for (int i = 0; i < 10; i++) {
      if (digitalRead(pin3) == LOW) {
        return false;
      }
    }
    return true;
  }
  return false;
}

// end game stuff
void endGame(int aNum) {
  if(aNum == 0) {
    // TODO make Squidward say "Time's up!" - convert to pwm output for speaker
    noTone(pin5);
    speakerEnding1(pin5);
    delay(500);
    noTone(pin5);
  }
  else if(aNum == 1) {
    // TODO make Squidward say "Wrong move!" - convert to pwm output for speaker
    noTone(pin5);
    speakerEnding1(pin5);
    delay(500);
    noTone(pin5);
  }
  else if(aNum == 2) {
    // TODO make Squidward say "You win!" - convert to pwm output for speaker
    speakerEnding2(pin5);
    delay(500);
    noTone(pin5);
  }
}

// correct stuff
void isCorrect() {
  // TODO make Squidward play clarinet - convert to pwm output for speaker
  correctCommand = true;
  noTone(pin5);
  delay(500);
  
  addPoints(); // increment points

  updateHexDisplay(); // update hex displays

  isPoints(); // if game is still going, use the amount of points to determine new time limit
  if (points != totalPoints) {
    speakerClarinet(pin5);
  }
}

// increment points
void addPoints() {
  if (hex01 == 9) {
    hex10 += 1;
    hex01 = 0;
  }
  else {
    hex01 += 1;
  }
  points += 1;
}

void updateHexDisplay() {
  digitalWrite(pin6, toBCD4(hex10));
  digitalWrite(pin7, toBCD3(hex10));
  digitalWrite(pin8, toBCD2(hex10));
  digitalWrite(pin9, toBCD1(hex10));

  digitalWrite(pin10, toBCD4(hex01));
  digitalWrite(pin11, toBCD3(hex01));
  digitalWrite(pin12, toBCD2(hex01));
  digitalWrite(pin13, toBCD1(hex01));
}

uint8_t toBCD1(int num) {
  switch(num) {
    case 0: // 0000
      return LOW;
    case 1: // 0001
      return HIGH;
    case 2: // 0010
      return LOW;
    case 3: // 0011
      return HIGH;
    case 4: // 0100
      return LOW;
    case 5: // 0101
      return HIGH;
    case 6: // 0110
      return LOW;
    case 7: // 0111
      return HIGH;
    case 8: // 1000
      return LOW;
    case 9: // 1001
      return HIGH;
  }
}

uint8_t toBCD2(int num) {
  switch(num) {
    case 0: // 0000
      return LOW;
    case 1: // 0001
      return LOW;
    case 2: // 0010
      return HIGH;
    case 3: // 0011
      return HIGH;
    case 4: // 0100
      return LOW;
    case 5: // 0101
      return LOW;
    case 6: // 0110
      return HIGH;
    case 7: // 0111
      return HIGH;
    case 8: // 1000
      return LOW;
    case 9: // 1001
      return LOW;
  }
}

uint8_t toBCD3(int num) {
  switch(num) {
    case 0: // 0000
      return LOW;
    case 1: // 0001
      return LOW;
    case 2: // 0010
      return LOW;
    case 3: // 0011
      return LOW;
    case 4: // 0100
      return HIGH;
    case 5: // 0101
      return HIGH;
    case 6: // 0110
      return HIGH;
    case 7: // 0111
      return HIGH;
    case 8: // 1000
      return LOW;
    case 9: // 1001
      return LOW;
  }
}

uint8_t toBCD4(int num) {
  switch(num) {
    case 0: // 0000
      return LOW;
    case 1: // 0001
      return LOW;
    case 2: // 0010
      return LOW;
    case 3: // 0011
      return LOW;
    case 4: // 0100
      return LOW;
    case 5: // 0101
      return LOW;
    case 6: // 0110
      return LOW;
    case 7: // 0111
      return LOW;
    case 8: // 1000
      return HIGH;
    case 9: // 1001
      return HIGH;
  }
}

// checks the amount of points and uses that to determine the time limit
void isPoints() {
  if (points == totalPoints) {
    endGame(2);
  }
  else {
    timeLimit = 5000 - (40 * points);
  }
  /**
  switch(points){
    case totalPoints:
      endGame(2);
      break;
    case 19:
      timeLimit -=1 * 1000;
      break;
    case 39:
      timeLimit -=1 * 1000;
      break;
    case 59:
      timeLimit -=1 * 1000;
      break;
    case 79:
      timeLimit -=1 * 1000;
      break;
    case 89:
      timeLimit -=1 * 1000;
      break;
    default:
      return;
  }
  */
}

// speaker outputs
void speakerStart(uint8_t speakerPin) {
  // Start
  tone(speakerPin, 783.9909, 100); // G5
  delay(167);
  tone(speakerPin, 880.0000, 100); // A5
  delay(167);
  tone(speakerPin, 987.7666, 100); // B5
  delay(167);
  tone(speakerPin, 880.0000, 100); // A5
  delay(333);
  tone(speakerPin, 987.7666, 100); // B5
  delay(167);
  tone(speakerPin, 783.9909, 100); // G5
  delay(333);
  tone(speakerPin, 587.3295, 167); // D5
  delay(167);
  tone(speakerPin, 783.9909, 100); // G5
  delay(167);
  noTone(speakerPin);
}

void speakerCommand1(uint8_t speakerPin) {
  // Spongebob
  tone(speakerPin, 932.3275, 111); // Bf5
  delay(111);
  tone(speakerPin, 880.0000, 111); // A5
  delay(111);
  tone(speakerPin, 783.9909, 111); // G5
  delay(111);
  tone(speakerPin, 698.4565, 111); // F5
  delay(111);
  tone(speakerPin, 659.2551, 111); // E5
  delay(111);
  tone(speakerPin, 698.4565, 111); // F5
  delay(111);
  tone(speakerPin, 783.9909, 111); // G5
  delay(111);
  tone(speakerPin, 659.2551, 111); // E5
  delay(111);
  tone(speakerPin, 698.4565, 145); // F5
  delay(222);
  tone(speakerPin, 698.4565, 145); // F5
  delay(222);
  tone(speakerPin, 698.4565, 145); // F5
  delay(222);
  noTone(speakerPin);
}

void speakerCommand2(uint8_t speakerPin) {
  // Sandy
  tone(speakerPin, 440, 100);
  delay(100);
  tone(speakerPin, 587.3295, 100);
  delay(100);
  noTone(speakerPin);
}

void speakerCommand3(uint8_t speakerPin) {
  // Patrick
  tone(speakerPin, 103.8262, 1000);
  delay(800);
  noTone(speakerPin);
}

void speakerEnding1(uint8_t speakerPin) {
  // Game over
  tone(speakerPin, 293.6648, 167); // D4
  delay(333);
  tone(speakerPin, 277.1826, 167); // Cs4
  delay(333);
  tone(speakerPin, 261.6256, 167); // C4
  delay(333);
  tone(speakerPin, 246.9417, 1000); // B
  delay(1000);
  noTone(speakerPin);
}

void speakerEnding2(uint8_t speakerPin) {
  // Win
  tone(speakerPin, 369.9944, 167); // Fs4
  delay(167);
  tone(speakerPin, 440.0000, 167); // A4
  delay(167);
  tone(speakerPin, 587.3295, 100); // D5
  delay(167);
  tone(speakerPin, 391.9954, 167); // G4
  delay(167);
  tone(speakerPin, 493.8833, 167); // B4
  delay(167);
  tone(speakerPin, 587.3295, 100); // D5
  delay(167);
  tone(speakerPin, 369.9944, 167); // Fs4
  delay(167);
  tone(speakerPin, 440.0000, 167); // A4
  delay(167);
  tone(speakerPin, 587.3295, 100); // D5
  delay(167);
  tone(speakerPin, 739.9888, 167); // Fs5
  delay(167);
  tone(speakerPin, 659.2551, 167); // E5
  delay(167);
  tone(speakerPin, 587.3295, 100); // D5
  delay(167);
  tone(speakerPin, 554.3653, 167); // Cs5
  delay(167);
  tone(speakerPin, 493.8833, 167); // B4
  delay(167);
  tone(speakerPin, 554.3653, 100); // Cs5
  delay(167);
  tone(speakerPin, 440.0000, 167); // A4
  delay(167);
  tone(speakerPin, 493.8833, 167); // B4
  delay(167);
  tone(speakerPin, 554.3653, 100); // Cs5
  delay(167);
  tone(speakerPin, 587.3295, 167); // D5
  delay(167);
  tone(speakerPin, 739.9888, 100); // Fs5
  delay(167);
  tone(speakerPin, 659.2551, 100); // E5
  delay(167);
  tone(speakerPin, 587.3295, 500); // D5
  delay(500);
  noTone(speakerPin);
}

void speakerClarinet(uint8_t speakerPin) {
  // Correct
  tone(speakerPin, 587.3295, 100); // D5
  delay(100);
  tone(speakerPin, 739.9888, 100); // Fs5
  delay(100);
  tone(speakerPin, 880.0000, 100); // A5
  delay(100);
  noTone(speakerPin);
}
