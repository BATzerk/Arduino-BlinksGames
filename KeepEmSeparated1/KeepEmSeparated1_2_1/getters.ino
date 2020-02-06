

bool updateHasNeighbor() {
  hasNeighbor = false;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      hasNeighbor = true;
      break;
    }
  }
}

bool updateIsHappy() {
  isHappy =
       (wantsNeighbor && hasNeighbor)
    || (!wantsNeighbor && !hasNeighbor);
}

unsigned long getRoundDuration() {
  if (numRoundsPlayed == 0) { return 6000; }
  if (numRoundsPlayed == 1) { return 5000; }
  if (numRoundsPlayed == 2) { return 4500; }
  if (numRoundsPlayed <= 10) { return 4000; }
  else { return 3500; }
}
