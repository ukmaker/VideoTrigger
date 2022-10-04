#include <EncoderButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define INTER_FRAME_LINES 11

// Actually 312.5
#define PAL_LINES 313
#define PAL_VISIBLE_LINES 288

// Actually 262.5
#define NTSC_LINES 263
#define NTSC_VISIBLE_LINES 238
#define ARM 7
#define ARMN 6
#define ENCA 11
#define ENCB 12
#define ENCC 10
#define ARMND (1<<7)
#define ARMD (1<<6)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


volatile int line = 0;
volatile int lines = 0;
volatile int trigger = 24;
volatile int maxlines = 0;
volatile bool falling = false;
volatile bool armed = false;
volatile uint8_t arm = ARMD;
volatile uint8_t frames = 0;
int lastTrigger = 0;
int lastPolarity = false;


EncoderButton eb1(ENCA, ENCB, ENCC);

void onEb1Clicked(EncoderButton& eb) {
  falling = !falling;
  detachInterrupt(digitalPinToInterrupt(2));
  if (falling) {
    attachInterrupt(digitalPinToInterrupt(2), hsync, FALLING);
    arm = ARMD;
  } else {
    attachInterrupt(digitalPinToInterrupt(2), hsync, RISING);
    arm = ARMND;
  }
  maxlines = 0;
}

void onEb1Encoder(EncoderButton& eb) {
  long pos = eb.position();
  if (pos > maxlines) pos = 1;
  if (pos <= 0) pos = maxlines;
  trigger = pos;
}

void onButton1LongPress(EncoderButton& eb) {
  trigger = 1;
}

void setup() {

  pinMode(ARM, OUTPUT);
  pinMode(ARMN, OUTPUT);
  digitalWrite(ARM, HIGH);
  digitalWrite(ARMN, LOW);
  // setup the encoder and screen

  //Link the event(s) to your function
  eb1.setClickHandler(onEb1Clicked);
  eb1.setEncoderHandler(onEb1Encoder);
  eb1.setLongPressHandler(onButton1LongPress, false);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(("Video"));
  display.println(("Trigger"));
  display.display();
  delay(1000);
  display.setCursor(10, 0);
  eb1.resetPosition(trigger);
  attachInterrupt(digitalPinToInterrupt(3), vsync, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), hsync, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  // reset the line count every now and then in case we got a glitch
  if(frames == 0) {
    maxlines = 0;
  }
  eb1.update();
  if (lastTrigger != trigger || lastPolarity != falling) {
    lastTrigger = trigger;
    lastPolarity = falling;

    eb1.resetPosition(trigger);

    display.clearDisplay();
    display.setCursor(0, 0);

    if (!falling) {
      display.print("\\_");
    } else {
      display.print("_/");
    }
    display.print(trigger, DEC);
    display.print("/");
    display.print(maxlines);
    display.display();
  }
}


void hsync() {

  if (line == trigger) {
    PORTD |= arm;
  } else {
    PORTD &= 0x3f;
  }

  lines++;
  line++;
  if (line > maxlines) line = 1;
}

void vsync() {
  line = maxlines - INTER_FRAME_LINES;
  if (lines > maxlines) maxlines = lines;
  lines = 0;
  frames++;
}
