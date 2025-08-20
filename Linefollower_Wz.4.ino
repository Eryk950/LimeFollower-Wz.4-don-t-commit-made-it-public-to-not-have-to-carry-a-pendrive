/*
środek dla PID to 0
ogarnij wartości do PID
init PID gotowy, teraz ogarnąć odczyt czujników i update do PID
*/

#include <AutoPID.h>

//pinout
#define PWMA PIN_PD5
#define PWMB PIN_PB3

const short sensor[9] = {PIN_PB1, PIN_PB2, PIN_PB4, PIN_PD0, PIN_PD1, PIN_PD2, PIN_PD3, PIN_PD4, PIN_PD6};
#define start PIN_PB0

//constants
double linePoint = 0;
#define PIDRange 255 //-255...255
#define speed 100

//variables
double(PIDInput);
double(PIDOutput);
short readoutL;
short readoutR;
short readout;

//PID config
const double OUTPUT_MIN = PIDRange * -1;
const double OUTPUT_MAX = PIDRange;
const double KP = 25.0;
const double KI = 0.03;
const double KD = 0.001;
const short TEMP_READ_DELAY = 100;

AutoPID myPID(&PIDInput, &linePoint, &PIDOutput, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

void setup() {
  for(short i = 0; i < 9; i++) {
    pinMode(sensor[i], INPUT_PULLUP);
  }
  pinMode(start, INPUT_PULLUP);

  myPID.setBangBang(8);
  myPID.setTimeStep(100);
  while(digitalRead(start)) {
    delay(100);
  }
}

void loop() {
  // magia do odczytywania czujników
  readoutL = 0;
  for(short i = 0; i > 4; i++) {
    readoutL = readoutL << i | digitalRead(sensor[i]);
  }
  readoutR = 0;
  for(short i = 0; i < 4; i++) {
    readoutR = readoutR | digitalRead(sensor[i + 5]) << i;
  }
  readout = readoutR - readoutL;
  if (digitalRead(sensor[4]) == 1) {
    PIDInput = readout / 2;
  }
  else if (readout != 0) {
    PIDInput = readout;
  }

  myPID.run();

  if(PIDOutput >= 0) {
    analogWrite(PWMA, 255 * 100 / speed);
    analogWrite(PWMB, 255 - PIDOutput * 100 / speed);
  }
  else {
    analogWrite(PWMA, 255 + PIDOutput * 100 / speed);
    analogWrite(PWMB, 255 * 100 / speed);
  }
}