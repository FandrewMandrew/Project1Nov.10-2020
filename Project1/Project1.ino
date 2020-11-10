#include <Wire.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include "pitches.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int min_value = 10, sec_value = 00;
signed short minutes = min_value, seconds = sec_value;
char timeline[16];
int done = 0;

const int button_pin = 2;
int button_state = 0;
int last_buttonState = HIGH;
bool bPress = false;

int speakerpin = 9;
// notes in the intro:
int Intro_melody[] = {
  NOTE_E4, NOTE_E4, REST, NOTE_E4, 
  REST, NOTE_C4, NOTE_E4, REST,
  NOTE_G4, REST, REST, NOTE_G3, REST
};

int Intro_Notes[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 2, 4, 2, 2
};

const int num_elements_in_Intro = sizeof(Intro_Notes)/sizeof(Intro_Notes[0]);

// notes in the melody:
int Mario_melody[] = {
  NOTE_C4, REST, REST, NOTE_G3,
  REST, NOTE_E3, REST,
  REST, NOTE_A3, REST, NOTE_B3,   
  REST, NOTE_AS3, NOTE_A3, REST,
  
  NOTE_G3, NOTE_E4, NOTE_G4,
  NOTE_A4, REST, NOTE_F4, NOTE_G4, 
  REST, NOTE_E4, REST, NOTE_C4, 
  NOTE_D4, NOTE_B3, REST
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int Mario_Notes[] = {
  4, 4, 4, 4,
  2, 4, 4,
  4, 4, 4, 4,  
  4, 4, 4, 4,
  
  4, 2, 4,
  4, 4, 4, 4,
  4, 4, 4, 4, 
  4, 4, 2
  };
  
  const int num_elements_in_arr = sizeof(Mario_Notes)/sizeof(Mario_Notes[0]);

void check_press();
void timer();
void buzzer();

void setup() 
{
  Serial.begin(9600);
  
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(9, OUTPUT);
  
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight
}

void loop()
{
  check_press();

  if (bPress)
  {
    bPress = false;

    timer();    
  }
  
}

void check_press()
{
  button_state = digitalRead(button_pin);

  if (button_state != last_buttonState)
  {
    bPress = true; 
    Serial.println(button_state);
  }

  delay(50);
}

void timer()
{
  minutes = min_value;
  seconds = sec_value;

  lcd.clear();
  lcd.print("Timer: ");
  
  while (done == 0)
  {
    lcd.setCursor(0, 1);
    sprintf(timeline,"%0.2d mins %0.2d secs", minutes, seconds);
    lcd.print(timeline);
    
    delay(1000);
    seconds--;

    if (seconds == 00)
    {
      if (minutes == 0)
      {
        done = 1;
        lcd.clear();
        lcd.print("Take a break!");
        buzzer();
      }
      else
      {
        seconds = 59;     
        minutes--;
      }
    }
  }
  done = 0;
}

void buzzer()
{
  
  for (int thisNote = 0; thisNote < num_elements_in_Intro; thisNote++) 
  {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500/Intro_Notes[thisNote];
    tone(speakerpin, Intro_melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerpin);
  } 

  for (int i = 0; i < 2; i++)
  {
    for (int thisNote = 0; thisNote < num_elements_in_arr; thisNote++) 
    {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 500/Mario_Notes[thisNote];
    tone(speakerpin, Mario_melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(speakerpin);  
    }
  }  
}
