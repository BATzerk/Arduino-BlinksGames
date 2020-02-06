

void updateNeighborInfo() {
  numNeighborsSame = 0;
  numNeighborsDifferent = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) { // if neighbor...
      bool isMatch = teamFromData(getLastValueReceivedOnFace(f)) == myTeam;
      if (isMatch) numNeighborsSame ++;
      else numNeighborsDifferent ++;
    }
  }

  isHappy = numNeighborsSame>0 && numNeighborsDifferent<=0;
}


unsigned long getRoundDuration() {
  if (numRoundsPlayed == 0) { return 6000; }
  if (numRoundsPlayed == 1) { return 5500; }
  if (numRoundsPlayed == 2) { return 5000; }
  if (numRoundsPlayed <= 8) { return 4500; }
//  if (numRoundsPlayed <= 12) { return 4200; }
  else { return 4000; }
}
