

// Constants
enum GameState {UNDEFINED, PREPPING, PLAYING, RESULTS};

// Properties
bool wantsNeighbor;
bool hasNeighbor;
bool isHappy; // true if we have a neighbor and want one, or the opposite.
byte myGameState;
byte myTeamHue;
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
  
  switch (myGameState) {
    case PLAYING:
      // Update neighbor info
      updateHasNeighbor();
      updateIsHappy();
      break;
  }
  
  if (buttonSingleClicked()) {
    startPrepping();
  }
  else if (buttonDoubleClicked()) {
    startResults();
  }
  
  

  // Update DISPLAY
  switch (myGameState) {
    case UNDEFINED:
      setColor(MAGENTA);
      break;

    case PREPPING:
      displayPrepping();
      break;
      
    case PLAYING:
      setColor(dim(myTeamColor, isHappy ? 255 : 120));
      break;
      
    case RESULTS:
      if (isHappy)
//        displayResultsHappy();
        setColor(GREEN);
      else
//        displayResultsNotHappy();
        setColor(RED);
      break;
  }

}



void goToNextState() {
  switch (myGameState) {
    case PREPPING:
      startPlaying();
      break;
    case PLAYING:
      startResults();
      break;
    case RESULTS:
      startPrepping();
      break;
  }
}


void startPrepping() {
  myGameState = PREPPING;
  timeWhenNextState = millis() + 3000;
}
void startPlaying() {
  myGameState = PLAYING;
  wantsNeighbor = random(100) < 50; // randomly decide if I want a neighbor
  myTeamHue = wantsNeighbor ? 120 : 235;
  myTeamColor = makeColorHSB(myTeamHue, 255,255);//wantsNeighbor ? c_wantsNeighbor : c_wantsSolo;
  timeWhenNextState = millis() + 4000;
}
void startResults() {
  myGameState = RESULTS;
  timeWhenNextState = millis() + 5000;
}
