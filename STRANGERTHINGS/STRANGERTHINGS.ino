#define FASTLED_INTERNAL
#include <FastLED.h>

FASTLED_USING_NAMESPACE



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    100
CRGB leds[NUM_LEDS];
int z = 0;
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120
int k =0;


// After setting up the letters of the wall, assign each letter to the two closest LEDs
// Defining them in this list format is a little easier to reference
// List contains 27 entries to simply the letter to number relationship (A=1, B=1, ...., Z=26) 
int alphabets[27][2] = { 
  
  {0,0}, //0
  {99,98}, //A
  {95,94}, //B
  {92,91}, //C
  {88,89}, //D
  {85,84}, //E
  {81,82}, //F
  {78,77}, //G
  {74,75}, //H
  {71,72}, //I
  
  {40,41}, //J
  {44,45}, //K
  {47,48}, //L
  {52,51}, //M
  {56,55}, //N
  {59,58}, //O
  {62,61}, //P
  {66,65}, //Q
  
  {34,35}, //R
  {30,31}, //S
  {28,27}, //T
  {23,24}, //U
  {21,20}, //V
  {17,18}, //W
  {14,13}, //X
  {12,11}, //Y
  {9,8} //Z 
  };


//Set a string of nine colors to use in some of the patterns
int ninecolors[9][3] = {
  {0,255,0}, //red
  {0,0,255}, //blue
  {80,255,0}, //orange
  {255,167,30}, //light green
  {22,255,255}, //purple
  {255,255,22}, //yellow
  {255,0,0}, //green
  {40,255,66}, //pink
  {255,20,188} //turq
};



//Initializes the array for spelling words. This allows a maximum of 9 letters. 
int word_spell[9] = {0,0,0,0,0,0,0,0,0};

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {  sinelon, STRANGE, rainbow, rainbowWithGlitter, confetti, bpm , UPandDown  };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 45 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 9);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}




void STRANGE() {

//Each LED lights up white, setting an indication that a "message" is coming
  FastLED.clear();
  for(int i = 0; i <=99; i++){
    leds[i] = CRGB (255,255,255);
    FastLED.show();
    delay(15);
    
  }
  delay(1000);
  FastLED.clear();




int z = random(1,4); //chooses a random number and will display one of the words below if chosen



if (z == 1){

HIDE(); 

}

if (z == 2){


DANCE();

}


if (z == 3){
RUN();

}



if (z == 4){
HELPME();


}



mypatt(); //this is a filler pattern I wrote to take up the rest of the time remaining in the STRANGE function. 

}










void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter2() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(160);
}



void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 60);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 40);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}




void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 40;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


void colortemp(){
  // draw a generic, no-name rainbow
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 60);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (20 * 2);
  if( secs < 60) {
    FastLED.setTemperature( Tungsten100W ); // first temperature
    leds[0] = Tungsten100W; // show indicator pixel
  } else {
    FastLED.setTemperature( OvercastSky ); // second temperature
    leds[0] = OvercastSky; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  if( (secs % 60) < 3) {
    memset8( leds, 0, NUM_LEDS * sizeof(CRGB));
  }
  
  FastLED.show();
  FastLED.delay(3);
}



void HIDE() { 

  int word_spell[9] = {8,9,4,5,0,0,0,0,0}; //each number refers to the letter
 
  spelling(word_spell,4); //takes the "word" string and the number of letters in the word as inputs
}



void RUN(){
  int word_spell[9] = {18,21,14,0,0,0,0,0,0};
  
  spelling(word_spell,3);
  
}


void DANCE(){
  int word_spell[9] = {4,1,14,3,5,0,0,0,0};
 
  spelling(word_spell,5);
}


void HELPME(){
  int word_spell[9] = {8,5,12,16,13,5,0,0,0};
  
  spelling(word_spell,6);
}



