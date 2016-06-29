#include "Button.h"


//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND

#define SET(x,y) (x |=(1<<y))       //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))           // |
#define CHK(x,y) (x & (1<<y))               // |
#define TOG(x,y) (x^=(1<<y))                //-+

#define N 160  //How many frequencies

float results[N];                 //-Filtered result buffer
float freq[N];                    //-Filtered result buffer
const int sizeOfArray = N;


#define RESTING 0
#define FOOD 1
#define GRAB 2
#define NUM_GESTURES 3

Button buttons[NUM_GESTURES] = { Button(10), Button(11), Button(12) };
float gesturePoints[NUM_GESTURES][2] = {{0.0, 0.0}, {0.0, 0.0}};

void setup()
{
  TCCR1A = 0b10000010;            //-Set up frequency generator
  TCCR1B = 0b00011001;            //-+
  ICR1 = 110;
  OCR1A = 55;

  pinMode(9,OUTPUT);              //-Signal generator pin
  pinMode(8,OUTPUT);              //-Sync (test) pin



  Serial.begin(115200);

  for(int i = 0; i < N; i++) {    //-Preset results
    results[i] = 0;               //-+
  }
}

void updateButtons() {
  for(unsigned int i = 0; i < NUM_GESTURES; i++) {
    buttons[i].update();
  }
}

void loop()
{
  float maxResult = 0;
  unsigned int maxIdx = 0;
  for(unsigned int d = 0; d < N; d++)
  {
    const int v = analogRead(0);  //-Read response signal
    CLR(TCCR1B, 0);               //-Stop generator
    TCNT1 = 0;                    //-Reload new frequency
    ICR1 = d;                     // |
    OCR1A = d / 2;                //-+
    SET(TCCR1B, 0);               //-Restart generator

    results[d] = results[d] * 0.5 + (float)(v) * 0.5;   //Filter results

    freq[d] = d;

    if(results[d] > maxResult) {
      maxResult = results[d];
      maxIdx = d;
    }
    // plot(v, 0);              //-Display
    // plot(results[d], 1);
    // delayMicroseconds(1);

    // Serial.println(results[d]);
  }

  updateButtons();
  for(unsigned int i = 0; i < NUM_GESTURES; i++) {
    if(buttons[i].getState() == HIGH){
      gesturePoints[i][0] = freq[maxIdx];
      gesturePoints[i][1] = results[maxIdx];
    }
  }

  // PlottArray(1, freq, results);

  TOG(PORTB, 0);            //-Toggle pin 8 after each sweep (good for scope)
}
