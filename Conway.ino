#include "LedControl.h"

// Globals
bool oldMap[8][8];
LedControl lc = LedControl(12,11,10,1);
int delayTime = 1000;

void setup() {
  Serial.begin(9600);         // for serial monitor
  randomSeed(analogRead(0));  // seed RNG

  // Set up display
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  // Seed map with random values
  for (int col = 0; col < 8; col++) {
    for (int row = 0; row < 8; row++) {
      oldMap[col][row] = random(2);
    }
  }
}

void display() {
  // Show values contained in oldMap[]
  for (int col = 0; col < 8; col++) {
    for (int row = 0; row < 8; row++) {
      lc.setLed(0, col, row, oldMap[col][row]);
    }
  }
}

void evolve() {
  bool newMap[8][8];
  
  for (int col = 0; col < 8; col++) {
    for (int row = 0; row < 8; row++) {
      int numNeighbors = countNeighbors(oldMap,col,row);

      // Update newMap according to Conway's rules
      if (oldMap[col][row] == true && numNeighbors < 2) {
        newMap[col][row] = false;
      } else if (oldMap[col][row] == true && (numNeighbors == 2 || numNeighbors == 3)) {
        newMap[col][row] = true;
      } else if (oldMap[col][row] == true && numNeighbors > 3) {
        newMap[col][row] = false;
      } else if (oldMap[col][row] == false && numNeighbors == 3) {
        newMap[col][row] = true;
      }

      // Done with oldMap, so overwrite it with newMap
      memcpy(oldMap, newMap, sizeof(newMap));
    }
  }
}

int countNeighbors(bool oldMap[][8], int col, int row) {
  int numNeighbors = 0;
  
  int prevCol = col - 1;
  int nextCol = col + 1;
  int prevRow = row - 1;
  int nextRow = row + 1; 

  // Check NW
  if (prevCol > 0 && prevRow > 0 && oldMap[prevCol][prevRow] == true) {numNeighbors++;}
  // Check N
  if (prevRow > 0 && oldMap[col][prevRow] == true) {numNeighbors++;}
  // Check NE
  if (nextCol < 8 && prevRow > 0 && oldMap[nextCol][prevRow] == true) {numNeighbors++;}
  // Check E
  if (nextCol < 8 && oldMap[nextCol][row] == true) {numNeighbors++;}
  // Check SE 
  if (nextCol < 8 && nextRow < 8 && oldMap[nextCol][nextRow] == true) {numNeighbors++;}
  // Check S
  if (nextRow < 8 && oldMap[col][nextRow] == true) {numNeighbors++;}
  // Check SW
  if (prevCol > 0 && nextRow < 8 && oldMap[prevCol][nextRow] == true) {numNeighbors++;}
  // Check W
  if (prevCol > 0 && oldMap[prevCol][row] == true) {numNeighbors++;}

  return numNeighbors;
}

void loop() {
  display();
  evolve();
  delay(delayTime);
}
