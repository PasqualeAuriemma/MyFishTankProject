/*
  Aquarium Project Pasquale
*/

// Get kayboard value from analogic signal and return key
int getKeyValue()
{
  int val = 0;
  val = analogRead(keypadPin);
  
  if (val > 7 && val < 975) {
    if (val > 7 && val < 130) { //left
      return code[1];
    } else if (val > 131 && val < 315) { //down
      return code[2];
    } else if (val > 316 && val < 500) { //up
      return code[3];
    } else if (val > 501 && val < 710) { //right
      return code[0];
    } else {//if(val >756 && val < 880){ //ok
      return code[4];
    }
  } else { //no press
    return code[6];
  }
}

void resetVar() {
  key = code[6];
}
