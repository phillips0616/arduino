#include <Servo.h>

Servo armA;
const int limitSwitch = 13;

bool ArmAHome = false;

void setup() {
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
  int curDegree = armA.read();
  while(digitalRead(limitSwitch) == HIGH) {
      Serial.println(armA.read());
      Serial.println(curDegree--);
      Serial.println("----------");
      curDegree--;
      //armA.write(curDegree);
      delay(150);
  }
  //once we trigger the limit switch we are considering that degree so we need to move back 90
//  for(int i = 0; i < 90; i++) {
//      curDegree++;
//      armA.write(curDegree);
//      delay(15);
//  }
  ArmAHome = true;
}

void loop() {

  if(!ArmAHome) {
    //moveToPos(armA, initialPos, 90, 15);
    findHomeArmA();
    Serial.println(armA.read());
  } else {
     Serial.println(armA.read());
  }
  delay(100);
}