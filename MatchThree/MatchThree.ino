// MatchThree (working title)
// by Brett Taylor
// Started 2/7/2020

/*
---- RULES ----
1P. 12+ Blinks.
SETUP: Cluster all Blinks together.
PLAY: Make a move to match as many like-colors as possible! Fracture-like splitting rules.
      Pull a chunk (or even 1) of Blinks off, rotate as desired, and stick it back on to the main cluster.
      Three or more Blinks touching will match, clear, and change color!
WINNING: Currently no endgame right now. Perhaps could add a scoring and moves count system though.
NOTE: If playing with 10 or 11 Blinks, you should set it NUM_TEAMS to 4 instead of 5.



TODOS (DONE)
Add state PREIDLE. Fade in my color for 0.5 seconds.
Make happy flash spiral white around my color!
Each Blink checks if it's touching 2+ of its team. If YES, then say isMatchHappy. Broadcast it.
If isMatchHappy, flicker rainbow for 1 second, then randomize my team.
If my NEIGHbor is isMatchHappy and we MATCH, then also set ME isMatchHappy!

MAYBES
New ones that come in shouldn't match
4+ in a row must be rewarded, man


*/



// Constants
enum MatchState {UNDEFINED, PREIDLE, IDLE, MATCHED};
int teamHues[] = {15,42,110,230,75};
int NUM_TEAMS = 5;

// Properties
byte myTeam;
byte myMatchState;
Color myTeamColor;
unsigned long timeWhenNextState;



// ==== SETUP ====
void setup() {
  randomize(); // must call to randomize seed across Blinks.
  setStatePreIdle();
}



// ==== LOOP ====
void loop() {
  // LOGIC
  if (millis() > timeWhenNextState) {
    goToNextState();
  }
  switch (myMatchState) {
    case IDLE:
      updateIdle();
      break;
  }

  // DISPLAY
  switch (myMatchState) {
    case UNDEFINED: setColor(MAGENTA); break;
    case PREIDLE: displayPreIdle(); break;
    case IDLE: displayIdle(); break;
    case MATCHED: displayMatched(); break;
  }
  
//  // DEBUG
//  if (buttonLongPressed()) {
//    setStatePreIdle();
//  }
}

void updateIdle() {
  int numNeighborsMyColor = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte data = getLastValueReceivedOnFace(f);
      byte neighborTeam = getTeamFromData(data);
      byte neighborMatchState = getMatchStateFromData(data);
      if (neighborTeam == myTeam && neighborMatchState != PREIDLE) {
        numNeighborsMyColor ++;
        // Should I JOIN this match??
        if (neighborMatchState == MATCHED) {
          setStateMatched();
        }
      }
    }
  }
  // Check if I should CREATE a match!
  if (numNeighborsMyColor >= 2) {
    setStateMatched();
  }
}


// ==== DISPLAY ====
void displayIdle() {
  setColor(myTeamColor);
}
void displayPreIdle() {
  float timeLeft = timeWhenNextState - millis();
  float locToNextState = (500-timeLeft) / 500.0;
  Color color = dim(myTeamColor, locToNextState*255);
  setColor(color);
}
void displayMatched() {
  setColor(myTeamColor);
  // Spiral happy white!
  int face0 = (millis()/50) % FACE_COUNT;
  int face1 = (face0+1) % FACE_COUNT;
  int face2 = (face1+1) % FACE_COUNT;
  setColorOnFace(makeColorHSB(teamHues[myTeam],  0,255), face0);
  setColorOnFace(makeColorHSB(teamHues[myTeam],120,255), face1);
  setColorOnFace(makeColorHSB(teamHues[myTeam],220,255), face2);
}



// ==== DOERS ====
void setMyTeam(byte _team) {
  myTeam = _team;
  myTeamColor = makeColorHSB(teamHues[myTeam],255,255);
  setColor(myTeamColor);
  updateValuesSentOnFaces(); // now we know what team I'm on, set my face values!
}
void setMyMatchState(MatchState _state) {
  myMatchState = _state;
  updateValuesSentOnFaces();
}

void setStatePreIdle() {
  setMyTeam(random(NUM_TEAMS-1));
  setMyMatchState(PREIDLE);
  timeWhenNextState = millis() + 500;
}
void setStateIdle() {
  setMyMatchState(IDLE);
  timeWhenNextState = 99999999;
}
void setStateMatched() {
  setMyMatchState(MATCHED);
  timeWhenNextState = millis() + 500;
}

void goToNextState() {
  switch (myMatchState) {
    case PREIDLE: setStateIdle(); break;
    case MATCHED: setStatePreIdle(); break;
  }
}




// ==== DATA ====
void updateValuesSentOnFaces() {
  byte sendData = (myMatchState << 3) + (myTeam);
  setValueSentOnAllFaces(sendData); // also tell all my neighbors! if any are aboout to be this state, tell 'em to just switch now.
}
byte getMatchStateFromData(byte data) {
  return data >> 3 & 0b11;
}
byte getTeamFromData(byte data) {
  return data & 0b111;
}
