// Flickball
// by Brett Taylor
// Started 2/7/2020

/*
---- RULES ----
2P. 6+ Blinks. 5 min.
"It's like dodgeball, but with fundamental differences!"
CONTROLS: Click a Blink to change its team color.
SETUP: Players stand on opposite ends of a table, each with 3 Blinks of their team color. Connect your Blinks in a row. This is your Wall.
PLAY: Make sure the Blinks in your opponent's Wall are connected and in a line. On your turn, remove a Blink from your Wall and FLICK it at your opponent's Wall.
      Any opponent Blinks you connect with are now yours! Click to convert their team, and add them to your Wall.
      If your flicked Blink didn't connect with anything (oops), your opponent adds it to their Wall.
WIN: When all the Blinks are yours!

 */

// Constants
Color teamColors[] = { CYAN, ORANGE };
int NUM_TEAMS = 2;
// Properties
byte myTeam;


// ==== SETUP ====
void setup() {
  myTeam = 0;
  applyTeamColor();
}


// ==== LOOP ====
void loop() {
  // LOGIC
  if (buttonPressed()) {
    changeMyTeam();
  }
}


// ==== DOERS ====
void changeMyTeam() {
  myTeam ++;
  if (myTeam >= NUM_TEAMS) {
    myTeam = 0;
  }
  applyTeamColor();
}

void applyTeamColor() {
  setColor(teamColors[myTeam]);
}
