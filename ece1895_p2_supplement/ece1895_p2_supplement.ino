int pin1 = 4;

int pin2 = 5;
int pin3 = 6;
int pin4 = 7;
int pin5 = 8;

int pin6 = 12;
int pin7 = 11;
int pin8 = 10;
int pin9 = 9;

int hex10 = 0;
int hex1 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, INPUT);

  pinMode(pin2, OUTPUT); 
  pinMode(pin3, OUTPUT); 
  pinMode(pin4, OUTPUT); 
  pinMode(pin5, OUTPUT);

  pinMode(pin6, OUTPUT); 
  pinMode(pin7, OUTPUT); 
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(digitalRead(pin1) == HIGH) {
    addPoints();
    digitalWrite(pin2, toBCD4(hex10));
    digitalWrite(pin3, toBCD4(hex10));
    digitalWrite(pin4, toBCD4(hex10));
    digitalWrite(pin5, toBCD4(hex10));

    digitalWrite(pin6, toBCD4(hex1));
    digitalWrite(pin7, toBCD4(hex1));
    digitalWrite(pin8, toBCD4(hex1));
    digitalWrite(pin9, toBCD4(hex1));
  }
}

void addPoints() {
  if (hex1 == 9) {
    hex10 += 1;
    hex1 = 0;
  }
  else {
    hex1 += 1;
  }
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

