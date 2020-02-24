// KeepEmSeparated2
// by Brett Taylor
// Created on February 1 2020, for Global Game Jam 2020!

/*
---- RULES ----
1+ Players. 6+ Blinks. 5-10 Min.
"Put like-colors together!"
SETUP: Cluster Blinks in one large group. Double click any Blink to start (or reset) the game.
GAMEPLAY: Each round (~12 seconds) starts when the Blinks turn yellow— when they do, slide them together into a big cluster and get ready!
          In a moment, they’ll reveal their colors, and when they do, you only have a few seconds to separate them into their color groups!
          Blinks that are touching a different color will turn red. Ouch! Let’s end each round with all Blinks green and happy, ok? Keep colors separated. Ok? Promise?
WIN: There’s no end game. Play until you feel your worries have melted away. <3
----------------


TODOS
Test the below changes!
If never saw neighbor of same color, get happy if no neighbors! If saw neighbor, get unhappy.
First two rounds, only pick first two colors.

EVENTUALLY
Win/lose state!
Prevent only 1 of a color.
Do tree of life while prepping

IDEAS
Neighbors on specific faces

 */


// Constants
enum GameState {GAMESTATE_UNDEFINED, PREPPING, PLAYING, RESULTS, RESETGO, RESETRESOLVE};
enum HappinessTypes {HAPPINESS_UNDEFINED, UNHAPPY, HAPPY}; // unhappy if a bad match. happy if neighbors, and all match me!
byte teamHues[] = {25, 100, 230 };
int NUM_TEAMS = 3;

// Properties
byte myHappiness;
byte myTeam;
bool didSeeMatchingNeighbor; // we use this to resolve single Blink scenarios. If we HAVE seen a matching neighbor and we're alone, we're NOT happy.
int numNeighborsSame;
int numNeighborsDifferent;
byte myGameState;
byte myTeamHue;
byte numRoundsPlayed;
byte numTimesUnhappy; // just for me tho.
Color myTeamColor = makeColorRGB(0,255,255);
unsigned long timeWhenNextState;




// ==== SETUP ====
void setup() {
  randomize(); // must call to randomize seed across Blinks.
  startPrepping();
}




// ==== GETTERS ====
unsigned long getRoundDuration() {
  if (numRoundsPlayed == 0) { return 6000; }
  if (numRoundsPlayed == 1) { return 5500; }
  if (numRoundsPlayed == 2) { return 5000; }
  if (numRoundsPlayed <= 8) { return 4500; }
  else { return 4000; }
}
byte getRandomTeam() {
  if (numRoundsPlayed <= 2) { // first 2 rounds, only have 2 colors. Keep it easy, yo.
    return random(NUM_TEAMS-2);
  }
  return random(NUM_TEAMS-1);
}



// ==== LOOP ====
void loop() {
  // LOGIC
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

  // DISPLAY
  switch (myGameState) {
    case PREPPING: displayPrepping(); break;
    case PLAYING: displayPlaying(); break;
    case RESULTS: displayResults(); break;
    case RESETGO: displayResetGo(); break;
    case RESETRESOLVE: displayResetResolve(); break;
    default: setColor(MAGENTA); break;
  }
}


// ==== UPDATES ====
void updateLoopGameplay() {
  // Double-click to start in reset-mode.
  if (buttonLongPressed()) {
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
  // If I'm in state Playing, also update isHappy!
  if (myGameState == PLAYING) {
    updateHappiness();
  }
}

void updateHappiness() {
  numNeighborsSame = 0;
  numNeighborsDifferent = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      byte neighborTeam = teamFromData(getLastValueReceivedOnFace(f));
      if (neighborTeam == myTeam) {
        numNeighborsSame ++;
        didSeeMatchingNeighbor = true;
      }
      else {
        numNeighborsDifferent ++;
      }
    }
  }
  // Any different neighbors? Unhappy.
  if (numNeighborsDifferent > 0) { myHappiness = UNHAPPY; }
  // I've got at least one same neighbor? Happy!
  else if (numNeighborsSame > 0) { myHappiness = HAPPY; }
  // I've no neighbors. I'm happy if I HAVEN'T seen a matching neighbor.
  else { myHappiness = didSeeMatchingNeighbor ? UNHAPPY : HAPPY; }
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




// ==== SENDING/HEARING DATA ===
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




// ==== GAME STATE CHANGING ====
byte getNextGameState() {
  switch (myGameState) {
    case PREPPING: return PLAYING;
    case PLAYING: return RESULTS;
    case RESULTS: return PREPPING;
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


void setGameState(byte _state) {
  myGameState = _state;
  updateValuesSentOnFaces();
}
void startPrepping() {
  setGameState(PREPPING);
  timeWhenNextState = millis() + 3000;
  didSeeMatchingNeighbor = false;
}
void startPlaying() {
  setGameState(PLAYING);
  myTeam = getRandomTeam();
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
  if (myHappiness == UNHAPPY) {
    numTimesUnhappy ++;
  }
}
void startResetGo() {
  setGameState(RESETGO);
  // Reset values!
  numRoundsPlayed = 0;
  numTimesUnhappy = 0;
}
void startResetResolve() {
  setGameState(RESETRESOLVE);
}





// ==== DISPLAY ====
void displayPrepping() {
  // Last 1 second
  unsigned long timeUntilNextState = timeWhenNextState - millis();
  if (timeUntilNextState < 1000) { // Flicker for last second.
    setColor(dim(YELLOW, 140+random(115)));
  }
  else {
    setColor(YELLOW);
  }
}

void displayPlaying() {
//  Color displayColor = dim(myTeamColor, isHappy ? 255 : 120);
  setColor(myTeamColor);//displayColor);
}

void displayResults() {
  if (myHappiness == HAPPY) {
    setColor(GREEN);
  }
  else if (myHappiness == UNHAPPY) {
    setColor(RED);
  }
//  else { // If neutral, leave my color displayed (though a little dimmed).
//    setColor(dim(myTeamColor, 150));
//  }
}

void displayResetGo() {
  setColor(WHITE);
}
void displayResetResolve() {
  setColor(dim(WHITE, 120));
}
