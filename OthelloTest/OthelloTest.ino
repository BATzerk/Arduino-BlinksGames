


Color colors[] = {
  makeColorHSB(0,0,0),
  makeColorHSB(90, 255, 255),
  makeColorHSB(200, 255, 255)
};

int colorIndex = 0;
bool faceIsArrow[6];

int numTimesChanged;

void setup() {
  reset();
}

void reset() {
  colorIndex = 0;
  setNumTimesChanged(0);
  applyColor();
}
void setNumTimesChanged(int val) {
  numTimesChanged = val;
  setBoolArray(faceIsArrow, false);
  if (numTimesChanged >= 1) {
    faceIsArrow[0] = true;
  }
  if (numTimesChanged >= 2) {
    faceIsArrow[3] = true;
  }
  
}

void loop() {
  // Long-press to reset, if ya want.
  if (buttonLongPressed()) {
    reset();
  }
  // Button click to change color
  else if (buttonPressed()) {
    goToNextColor();
  }
  

  
}

void goToNextColor() {
  colorIndex ++;
  if (colorIndex >= COUNT_OF(colors)) {
    colorIndex = 1;
    setNumTimesChanged(numTimesChanged+1);
  }
  
  applyColor();
}


void applyColor() {
  setColor(colors[colorIndex]);
  
  FOREACH_FACE(f) {
    if (faceIsArrow[f]) {
      setColorOnFace(RED, f);
    }
  }
}

void setBoolArray(bool bools[], bool val) {
  for (int i=0; i<COUNT_OF(bools); i++) { bools[i] = val; }
}


//    if (!isValueReceivedOnFaceExpired(f)) {
