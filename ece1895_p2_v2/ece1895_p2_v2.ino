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
uint8_t pin3 = 6; // karate chop
uint8_t pin4 = 7; // jellyfish

uint8_t pin5 = 10; // pwm for speaker

// for hex10
uint8_t pin6 = A4; 
uint8_t pin7 = A3;
uint8_t pin8 = A2;
uint8_t pin9 = A1;

// for hex01
uint8_t pin10 = A0; 
uint8_t pin11 = 13; 
uint8_t pin12 = 11; 
uint8_t pin13 = 9; 

// for debugging
uint8_t pin14 = 2; // led0 for command // **OPTIONAL
uint8_t pin15 = 4; // led1 for command // **OPTIONAL
uint8_t pin16 = 8; // correct command indicator // **OPTIONAL
uint8_t pin17 = 12; // end game indicator // **OPTIONAL
  
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

  // for debugging
  pinMode(pin14, OUTPUT); // command // **OPTIONAL
  pinMode(pin15, OUTPUT); // command // **OPTIONAL
  pinMode(pin16, OUTPUT); // correct // **OPTIONAL
  pinMode(pin17, OUTPUT); // wrong/end // **OPTIONAL
}

void loop() {
  // check if the user started the game
  if(digitalRead(pin1) == HIGH) {
    // indicate new game has started, reset points and time
    reset();
    while(isGame) {
      int aCommand = randomCommand(); // have Squidward play a random command
      bool isCommand = true; // boolean value for continuing to check move
      correctCommand = false; // boolean value for checking if correct move is made
      digitalWrite(pin16, LOW); // correct command indicator is reset
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
        int elapsedTime = millis() - t0;
        if (elapsedTime > timeLimit ) {
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

  timeLimit = 10 * 1000; // 10 seconds time limit

  prevState = analogRead(pin2); // patty flip state reset

  digitalWrite(pin17, LOW); // **OPTIONAL
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
  switch(randomNumber) {
    case 0:
      // TODO make Squidward say "Spongebob, flip the patty!" - convert to pwm output for speaker
      digitalWrite(pin14, HIGH); // command indicator // **OPTIONAL
      delay(500);
      digitalWrite(pin14, LOW); // reset command indicator // **OPTIONAL
      speakerCommand1(pin5);
      break;
    case 1:
      // TODO make Squidward say "Sandy, karate chop the table!" - convert to pwm output for speaker
      digitalWrite(pin15, HIGH); // command indicator // **OPTIONAL
      delay(500);
      digitalWrite(pin15, LOW); // reset command indicator // **OPTIONAL
      speakerCommand2(pin5);
      break;
    case 2:
      // TODO make Squidward say "Patrick, catch the jellyfish!" - convert to pwm output for speaker
      digitalWrite(pin14, HIGH); // command indicator // **OPTIONAL
      digitalWrite(pin15, HIGH); // command indicator // **OPTIONAL
      delay(500);
      digitalWrite(pin14, LOW); // reset command indicator // **OPTIONAL
      digitalWrite(pin15, LOW); // reset command indicator // **OPTIONAL
      speakerCommand3(pin5);
      break;
  }

  // reset output for speaker
  // delay(500);
  // noTone(pin5);

}

boolean checkPattyFlip() {
  int currState = analogRead(pin2);
  bool checkPrevSide = (prevState < 512);
  bool checkCurrSide = (currState < 512);
  if (checkCurrSide != checkPrevSide) {
    prevState = currState;
    return true;
  }
  return false;
}

// isDone checks if the correct command is played
bool isDone(int aCommand) {
  // read input pins
  bool checkSpongeBob = checkPattyFlip();
  bool checkSandy = (digitalRead(pin3) == HIGH);
  bool checkPatrick = (digitalRead(pin4) == HIGH);

  // introduce debouncing
  delay(100);

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

// end game stuff
void endGame(int aNum) {
  digitalWrite(pin17, HIGH); // indicate wrong move
  if(aNum == 0) {
    // TODO make Squidward say "Time's up!" - convert to pwm output for speaker
    noTone(pin5);
    speakerEnding1(pin5);
    delay(500);
    noTone(pin5);
    badEnding();
  }
  else if(aNum == 1) {
    // TODO make Squidward say "Wrong move!" - convert to pwm output for speaker
    noTone(pin5);
    speakerEnding2(pin5);
    delay(500);
    noTone(pin5);
    badEnding();
  }
  else if(aNum == 2) {
    // TODO make Squidward say "You win!" - convert to pwm output for speaker
    speakerEnding3(pin5);
    delay(500);
    noTone(pin5);
  }
}

void badEnding() {
  // TODO make Patrick yell in frustration/"Boooo!" - convert to pwm output for speaker
  speakerPatrick(pin5);
  delay(500);
  noTone(pin5);
}

// correct stuff
void isCorrect() {
  // TODO make Squidward play clarinet - convert to pwm output for speaker
  correctCommand = true;
  noTone(pin5);
  digitalWrite(pin16, HIGH); // correct command indicator // **OPTIONAL
  delay(500);
  digitalWrite(pin16, LOW);
  
  addPoints(); // increment points

  updateHexDisplay(); // update hex displays

  isPoints(); // if game is still going, use the amount of points to determine new time limit
  speakerClarinet(pin5);
  delay(500 );
  noTone(pin5);
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
}

// speaker outputs
void speakerCommand1(uint8_t speakerPin) {
  // "Spongebob"
  tone(speakerPin, 330, 200); // E4 (S)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (p)
  delay(250);
  tone(speakerPin, 392, 200); // G4 (o)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (n)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (g)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (e)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (b)
  delay(250);

  // "flip!"
  tone(speakerPin, 392, 200); // G4 (f)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (l)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (i)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (p)
  delay(250);

  noTone(speakerPin);
}

void speakerCommand2(uint8_t speakerPin) {
  // "Sandy"
  tone(speakerPin, 330, 200); // E4 (S)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (a)
  delay(250);
  tone(speakerPin, 392, 200); // G4 (n)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (d)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (y)
  delay(250);
  
  // "chop"
  tone(speakerPin, 294, 200); // D4 (c)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (h)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (o)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (p)
  delay(250);

  noTone(speakerPin);
}

void speakerCommand3(uint8_t speakerPin) {
  // "Patrick"
  tone(speakerPin, 330, 200); // E4 (P)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (a)
  delay(250);
  tone(speakerPin, 392, 200); // G4 (t)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (r)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (i)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (c)
  delay(250);

  // "catch"
  tone(speakerPin, 294, 200); // D4 (c)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (a)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (t)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (ch)
  delay(250);

  noTone(speakerPin);
}

void speakerEnding1(uint8_t speakerPin) {
  // "Time's"
  tone(speakerPin, 330, 200); // E4 (Ti)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (me)
  delay(250);
  tone(speakerPin, 392, 200); // G4 (s)
  delay(250);
  
  // "up!"
  tone(speakerPin, 440, 200); // A4 (u)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (p)
  delay(250);

  noTone(speakerPin);
}

void speakerEnding2(uint8_t speakerPin) {
  // "Wrong"
  tone(speakerPin, 330, 200); // E4 (Wr)
  delay(250);
  tone(speakerPin, 349, 200); // F4 (o)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (n)
  delay(250);
  
  // "move!"
  tone(speakerPin, 392, 200); // G4 (m)
  delay(250);
  tone(speakerPin, 440, 200); // A4 (o)
  delay(250);

  noTone(speakerPin);
}

void speakerEnding3(uint8_t speakerPin) {
  // "You"
  tone(speakerPin, 330, 200); // E4 (Y)
  delay(250);
  tone(speakerPin, 392, 200); // G4 (o)
  delay(250);

  // "win!"
  tone(speakerPin, 349, 200); // F4 (w)
  delay(250);
  tone(speakerPin, 294, 200); // D4 (i)
  delay(250);
  tone(speakerPin, 330, 200); // E4 (n)
  delay(250);

  noTone(speakerPin);
}

void speakerPatrick(uint8_t speakerPin) {
  // "Boooooo!"
  tone(speakerPin, 262, 300); // C4 (B)
  delay(150);
  tone(speakerPin, 330, 300); // E4 (o)
  delay(150);
  tone(speakerPin, 392, 300); // G4 (o)
  delay(150);
  tone(speakerPin, 330, 300); // E4 (o)
  delay(150);
  tone(speakerPin, 262, 300); // C4 (o)
  delay(150);

  noTone(speakerPin);
}

void speakerClarinet(uint8_t speakerPin) {
  // Simulating Squidward playing the clarinet for 2 seconds
  tone(speakerPin, 440, 200); // A4
  delay(250);
  tone(speakerPin, 349, 200); // F4
  delay(250);
  tone(speakerPin, 392, 200); // G4
  delay(250);
  tone(speakerPin, 466, 200); // A#4/Bb4
  delay(250);
  tone(speakerPin, 330, 200); // E4
  delay(250);
  tone(speakerPin, 392, 200); // G4
  delay(250);

  noTone(speakerPin);
}
