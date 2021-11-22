
#include <Adafruit_NeoPixel.h>

// change this one later i guess?
#define LED_PIN 0
// remove this one later i guess?
#define PIXELS 1

#define SPEED 50
#define HOVER_POS_MIN 0
#define HOVER_POS_MAX 180
#define HOVER_STR_MAX 4

uint8_t index = 130; // where in the gradient to start (should be inside HOVER_POS bounds)

Adafruit_NeoPixel pixels(PIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);

/*
 * Define color maps for R, G and B respectively. We can't store entire tables (no room) but it's possible to
 * store the points at least, and interpolate in real time. The arrays go like this: a position followed by a value,
 * repeating. so {0,0,255,255} would be a linear rise, {0,255,255,0} would be a linear fall,
 * {0,0, 128,255, 255,0} would be like a "triangle" /\ and so forth. The channels don't have to have their
 * points at the same places.
 * 
 */
// must be of even and >= 4 length
uint8_t redData[] = {0, 0,
                     36, 0,
                     72, 51,
                     108, 106,
                     144, 195,
                     180, 255,
                     255, 255
                     };
uint8_t greenData[] = {0, 0,
                     36, 4,
                     72, 0,
                     144, 0,
                     180, 46,
                     216, 145,
                     255, 145
                     };
uint8_t blueData[] = {0, 0,
                     36, 77,
                     72, 131,
                     108, 189,
                     144, 233,
                     180, 183,
                     255, 183
                     };

// good old lerp to find a point on the line between two values
uint8_t lerp(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t t) {
  float normalT = (t - x1) / (float)(x2 - x1);
  float inversT = 1 - normalT;
  // float lerp = (inversT * y1) + (normalT * y2);
  float lerp = y1 + (normalT * (y2 - y1));
  return (uint8_t) lerp;
}

/*
 * Find the value in "table" that corresponds to "position". Basically returns f(x) where "table" holds
 * the points that defines the function f (and "position" is x).
 */
uint8_t gradient(uint8_t position, uint8_t table[]) {
    uint8_t index = 2;
    while(position > table[index]) {
        index += 2;
    }
    uint8_t x1 = table[index-2];
    uint8_t y1 = table[index-1];
    uint8_t x2 = table[index];
    uint8_t y2 = table[index+1];
    return lerp(x1, y1, x2, y2, position);
}

//
// main program
//

void setup() {
  pixels.begin();
  pixels.clear();
}

boolean index_in_hover_range() {
  return (index >= HOVER_POS_MIN) && (index <= HOVER_POS_MAX);
}

void loop() {
  delay(SPEED);
  pixels.setPixelColor(0, pixels.Color(
    gradient(index, redData), 
    gradient(index, greenData), 
    gradient(index, blueData)
  ));
  pixels.show();


  if (index_in_hover_range()) {
    index += random(0-HOVER_STR_MAX, HOVER_STR_MAX+1);
    index = constrain(index, HOVER_POS_MIN, HOVER_POS_MAX);
  }
}
