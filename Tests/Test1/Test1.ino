


Color colors[] = {
  makeColorHSB(0,0,0),
  WHITE,//makeColorHSB(90, 255, 255),
  YELLOW,//makeColorHSB(200, 255, 255)
//  GREEN,
//  ORANGE,
};

int colorIndex = 0;


void randomizeColor() {
  colorIndex = 1 + random(COUNT_OF(colors)-1);
}


void setup() {
  randomize();
  randomizeColor();
  applyColorIndex();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (buttonLongPressed()) {
    randomizeColor();
  }
  else if (buttonPressed()) {
    colorIndex ++;
    if (colorIndex >= COUNT_OF(colors)) {
      colorIndex = 0;
    }
  }

  applyColorIndex();
  
//  FOREACH_FACE(f) {
//    if (!isValueReceivedOnFaceExpired(f)) {
//      setColorOnFace(WHITE, f);
//    }
//  }
  
}



void applyColorIndex() {
//  setColor(makeColorHSB(hues[colorIndex], 255,255));//colors[colorIndex]);
  setColor(colors[colorIndex]);
}
