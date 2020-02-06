

// Constants
byte teamHues[] = {25, 100, 230 };


// ==== SETUP ====
void setup() {
  randomize(); // must call to randomize seed across Blinks.
}

// ==== LOOP ====
void loop() {
  setColor(OFF);
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      byte neighborTeam = teamFromData(getLastValueReceivedOnFace(f));
      Color teamColor = makeColorHSB(teamHues[neighborTeam], 255,255);
      setColorOnFace(teamColor, f);
    }
  }
}


//void updateValuesSentOnFaces() {
//  byte sendData = (myGameState << 2) + (myTeam);
//  setValueSentOnAllFaces(sendData); // also tell all my neighbors! if any are aboout to be this state, tell 'em to just switch now.
//}
byte gameStateFromData(byte data) {
  return data >> 2 & 7;
}
byte teamFromData(byte data) {
  return data & 3;
}
