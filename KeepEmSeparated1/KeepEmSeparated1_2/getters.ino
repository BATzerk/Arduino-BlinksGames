

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

  
