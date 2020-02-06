
#define HAPPY_FLASH_DURATION    500
#define EDGE_FADE_DURAION       500
#define SPARKLE_OFFSET          80
#define SPARKLE_DURATION        800
#define SPARKLE_CYCLE_DURATION  1600
Timer happyFlashTimer;
bool happyFlashOn;

byte sparkleOffset[6] = {0, 3, 5, 1, 4, 2};



void displayPrepping() {
  // Last 1 second
  unsigned long timeUntilNextState = timeWhenNextState - millis();
  if (timeUntilNextState < 1000) {
    setColor(dim(YELLOW, 140+random(115)));
  }
  else {
    setColor(YELLOW);
  }
}



void displayResultsHappy() {
  // have the color on the Blink raise and lower to feel more alive
  byte bri = 185 + sin8_C( (millis() / 14) % 255) * 70 / 255; // oscillate between values 185and 255
  setColor(dim(myTeamColor, bri));

  // lets do a celebration on each face in an order
  word delta = millis() % SPARKLE_CYCLE_DURATION; // 2 second cycle

  if (delta > SPARKLE_DURATION) {
    delta = SPARKLE_DURATION;
  }

  FOREACH_FACE(f) {

    // if the face has started it's glow
    uint16_t sparkleStart = sparkleOffset[f] * SPARKLE_OFFSET;
    uint16_t sparkleEnd = sparkleStart + SPARKLE_DURATION - (6 * SPARKLE_OFFSET);

    if ( delta > sparkleStart ) {
      // minimum of 125, maximum of 255
      word phaseShift = 60 * f;
      byte amplitude = 55;
      byte midline = 185;
      byte rate = 6;
      byte lowBri = midline + (amplitude * sin8_C( (phaseShift + millis() / rate) % 255) / 100);
      byte brightness;
      byte saturation;

      if ( delta < sparkleEnd ) {
        brightness = map(delta, sparkleStart, sparkleStart + SPARKLE_DURATION - (6 * SPARKLE_OFFSET), 255, lowBri);
        saturation = map(delta, sparkleStart, sparkleStart + SPARKLE_DURATION - (6 * SPARKLE_OFFSET), 0, 255);
      }
      else {
        //brightness = lowBri;
        saturation = 255;
      }

      Color faceColor = makeColorHSB(myTeamHue, saturation, 255);
      setColorOnFace(faceColor, f);
    }
  }
}

void displayResultsNotHappy() {
  // have the color on the Blink raise and lower to feel more alive
  byte bri = 185 + sin8_C( (millis() / 14) % 255) * 70 / 255; // oscillate between values 185and 255
  setColor(dim(myTeamColor, bri));
}
