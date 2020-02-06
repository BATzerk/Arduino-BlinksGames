
/*
New ones that come in shouldn't match
4+ in a row must be rewarded, man


 */



// Constants
enum GameState {UNDEFINED, PREPPING, PLAYING, RESULTS};
byte teamHues[] = {15, 42, 110, 230 };
int NUM_TEAMS = 4;

// Properties
byte myTeam;
byte myGameState;
byte myTeamHue;
Color myTeamColor = makeColorRGB(0,255,255);
unsigned long timeWhenNextState;



// ==== SETUP ====
void setup() {
  randomize(); // must call to randomize seed across Blinks.
  resetTeam();
}



// ==== LOOP ====
void loop() {
  // Update LOGIC
  if (millis() > timeWhenNextState) {
    goToNextState();
  }

  switch (myGameState) {
    case PLAYING:
      updateNeighborInfo();
      break;
  }
  
  if (buttonLongPressed()) {
    startPrepping();
  }
  
  

  // Update DISPLAY
  switch (myGameState) {
    case UNDEFINED: setColor(MAGENTA); break;
    case PREPPING: displayPrepping(); break;
    case PLAYING: displayPlaying(); break;
    case RESULTS: displayResults(); break;
  }

}



void goToNextState() {
  myGameState = getNextGameState();
  switch (myGameState) {
    case PREPPING: startPrepping(); break;
    case PLAYING: startPlaying(); break;
    case RESULTS: startResults(); break;
  }
}
byte getNextGameState() {
  switch (myGameState) {
    case PREPPING: return PLAYING;
    case PLAYING: return RESULTS;
    case RESULTS: return PREPPING;
  }
}


void setGameState(byte _state) {
  myGameState = _state;
  updateValuesSentOnFaces();
}
void updateValuesSentOnFaces() {
  byte sendData = (myGameState << 2) + (myTeam);
  setValueSentOnAllFaces(sendData); // also tell all my neighbors! if any are aboout to be this state, tell 'em to just switch now.
}
byte gameStateFromData(byte data) {
  return data >> 2 & 3;
}
byte teamFromData(byte data) {
  return data & 3;
}

void startPrepping() {
  setGameState(PREPPING);
  timeWhenNextState = millis() + 3000;
}
void startPlaying() {
  setGameState(PLAYING);
  myTeam = random(NUM_TEAMS-1);
  myTeamHue = teamHues[myTeam];
  myTeamColor = makeColorHSB(myTeamHue, 255,255);
  timeWhenNextState = millis() + getRoundDuration();
  updateValuesSentOnFaces(); // now we know what team I'm on, set my face values!
}
void startResults() {
  setGameState(RESULTS);
  timeWhenNextState = millis() + 5000;
  // Increment score
  numRoundsPlayed ++;
  if (!isHappy) {
    numTimesWrong ++;
  }
}
