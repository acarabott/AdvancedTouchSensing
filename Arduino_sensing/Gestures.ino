#define TONE_PIN 7

void gestureSetup() {
  pinMode(TONE_PIN, OUTPUT);
}

uint8_t getCurrentBeat(uint32_t startTime, uint8_t numBeats, uint16_t beatDur) {
  uint16_t totalDur = numBeats * beatDur;
  uint32_t relativeTime = millis() - startTime;
  return (relativeTime % totalDur) / beatDur;
}

void touchResponse(uint32_t startTime) {
  const uint8_t currentBeat = getCurrentBeat(startTime, 24, 62);

  if(currentBeat == 0 || currentBeat == 4){
    tone(TONE_PIN, random(2114, 2162), 100);
  } else if(currentBeat == 1 || currentBeat == 5){
    tone(TONE_PIN, random(2269, 2319), 100);
  } else {
    noTone(TONE_PIN);
  }
}

void grabResponse(uint32_t startTime) {
  const uint8_t currentBeat = getCurrentBeat(startTime, 6, 130);

  if(currentBeat % 2 == 0) {
    tone(TONE_PIN, random(2500, 3000), 100);
  } else {
    noTone(TONE_PIN);
  }
}

void gestureResponse(uint16_t gesture, uint32_t gestureStartTime) {
  switch (gesture) {
    case 0:   noTone(TONE_PIN);                 break;
    case 1:   touchResponse(gestureStartTime);  break;
    case 2:   grabResponse(gestureStartTime);   break;
    default:  noTone(TONE_PIN);                 break;
  }
}