void spelling(int word_spell[9],int kk) {  //takes the "word" string and the number of letters (kk) in the word as inputs


  FastLED.clear();

for (int i = 0; i < kk; i = i+1){
  
   int r1 = ninecolors[i][0];
   int g1 = ninecolors[i][1];
   int b1 = ninecolors[i][2];
  

  int currentletter = word_spell[i];


//lights each letter up
  leds[(alphabets[word_spell[i]][0])] = CRGB (r1,g1,b1);
  leds[(alphabets[word_spell[i]][1])] = CRGB (r1,g1,b1);

        FastLED.show();
      delay(1500); 

//turn each letter off
  leds[(alphabets[word_spell[i]][0])] = CRGB (0,0,0);
  leds[(alphabets[word_spell[i]][1])] = CRGB (0,0,0);

        FastLED.show();
      delay(1500); 

}

//displays all letters
for (int i = 0; i < kk; i = i+1){
   int r1 = ninecolors[i][0];
   int g1 = ninecolors[i][1];
   int b1 = ninecolors[i][2];
for (int j = 0; j < 2; j = j+1){
  leds[(alphabets[word_spell[i]][j])] = CRGB (r1,g1,b1);
  

        
 
}
FastLED.show();
         delay(1500); 
}
}



void mypatt(){ //a pattern I wrote as a "filler" in the STRANGE function
  FastLED.clear();

  for( int j = 6; j>0; j = j-1){
  for (int i = 0; i <=97; i++){

        
   int r1 = ninecolors[i % 9][0];
   int g1 = ninecolors[i % 9][1];
   int b1 = ninecolors[i % 9][2];

  leds[i] = CRGB (r1,g1,b1);
        FastLED.show();
      delay(10*j/2); 
  leds[i+2] = CRGB (r1,g1,b1);
        FastLED.show();
      delay(10*j/2); 
  leds[i] = CRGB (0,0,0);
  FastLED.show();
  }

  for (int i = 99; i >=2; i= i -1){

        
   int r1 = ninecolors[i % 9][0];
   int g1 = ninecolors[i % 9][1];
   int b1 = ninecolors[i % 9][2];

  leds[i] = CRGB (r1,g1,b1);
        FastLED.show();
      delay(10*j/2.5); 
  leds[i-2] = CRGB (r1,g1,b1);
        FastLED.show();
      delay(10*j/2.5); 
  leds[i] = CRGB (0,0,0);
  FastLED.show();
  }
  
}
}




void UPandDown(){ //another pattern I tried to write to go up and down my rows of lights. This is very dependent on the placement of the LEDS, so it may not be that useful.
    FastLED.clear();
   
int startpoint[3] = {98, 40, 34}; //the three most left LEDS, one for each row of lights
int endpoint[3] = {71, 66, 8}; //the three most right LEDS, one for each row of lights



for (int freq = 1; freq <=4; freq++){
for (int i = 1; i <=26; i++){

if(i %2 ==0) {

  for (int j = 2; j >=0; j--){

if (j==1){
   leds[startpoint[j] +i] = CRGB (245,254,36); //the color green
   FastLED.show();
  delay(100/freq);
 leds[startpoint[j] + i] = CRGB (0,0,0);
 
   FastLED.show();
}

 else{   
  leds[startpoint[j] -  i] = CRGB (245,254,36);
   FastLED.show();
  delay(100/freq);
 leds[startpoint[j] -  i] = CRGB (0,0,0);
 
   FastLED.show();
 }

  } 
}

  
 else{ for (int j = 0; j <=2; j++){

if (j==1){
   leds[startpoint[j] +i] = CRGB (245,254,36);
   FastLED.show();
  delay(100/freq);
 leds[startpoint[j] + i] = CRGB (0,0,0);
 
   FastLED.show();
}

 else{   
  leds[startpoint[j] -  i] = CRGB (245,254,36);
   FastLED.show();
  delay(100/freq);
 leds[startpoint[j] -  i] = CRGB (0,0,0);
 
   FastLED.show();
 }

  }  }
}
}
}
