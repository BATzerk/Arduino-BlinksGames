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
enum MatchState {UNDEFINED, PREIDLE, IDLE, MATCHED, BOMB, DETONATING, EXPLODING}; // Only Bombs go into DETONATING.
int teamHues[] = {15, 42, 110, 230, 75, 160};
//int NUM_TEAMS = 4;

// Properties
// TODO: Add willBecomeBomb flag instead! Check in goToNextState if should become bomb after matching.
byte myTeam;
byte myMatchState;
Color myTeamColor;
unsigned long timeWhenNextState;
int numTimesInitiatedAMatch = 0;
int myUpgradeLevel = 0; // use to tell what color we should be.



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
    case MATCHED:
      updateIdleOrMatched();
      break;
  }
  if (myMatchState==BOMB && buttonDoubleClicked()) {
    setStateDetonating();
  }

  // DISPLAY
  switch (myMatchState) {
    case UNDEFINED: setColor(MAGENTA); break;
    case PREIDLE: displayPreIdle(); break;
    case IDLE: displayIdle(); break;
    case MATCHED: displayMatched(); break;
    case BOMB: displayBomb(); break;
    case DETONATING: displayDetonating(); break;
    case EXPLODING: displayExploding(); break;
  }

  //  // DEBUG
  //  if (buttonLongPressed()) {
  //    setStatePreIdle();
  //  }
}

void updateIdleOrMatched() {
  int numNeighborsMyColor = 0;
  bool mayJoinMatch = myMatchState == IDLE;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      byte data = getLastValueReceivedOnFace(f);
      byte neighborTeam = getTeamFromData(data);
      byte neighborMatchState = getMatchStateFromData(data);
      bool canMatchWithNeighbor = neighborMatchState==IDLE || neighborMatchState==MATCHED;
      // Match?
      if (neighborTeam == myTeam && canMatchWithNeighbor) {
        numNeighborsMyColor ++;
        // Should I JOIN this match??
        if (mayJoinMatch && neighborMatchState == MATCHED) {
          setStateMatched();
        }
      }
      // Bomb?
      if (neighborMatchState==DETONATING) {
        setStateExploding();
      }
    }
  }
  // Check if I should create a MATCH!
  if (mayJoinMatch && numNeighborsMyColor >= 2) {
    numTimesInitiatedAMatch ++;
    setStateMatched();
  }
  // NOTE: Disabled bombs!
//  // Check if I should create a BOMB!
//  if (numNeighborsMyColor >= 3) {
//    setStateBomb();
//  }
}


// ==== DISPLAY ====
void displayIdle() {
  setColor(OFF);
  int numPips = numTimesInitiatedAMatch+1;
  for (int f=0; f<numPips; f++) {
    setColorOnFace(myTeamColor, f);
  }
}
void displayPreIdle() {
  float timeLeft = timeWhenNextState - millis();
  float locToNextState = (500 - timeLeft) / 500.0;
  Color color = dim(myTeamColor, locToNextState * 255);
  
  setColor(OFF);
  int numPips = numTimesInitiatedAMatch+1;
  for (int f=0; f<numPips; f++) {
    setColorOnFace(color, f);
  }
//  setColor(color);
}
void displayMatched() {
  setColor(myTeamColor);
  // Spiral happy white!
  int face0 = (millis() / 50) % FACE_COUNT;
  int face1 = (face0 + 1) % FACE_COUNT;
  int face2 = (face1 + 1) % FACE_COUNT;
  setColorOnFace(makeColorHSB(teamHues[myTeam],  0, 255), face0);
  setColorOnFace(makeColorHSB(teamHues[myTeam], 120, 255), face1);
  setColorOnFace(makeColorHSB(teamHues[myTeam], 220, 255), face2);
}
void displayBomb() {
  float alpha = (millis()%500) / 500.0;
  setColor(dim(WHITE, alpha*255));
}
void displayDetonating() {
  setColor(dim(WHITE, 155+random(100)));
}
void displayExploding() {
  setColor(dim(WHITE, 155+random(100)));
}



// ==== DOERS ====
void setMyTeam(byte _team) {
  myTeam = _team;
  myTeamColor = makeColorHSB(teamHues[myTeam], 255, 255);
  setColor(myTeamColor);
  updateValuesSentOnFaces(); // now we know what team I'm on, set my face values!
}
void setMyMatchState(MatchState _state) {
  myMatchState = _state;
  updateValuesSentOnFaces();
}

void setStatePreIdle() {
  // Set myTeam!
  int newTeam;
  if (myUpgradeLevel == 0) newTeam = random(3);//random(NUM_TEAMS - 1);
  else if (myUpgradeLevel == 1) newTeam = random(4);
  else if (myUpgradeLevel == 2) newTeam = random(5);
  else newTeam = 6;
  setMyTeam(newTeam);
  
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
  if (numTimesInitiatedAMatch >= 6) {
    myUpgradeLevel ++;
    numTimesInitiatedAMatch = 0;
  }
}
void setStateBomb() {
  setMyMatchState(BOMB);
  timeWhenNextState = 99999999;
}
void setStateDetonating() {
  setMyMatchState(DETONATING);
  timeWhenNextState = millis() + 1000;
}
void setStateExploding() {
  setMyMatchState(EXPLODING);
  timeWhenNextState = millis() + 1000;
}

void goToNextState() {
  switch (myMatchState) {
    case PREIDLE:
      setStateIdle(); break;
    case MATCHED:
    case DETONATING:
    case EXPLODING:
      setStatePreIdle(); break;
  }
}




// ==== DATA ====
void updateValuesSentOnFaces() {
  byte sendData = (myMatchState << 3) + (myTeam);
  setValueSentOnAllFaces(sendData); // also tell all my neighbors! if any are aboout to be this state, tell 'em to just switch now.
}
byte getMatchStateFromData(byte data) {
  return data >> 3 & 0b111;
}
byte getTeamFromData(byte data) {
  return data & 0b111;
}
