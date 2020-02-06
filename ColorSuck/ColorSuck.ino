

// Constants
enum PaletteState {Undefined, Sucking, Blowing};
Color allColors[] = {OFF, RED,BLUE,GREEN, CYAN,YELLOW,MAGENTA};

// Properties
bool isPalette;
byte myFaceColors[6];
PaletteState currPaletteState;





// ==== SETUP ====
void setup() {
  randomize();
  
  isPalette = false;
  reset();
}



void reset() {
  if (isPalette) {
    for (int i=0; i<COUNT_OF(myFaceColors); i++) {
      myFaceColors[i] = 0;
    }
  }
  else {
    for (int i=0; i<COUNT_OF(myFaceColors); i++) {
      myFaceColors[i] = random(COUNT_OF(allColors));
    }
  }
  updateValuesSentOnFaces();
}



// ==== LOOP ====
void loop() {
  if (buttonLongPressed()) {
    isPalette = !isPalette;
    reset();
  }


  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      if (myFaceColors[f] == 0) {
        myFaceColors[f] = colorFromData(getLastValueReceivedOnFace(f));
        updateValuesSentOnFaces();
      }
    }
  }



  // DISPLAY
  FOREACH_FACE(f) {
    setColorOnFace(allColors[myFaceColors[f]], f);
  }
  // QQQ
  if (isPalette) {
    setColor(OFF);
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
        byte colorByte = colorFromData(getLastValueReceivedOnFace(f));
        setColorOnFace(allColors[colorByte], f);
      }
    }
  }
  
}


void updateValuesSentOnFaces() {
  FOREACH_FACE(f) {
    byte sendData = myFaceColors[f];//(myGameState << 2) + (myTeam);
    setValueSentOnFace(sendData, f);
  }
}
//byte gameStateFromData(byte data) {
//  return data >> 2 & 3;
//}

byte colorFromData(byte data) {
  return data;// & 3;
}
