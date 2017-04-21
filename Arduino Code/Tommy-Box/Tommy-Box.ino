#include <wavTrigger.h>
#include <Ultrasonic.h>
#include <AltSoftSerial.h>

//    Uno           WAV Trigger
//    ===           ===========
//    GND  <------> GND
//    Pin9 <------> RX
//    Pin8 <------> TX

Ultrasonic ultrasonic1(A1, A2); // (Trig PIN,Echo PIN)
const int trigerStart = 60;
int ultra1 = -1;
unsigned long previousMillis = 0;
const long rest = 60000;

wavTrigger wTrig;

int now;
const int startTrack = 2;
const int endTrack = 4;

void setup() {

    // If the Arduino is powering the WAV Trigger, we should wait for the WAV
    //  Trigger to finish reset before trying to send commands.
    delay(1000);

    // WAV Trigger startup at 57600
    wTrig.start();
    delay(10);

    // Send a stop-all command and reset the sample-rate offset, in case we have
    //  reset while the WAV Trigger was already playing.
    wTrig.stopAllTracks();
    wTrig.samplerateOffset(0);

    // Enable track reporting from the WAV Trigger
    wTrig.setReporting(true);

    pinMode(3, INPUT_PULLUP);

    pinMode(A0, OUTPUT);
    pinMode(A3, OUTPUT);

    digitalWrite(A0, HIGH);
    digitalWrite(A3, LOW);

    previousMillis = rest;
    now = startTrack;
    Serial.begin(9600);
}

void loop() {
    ultra1 = ultrasonic1.distanceRead(INC);
            Serial.println(ultra1);
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= rest) {
        Serial.println("Can Trigger");
        if ((ultra1 < trigerStart) && (ultra1 > 1)) {
            if (!wTrig.isTrackPlaying(1) && !wTrig.isTrackPlaying(2) && !wTrig.isTrackPlaying(3) && !wTrig.isTrackPlaying(4) && !wTrig.isTrackPlaying(5)) {
                Serial.println("Playing Now!");
                wTrig.trackPlaySolo(1);
                previousMillis = currentMillis;
            }
        }
    }
    else
    {
        Serial.println("Can NOT Trigger");
        Serial.println(currentMillis - previousMillis);
    }


    wTrig.update();

    if (digitalRead(3) == LOW) {
            wTrig.trackPlaySolo(now);
            if (now <= endTrack)
            {
              now++;
            }
            else
            {
              now = startTrack;
            }
    }

    delay(100);
}
