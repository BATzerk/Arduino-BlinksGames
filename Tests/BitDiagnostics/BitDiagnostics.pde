

void setup() {
  byte myTeam = 0;
  boolean isCaptured = true;
  
  byte data = myTeam;
  if (isCaptured) {
    data = byte(data | 100);
  }
  
  byte teamFromData = byte(data & 11);
  boolean isCapturedFromData = boolean((data >> 2) & 1);
  
  println("sendData: " + data);
  println("team: " + teamFromData);
  println("isCaptured: " + isCapturedFromData);
  
  
  
  exit();
}


