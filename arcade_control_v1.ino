#include <FastLED.h>

#define PIR_PIN  2   
#define RELAY_PIN  4
#define SWITCH_PIN 8

#define MARQUEE_PIN     11
#define BACKLIGHT_PIN     12

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS_MARQ  100      //Change to match number of LEDs in Marquee.  
#define NUM_LEDS_BACK  50       //Change to match number of LEDs on backlighting

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 500

boolean gameOn = false;
boolean switchOn = false;
boolean donePulsing = false;

int MARQcolor = CRGB::White;
int BACKcolor = CRGB::Blue;

CRGB MARQ_leds[NUM_LEDS_MARQ];
CRGB BL_leds[NUM_LEDS_BACK];

void setup() {
  Serial.begin(9600);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,HIGH);
  pinMode(MARQUEE_PIN, OUTPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);
  FastLED.addLeds<CHIPSET, BACKLIGHT_PIN, COLOR_ORDER>(BL_leds, NUM_LEDS_BACK).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, MARQUEE_PIN, COLOR_ORDER>(MARQ_leds, NUM_LEDS_MARQ).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 10 );

  Serial.println("/-------------------------\\");
  Serial.println("|   Arcade Control v1.0   |");
  Serial.println("\\-------------------------/");
  Serial.println("Initial Values");
  Serial.print("   Switch Value: ");
  Serial.println(digitalRead(SWITCH_PIN));
  Serial.print("   PIR Value:  ");
  Serial.println(digitalRead(PIR_PIN));

  //turn off all LEDS to begin...
  for (int j = 0; j<NUM_LEDS_MARQ; j++) {
      MARQ_leds[j] = CRGB::Black;
  }
  for (int j = 0; j<NUM_LEDS_BACK; j++) {
      BL_leds[j] = CRGB::Black;
  }
  FastLED.show();

  Serial.println("Starting Loop...");
}

void loop() {
  int curBrightness = 0;
  int pulseNumber = 0;
  int pulseDirection = 1;
  
  if(!gameOn && !digitalRead(SWITCH_PIN)) {
    // if the game system is not on, and the switch is on...
    Serial.println("Turning Game On...");
    Serial.println("Turn On Relay...");
    digitalWrite(RELAY_PIN,LOW);
    Serial.println("Ramping Marqee LEDs on...");
    for (int j = 0; j<NUM_LEDS_MARQ; j++) {
      MARQ_leds[j] = CRGB::White;
    }
    for (int j = 0; j<NUM_LEDS_BACK; j++) {
      BL_leds[j] = CRGB::Black;
    }
    for (int j=0; j<BRIGHTNESS;j++){
      FastLED.setBrightness(j);
      FastLED.show();
      delay(20);
    }
    gameOn = true;  // let the program know that the game system is on.     
    delay(500);  // wait a half sec to eliminate switch bounce.
  }
  else if(gameOn && digitalRead(SWITCH_PIN)) {
    // if the game system is on, but the switch is low, then power down
    Serial.println("Turning Game Off...");
    Serial.println("Turning off Relay...");
    digitalWrite(RELAY_PIN,HIGH);
    Serial.println("Ramping down marquee LEDs");
    for (int j=BRIGHTNESS; j>=0;j--){
      FastLED.setBrightness(j);
      FastLED.show();
      delay(20);
    }
    gameOn = false;
    delay(500);
  }
  else if(!gameOn && digitalRead(PIR_PIN)) {
    Serial.println("Motion Sensed...");
    for (int j = 0; j<NUM_LEDS_MARQ; j++) {
      MARQ_leds[j] = CRGB::Orange;
    }
    for (int j = 0; j<NUM_LEDS_BACK; j++) {
      BL_leds[j] = CRGB::Blue;
    }
    switchOn = !digitalRead(SWITCH_PIN);
    while(!switchOn && !donePulsing) {
      switchOn = !digitalRead(SWITCH_PIN);
      FastLED.setBrightness(curBrightness);
      FastLED.show();
      curBrightness = curBrightness+pulseDirection;
      if (curBrightness > BRIGHTNESS || curBrightness <=0) {
        pulseDirection = -pulseDirection;
        pulseNumber = pulseNumber+1;
      }
      if (pulseNumber > 5) {
        donePulsing = true;
      }
      delay(20);
    }
    
    donePulsing = false;
    for (int j = 0; j<NUM_LEDS_MARQ; j++) {
      MARQ_leds[j] = CRGB::Black;
    }
    FastLED.show();
  }
}
