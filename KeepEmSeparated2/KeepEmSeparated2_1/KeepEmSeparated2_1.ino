
/*
IDEAS


TODOS
Prevent only 1 of a color.
End state!
Do tree of life while prepping
EVENTUALLY
Flash red color
Neighbors on specific faces



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
enum GameState {UNDEFINED, PREPPING, PLAYING, RESULTS, RESETGO, RESETRESOLVE};
byte teamHues[] = {25, 100, 230 };
int NUM_TEAMS = 3;

// Properties
bool isHappy;
byte myTeam;
int numNeighborsSame;
int numNeighborsDifferent;
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
  switch (myGameState) {
    case PREPPING:
    case PLAYING:
    case RESULTS:
      updateLoopGameplay();
      break;
    case RESETGO:
      updateLoopResetGo();
      break;
    case RESETRESOLVE:
      updateLoopResetResolve();
      break;
  }

  // Update DISPLAY
  switch (myGameState) {
    case UNDEFINED: setColor(MAGENTA); break;
    case PREPPING: displayPrepping(); break;
    case PLAYING: displayPlaying(); break;
    case RESULTS: displayResults(); break;
    case RESETGO: displayResetGo(); break;
    case RESETRESOLVE: displayResetResolve(); break;
  }

}


void updateLoopGameplay() {
  // Double-click to start in reset-mode.
  if (buttonDoubleClicked()) {
    startResetGo();
  }
  // Time to go to next state?
  else if (millis() > timeWhenNextState) {
    goToNextState();
  }
  // Check my neighbors...
  else {
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
        byte neighborState = gameStateFromData(getLastValueReceivedOnFace(f));
        // Neighbor's in ResetGo? Join 'em!
        if (neighborState == RESETGO) {
          startResetGo();
          break;
        }
        // Neighbor's in the next game state? Join 'em!
        else if (neighborState == getNextGameState()) {
          goToNextState();
          break;
        }
      }
    }
  }

  // If I'm in state Playing, update neighbor info!
  if (myGameState == PLAYING) {
    updateNeighborInfo();
  }
}

void updateLoopResetGo() {
  bool isAnyNeighborInGameplay = false;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      byte neighborState = gameStateFromData(getLastValueReceivedOnFace(f));
      // If my neighbor is still in gameplay state...
      if (neighborState!=RESETGO && neighborState!=RESETRESOLVE) {
        isAnyNeighborInGameplay = true;
        break;
      }
    }
  }
  // If NO neighbor in a gameplay state, great! Go to resolve.
  if (!isAnyNeighborInGameplay) {
    startResetResolve();
  }
}
void updateLoopResetResolve() {
  bool isAnyNeighborInResetGo = false;
  FOREACH_FACE(f) {
  if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      byte neighborState = gameStateFromData(getLastValueReceivedOnFace(f));
      // If my neighbor's in ResetGo...
      if (neighborState == RESETGO) {
        isAnyNeighborInResetGo = true;
        break;
      }
    }
  }
  // If NO neighbor in ResetGo, great! Go into gameplay!
  if (!isAnyNeighborInResetGo) {
    startPrepping();
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
  return data >> 2 & 7;
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

void startResetGo() {
  setGameState(RESETGO);
  // Reset values!
  numRoundsPlayed = 0;
  numTimesWrong = 0;
}
void startResetResolve() {
  setGameState(RESETRESOLVE);
}
