#include <Servo.h>

Servo armA;
Servo armB;
const int limitSwitch = 13;
int curDegree = 90;
int curDegreeB = 90;

bool executingCommand = false; //boolean value to allow commands to finish before reading the next command from serial

void setup() {
  armB.attach(8);//
  armA.attach(7);
  pinMode(limitSwitch,INPUT);
  Serial.begin(9600);
}

void moveToPos(Servo s, int curDegree, int targetDegree, int delayTime) {
  if(curDegree > targetDegree) {
    while(curDegree > targetDegree) {
      curDegree--;
      s.write(curDegree);
      delay(delayTime);
    }
  } else {
    while(curDegree < targetDegree) {
      curDegree++;
      s.write(curDegree);
      delay(delayTime);
    }
  }
}

void findHomeArmA() {
  Serial.println("Entering find home method...");
  curDegree = armA.read();
  while(digitalRead(limitSwitch) == HIGH and curDegree > 0) {
      curDegree--;
      armA.write(curDegree);
      delay(15);
  }
  int backOff = 90 - curDegree;
//  once we trigger the limit switch we are considering that degree so we need to move back 90
  for(int i = 0; i < backOff; i++) {
      curDegree++;
      armA.write(curDegree);
      delay(15);
  }
  executingCommand = false;
}

void findHomeArmB() {
  Serial.println("Entering find home method...");
  curDegreeB = armB.read();
  while(digitalRead(limitSwitch) == HIGH and curDegreeB < 180) {
      curDegreeB++;
      armB.write(curDegree);
      delay(15);
  }
  int backOff = curDegreeB - 90;
//  once we trigger the limit switch we are considering that degree so we need to move back 90
  for(int i = curDegreeB; i > backOff; i--) {
      curDegreeB--;
      armB.write(curDegreeB);
      delay(15);
  }
  executingCommand = false;
}

void loop() {
  if(!executingCommand && Serial.available() > 0) {
    char incomingByte = Serial.read();
    Serial.println(incomingByte);
    executingCommand = true;
    
    switch(incomingByte) {
      case 'H':
        findHomeArmA();
        break;
      case 'M':
        armA.write(90);
        curDegree = armA.read();
        Serial.println("Moving to 90 from input 'M'");
        executingCommand = false;
        break;
      case 'R':
        Serial.println(armA.read());
        break;
      case 'B':
        findHomeArmB();
        break;
      default:
        executingCommand = false;
        break;
    }
  }
}s