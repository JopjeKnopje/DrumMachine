#include <MozziGuts.h>
#include <Oscil.h>               // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h> // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw(SAW2048_DATA);

const char INPUT_PIN = 18; // set the input for the knob to analog pin 0

// to convey the volume level from updateControl() to updateAudio()
byte volume;

float freq = 440.0f;
float mod = 0.0f;

void setup()
{
    // Serial.begin(9600); // for Teensy 3.1, beware printout can cause glitches
    Serial.begin(115200); // set up the Serial output so we can look at the piezo values // set up the Serial output so we can look at the input values
    aSin.setFreq(440);
    aSaw.setFreq(440);
    startMozzi(); // :))
}

void updateControl()
{
    // read the variable resistor for volume
    int sensor_value = mozziAnalogRead(INPUT_PIN); // value is 0-1023

    // map it to an 8 bit range for efficient calculations in updateAudio
    volume = map(sensor_value, 0, 1023, 0, 255);

    // print the value to the Serial monitor for debugging
    Serial.print("volume = ");
    Serial.println((int)volume);
}

int updateAudio()
{
    float x = sin(mod) * 10;
    aSaw.setFreq(freq + x);
    mod += 0.1f;
    return (((int)aSin.next() + ((int)(aSaw.next() / mod))) * volume) >> 8;
}

void loop()
{
    audioHook();
}
