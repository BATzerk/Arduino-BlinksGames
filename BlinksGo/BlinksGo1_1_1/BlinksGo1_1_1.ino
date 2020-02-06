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
bool isCaptured; // set to TRUE when we're surrounded by enough opponents. Set to FALSE when our team is changed.



// ==== SETUP ====
void setup() {
  setMyTeam(0); // start 'em all Team Edward
}



// ==== LOOP ====
void loop() {
  // ---- UPDATE ----
  // Button Press to change teams!
  if (buttonPressed()) {
    setMyTeam(getNextTeam()); // go to next team color.
  }
  updateIsCaptured();

  
  // ---- DISPLAY ----
  setColor(teamColors[myTeam]);
  displayIfIsCaptured();
}


byte getNextTeam() {
  int nextTeam = myTeam;
  nextTeam += 1;
  if (nextTeam >= NUM_TEAMS) { // cycle back to 0.
    nextTeam = 0;
  }
  return nextTeam;
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
        if (neighborTeam != myTeam) { numOpponentNeighbors ++; }
      }
    }
    isCaptured = numOpponentNeighbors >= NUM_OPPONENT_NEIGHBORS_TO_GET_CAPTURED;
  }
}


void displayIfIsCaptured() {
  if (isCaptured) {
    byte teamCapturedBy = getNextTeam();
    Color baseColor = teamColors[teamCapturedBy];
    setColor(baseColor);
    int face0 = int(millis()*0.015) % 6;
    setColorOnFace(GREEN, face0);
  }
}



// ==== SEND / GET DATA ====
void updateValueSentOnAllFaces() {
  setValueSentOnAllFaces(myTeam);
}
byte getTeamFromData(byte data) {
  return data;
}
