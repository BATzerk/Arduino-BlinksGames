



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
//  Color displayColor = dim(myTeamColor, isHappy ? 255 : 120);
  setColor(myTeamColor);//displayColor);
}

void displayResults() {
  if (isHappy) {
    setColor(GREEN);
  }
  else {
    setColor(RED);
  }
}




void displayResetGo() {
  setColor(WHITE);
}
void displayResetResolve() {
  setColor(dim(WHITE, 120));
}


//  FOREACH_FACE(f) {
//    Color faceColor = makeColorHSB(64, saturation, 255);
//    setColorOnFace(faceColor, f);
//  }

//  // have the color on the Blink raise and lower to feel more alive
//  byte bri = 185 + sin8_C( (millis() / 14) % 255) * 70 / 255; // oscillate between values 185and 255
//  setColor(dim(RED, bri));
//  if (millis()
