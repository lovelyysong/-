// 사진은 곧 올릴 예정
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define PIXELS_NUM 5
int trig = 12;
int echo = 8;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS_NUM, PIN, NEO_GRB + NEO_KHZ800);

void Lumos(uint32_t c, uint8_t wait) {
      for (int i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
         delay(wait);
      }
      strip.show();
}
void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  strip.begin();
  strip.show();
}
 

void loop() {
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
 
  unsigned long duration = pulseIn(echo, HIGH);
  float distance = duration / 29.0 / 2.0;
  Serial.print(distance);
  Serial.println("cm");
  strip.begin();
  if(distance<20){
    Lumos(strip.Color(127, 127, 127), 50);
    delay(200);
  }
  else{
    Lumos(strip.Color(0, 0, 0), 50);
    delay(200);
  }
  delay(200);
}

// strip.begin()이랑 strip.clear이 안먹는 이유가 무엇인가 （；へ：）...
