

// Constants
enum State {UNDEFINED, PLAYING, RESET};
Color c_wantsNeighbor = makeColorHSB(120,255,255);
Color c_wantsSolo = makeColorHSB(0,255,255);

// Properties
bool wantsNeighbor;
byte myState;
unsigned long timeToStopResetting;



// ==== SETUP ====
void setup() {
  randomize();
  reset();
}



// ==== LOOP ====
void loop() {
  // Fundamental Resets
  if (buttonSingleClicked()) {
    startResettingAll();
  }

  // Listen for state changes
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborVal = getLastValueReceivedOnFace(f);
      if (neighborVal == RESET && mayReset()) {
        reset();
      }
    }
  }
  if (myState == RESET && millis()>timeToStopResetting) {
    myState = PLAYING;
  }

  
  // Update neighbor info
  bool hasNeighbor = false;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      hasNeighbor = true;
      break;
    }
  }


  // Update colors
  if (myState == RESET) {
      setColor(dim(c_wantsNeighbor, brightness));
  }
  else {
    if (wantsNeighbor) {
      byte brightness = hasNeighbor ? 255 : 120;
      setColor(dim(c_wantsNeighbor, brightness));
    }
    else {
      byte brightness = hasNeighbor ? 120 : 255;
      setColor(dim(c_wantsSolo, brightness));
    }
  }

}




bool mayReset() { return millis() > timeToStopResetting; }

void reset() {
  wantsNeighbor = random(100) < 50; // randomly decide if I want a neighbor
  timeToStopResetting = millis() + 1000; // stop resetting in 1 second
  setValueSentOnAllFaces(RESET);
}
