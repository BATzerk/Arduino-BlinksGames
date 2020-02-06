// BlinksGo1_1
// by Brett Taylor
// Created 2/3/2020

/*
TODOS
Long-press to convert all adjacent tiles to my color.
Instead of red flashing, have it spiral opponent color happily! "Pick me up! I'm yours now!" wasCaptured = true. Click to set wasCaptured to false, and change its team.
 */


// Constants
Color teamColors[] = { WHITE, YELLOW };
byte NUM_TEAMS = COUNT_OF(teamColors);
byte NUM_OPPONENT_NEIGHBORS_TO_GET_CAPTURED = 3;
// Properties
byte myTeam = 0;



// ==== SETUP ====
void setup() {
  setMyTeam(0); // start 'em all Team Edward
}



// ==== LOOP ====
void loop() {
  
  // ---- UPDATE ----
  // Button Press to change teams!
  if (buttonPressed()) {
    setMyTeam(myTeam + 1); // go to next team color.
  }

  
  // ---- DISPLAY ----
  setColor(teamColors[myTeam]);
  displayRedIfOkToCapture();
}


void setMyTeam(int _val) {
  myTeam = _val;
  if (myTeam >= NUM_TEAMS) { // cycle back to 0.
    myTeam = 0;
  }
  updateValueSentOnAllFaces();
}




void displayRedIfOkToCapture() {
  int numOpponentNeighbors = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte neighborTeam = getTeamFromData(getLastValueReceivedOnFace(f));
      if (neighborTeam != myTeam) { numOpponentNeighbors ++; }
    }
  }
  bool isOkToCapture = numOpponentNeighbors >= NUM_OPPONENT_NEIGHBORS_TO_GET_CAPTURED;
  // Display red faces!
  if (isOkToCapture) {
    int startingFace = int(millis()*0.0016) % 2; // alternate starting face about every second.
    FOREACH_FACE(f) {
      if (f%2 == startingFace) {
        setColorOnFace(RED, f);
      }
    }
  }
}



// ==== SEND / GET DATA ====
void updateValueSentOnAllFaces() {
  setValueSentOnAllFaces(myTeam);
}
byte getTeamFromData(byte data) {
  return data;
}
