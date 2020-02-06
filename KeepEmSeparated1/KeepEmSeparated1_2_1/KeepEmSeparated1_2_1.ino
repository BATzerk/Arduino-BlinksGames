
/*
IDEAS
One, two, and three
Have same colors connect! That's it.
Neighbors on specific faces


TODOS
Hold one to reset all 
End state!
Do tree of life while prepping
EVENTUALLY
Flash red color


DONE
Start slower; get faster
More celebratory green
Make purple have spokes
Timer resync
Count how many times we A) rounds, and B) got wrong
WAIVED
Don't start unless 2+ neighbors?
Make wrong also flash its color

 */



// Constants
enum GameState {UNDEFINED, PREPPING, PLAYING, RESULTS};

// Properties
bool wantsNeighbor;
bool hasNeighbor;
bool isHappy; // true if we have a neighbor and want one, or the opposite.
byte myGameState;
byte myTeamHue;
byte numRoundsPlayed;
byte numTimesWrong; // just for me tho.
Color myTeamColor = makeColorRGB(0,255,255);
unsigned long timeWhenNextState;



// ==== SETUP ====
void setup() {
  randomize(); // must call to randomize seed across Blinks.
  startPrepping();
}



// ==== LOOP ====
void loop() {
  // Update LOGIC
  if (millis() > timeWhenNextState) {
    goToNextState();
  }
  else {
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
        if (getLastValueReceivedOnFace(f) == getNextGameState()) { // if my neighbor's in the next game state...!
          goToNextState();
          break;
        }
      }
    }
  }

  switch (myGameState) {
    case PLAYING:
      // Update neighbor info
      updateHasNeighbor();
      updateIsHappy();
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
  setValueSentOnAllFaces(myGameState); // also tell all my neighbors! if any are aboout to be this state, tell 'em to just switch now.
}

void startPrepping() {
  setGameState(PREPPING);
  timeWhenNextState = millis() + 3000;
}
void startPlaying() {
  setGameState(PLAYING);
  wantsNeighbor = random(100) < 50; // randomly decide if I want a neighbor
  myTeamHue = wantsNeighbor ? 120 : 235;
  myTeamColor = makeColorHSB(myTeamHue, 255,255);//wantsNeighbor ? c_wantsNeighbor : c_wantsSolo;
  timeWhenNextState = millis() + getRoundDuration();
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
