// BlinksGo1_1
// by Brett Taylor
// Created 2/3/2020

/*
TODOS
Long-press to convert all adjacent tiles to my color.
Instead of red flashing, have it spiral opponent color happily! "Pick me up! I'm yours now!" wasCaptured = true. Click to set wasCaptured to false, and change its team.
 */


// Constants
Color teamColors[] = { WHITE, YELLOW, RED };
byte NUM_OPPONENT_NEIGHBORS_TO_GET_CAPTURED = 3;
// Properties
byte myTeam;
bool isCaptured; // set to TRUE when we're surrounded by enough opponents. Set to FALSE when our team is changed.
bool doDisplayDiagnostic = false;



// ==== SETUP ====
void setup() {
  setMyTeam(0); // start 'em all Team White.
}



// ==== LOOP ====
void loop() {
  // ---- UPDATE ----
  // Button Press (while alone) to change teams!
  if (buttonPressed() && isAlone()) {
    setMyTeam(getNextTeam()); // go to next team color.
  }
  updateIsCaptured();
  updateValueSentOnAllFaces();

  
  // ---- DISPLAY ----
  setColor(teamColors[myTeam]);
  displayIfIsCaptured();

  
  // DEBUG
  if (buttonLongPressed()) {
    doDisplayDiagnostic = !doDisplayDiagnostic;
  }
  if (doDisplayDiagnostic) {
    displayDiagnostic();
  }
}


byte getNextTeam() {
  if (myTeam == 0) return 1;
  else return 0;
}
void setMyTeam(int _val) {
  myTeam = _val;
  isCaptured = false;
  updateValueSentOnAllFaces();
}



void updateIsCaptured() {
  if (!isCaptured) { // If I've been captured, don't do the check! We stay captured.
    int numOpponentNeighbors = 0;
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) {
        byte neighborTeam = getTeamFromData(getLastValueReceivedOnFace(f));
        bool isNeighborCaptured = getIsCapturedFromData(getLastValueReceivedOnFace(f));
        if (neighborTeam != myTeam && !isNeighborCaptured) { numOpponentNeighbors ++; }
      }
    }
    isCaptured = numOpponentNeighbors >= NUM_OPPONENT_NEIGHBORS_TO_GET_CAPTURED;
  }
}


void displayIfIsCaptured() {
  if (isCaptured) {
    byte teamCapturedBy = getNextTeam();
    Color baseColor = teamColors[teamCapturedBy];
    setColor(dim(baseColor, 180));
    int face0 = int(millis()*0.015) % 6;
    setColorOnFace(GREEN, face0);
  }
}
void displayDiagnostic() {
  setColor(OFF);
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborTeam = getTeamFromData(getLastValueReceivedOnFace(f));
      bool isNeighborCaptured = getIsCapturedFromData(getLastValueReceivedOnFace(f));
      setColorOnFace(teamColors[neighborTeam], f);
      if (isNeighborCaptured && (millis()%1000 < 800)) {
        setColorOnFace(RED, f);
      }
    }
  }
}



// ==== SEND / GET DATA ====
void updateValueSentOnAllFaces() {
  byte data = myTeam;
  if (isCaptured) { data = data | 0b100; }
  setValueSentOnAllFaces(data);
}
byte getTeamFromData(byte data) {
  return data & 0b11;
}
byte getIsCapturedFromData(byte data) {
  return boolean((data >> 2) & 0b1);
}
