/** Team Upsilon
 * Aidan Becher
 * Anjali Angal
 * Beyl Sin
 */

// variable to check if game is active
bool isGame = false;
// variable for storing the number of points
bool correctCommand = false;
int points = 0;
// variable for keeping track of the time (in seconds)
int t0 = 0;
int timeLimit = 0;
// variable for keeping track of patty flip
int prevState = 0;

// variables to hold the pins to be used for inputs/outputs
// arbitrary pin values are used
int pin1 = 5; // start button
int pin2 = A5; // patty
int pin3 = 6; // karate chop
int pin4 = 7; // jellyfish
int pin5 = 10; // pwm for speaker
int pin6 = 9; // pwm for hex display

// for debugging
int pin7 = 2; // led0 for command
int pin8 = 4; // led1 for command
int pin9 = 8; // correct command indicator
int pin10 = 12; // end game indicator
  
void setup() {
  // reading pin values
  pinMode(pin1, INPUT); // for checking if the start button is pressed
  pinMode(pin2, INPUT); // for patty flipping
  pinMode(pin3, INPUT); // for karate chopping
  pinMode(pin4, INPUT); // for jellyfish catching

  pinMode(pin7, OUTPUT); // command
  pinMode(pin8, OUTPUT); // command
  pinMode(pin9, OUTPUT); // correct
  pinMode(pin10, OUTPUT); // wrong/end

  pinMode(pin5, OUTPUT); // analog output for speaker
  pinMode(pin6, OUTPUT); // for updating the hex display for points
}

void loop() {
  // check if the user started the game
  if(digitalRead(pin1) == HIGH) {
    // indicate new game has started, reset points and time
    isGame = true;
    points = 0; // points accumulated
    timeLimit = 10 * 1000; // 10 seconds time limit
    prevState = analogRead(pin2);
    digitalWrite(pin10, LOW);
    while(isGame) {
      // have Squidward play a random command
      int aCommand = randomCommand(); // TODO
      bool isCommand = true; // for checking input
      correctCommand = false; // for checking if correct move is made
      digitalWrite(pin9, LOW); // correct command indicator is reset

      // check if command is played during time frame given
      // if done, grant a point and play clarinet sound
      // if not done, squidward says "time's up! you have xx points"
      // game ends, play patrick sound
      // set isGame to false if game ends
      t0 = millis(); // update time tracker
      while(!correctCommand && isCommand) {
        isCommand = isDone(aCommand); // TODO
        // check to see if the time is within time limit
        int elapsedTime = millis() - t0;
        if (elapsedTime > timeLimit ) {
          // end game stuff, time's up version
          endGame(0);
          isCommand = false;
        }
      }
      isGame = isCommand;
    }
  }
}

// function that makes Squidward play a random command
int randomCommand() {
  // returns a random value between 0 and 2
  // 0 = flip patty
  // 1 = karate chop
  // 2 = catch jellyfish
  int randomNumber = random(0, 3); 
  switch(randomNumber) {
    case 0:
      // TODO make Squidward say "Spongebob, flip the patty!" - convert to analog output for speaker
      analogWrite(pin5, 1);
      digitalWrite(pin7, HIGH);
      break;
    case 1:
      // TODO make Squidward say "Sandy, karate chop the table!" - convert to analog output for speaker
      analogWrite(pin5, 2);
      digitalWrite(pin8, HIGH);
      break;
    case 2:
      // TODO make Squidward say "Patrick, catch the jellyfish!" - convert to analog output for speaker
      analogWrite(pin5, 3);
      digitalWrite(pin7, HIGH);
      digitalWrite(pin8, HIGH);
      break;
  }

  // reset output for speaker
  delay(500);
  analogWrite(pin5, 0);
  digitalWrite(pin7, LOW);
  digitalWrite(pin8, LOW);
  return randomNumber;
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
  digitalWrite(pin10, HIGH); // indicate wrong move
  if(aNum == 0) {
    // TODO make Squidward say "Time's up!" - convert to analog output for speaker
    analogWrite(pin5, 4);
    delay(1000);
    analogWrite(pin5, 0);
    badEnding();
  }
  else if(aNum == 1) {
    // TODO make Squidward say "Wrong move!" - convert to analog output for speaker
    analogWrite(pin5, 5);
    delay(1000);
    analogWrite(pin5, 0);
    badEnding();
  }
  else if(aNum == 2) {
    // TODO make Squidward say "You win!" - convert to analog output for speaker
    analogWrite(pin5, 6);
    delay(1000);
    analogWrite(pin5, 0);
    // TODO make Squidward say "You got " + points + "points." - convert to analog output for speaker
    analogWrite(pin5, 7);
    delay(1000);
    analogWrite(pin5, 0);
  }
}

void badEnding() {
  // TODO make Squidward say "You got " + points + "points." - convert to analog output for speaker
  analogWrite(pin5, 7);
  delay(1000);
  analogWrite(pin5, 0);
  // TODO make Patrick yell in frustration/"Boooo!" - convert to analog output for speaker
  analogWrite(pin5, 8);
  delay(1000);
  analogWrite(pin5, 0);
}

// correct stuff
void isCorrect() {
  // TODO make Squidward play clarinet - convert to analog output for speaker
  correctCommand = true;
  digitalWrite(pin9, HIGH);
  analogWrite(pin5, 9);
  delay(1000);
  analogWrite(pin5, 0);

  points += 1; // increment points
  isPoints(); // if game is still going, use the amount of points to determine new time limit
  // TODO update hex display
  digitalWrite(pin6, HIGH);
  delay(1000);
  digitalWrite(pin6, LOW);
}

// checks the amount of points and uses that to determine the time limit
void isPoints() {
  switch(points){
    case 99:
      endGame(2);
      break;
    case 19:
      timeLimit -=1;
      break;
    case 39:
      timeLimit -=1;
      break;
    case 59:
      timeLimit -=1;
      break;
    case 79:
      timeLimit -=1;
      break;
    case 89:
      timeLimit -=1;
      break;
  }
}

