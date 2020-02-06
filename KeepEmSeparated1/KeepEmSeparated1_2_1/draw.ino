
#define HAPPY_FLASH_DURATION    500
#define EDGE_FADE_DURAION       500
#define SPARKLE_OFFSET          80
#define SPARKLE_DURATION        800
#define SPARKLE_CYCLE_DURATION  1600
Timer happyFlashTimer;
bool happyFlashOn;

byte sparkleOffset[6] = {0, 3, 5, 1, 4, 2};



void displayPrepping() {
  // Last 1 second
  unsigned long timeUntilNextState = timeWhenNextState - millis();
  if (timeUntilNextState < 1000) {
    setColor(dim(YELLOW, 140+random(115)));
  }
  else {
    setColor(YELLOW);
  }
}

void displayPlaying() {
  Color displayColor = dim(myTeamColor, isHappy ? 255 : 120);
  // Friendlies are solid.
  if (wantsNeighbor) {
    setColor(displayColor);
  }
  // Soloists are "spoked."
  else {
    FOREACH_FACE(f) {
      if (f%2==0) setColorOnFace(displayColor, f);
      else setColorOnFace(OFF, f);
    }
  }
}

void displayResults() {
  if (isHappy)
    displayResultsHappy();
//        setColor(GREEN);
  else
    displayResultsNotHappy();
//        setColor(RED);
}


void displayResultsHappy() {
//  FOREACH_FACE(f) {
//    Color faceColor = makeColorHSB(64, saturation, 255);
//    setColorOnFace(faceColor, f);
//  }
  setColor(GREEN);
}

void displayResultsNotHappy() {
//  // have the color on the Blink raise and lower to feel more alive
//  byte bri = 185 + sin8_C( (millis() / 14) % 255) * 70 / 255; // oscillate between values 185and 255
//  setColor(dim(RED, bri));
//  if (millis()
  setColor(RED);
}
