#include <Servo.h>

Servo shifter;

const int GEAR_VALUES[] = {170, 158, 149, 139, 132, 128}; 
const int LED_1 = 7; 
const int LED_2 = 6; 
const int UPSHIFT_BUTTON = 10; 
const int DOWNSHIFT_BUTTON = 11;

unsigned gear; 
unsigned cadence;    // crank revolutions per minute      
unsigned cadence_prev; 

long lastContactWithStroke;  //previous millis
long prevMil;
long cycleTimeout = -1;
bool hasFinishedTimeout = false;

int stroke;

void setup() 
{
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    shifter.attach(10);

    Serial.begin(9600);
}

void loop() 
{
    paceMode();
}


void cadenceMeter() 
{
    stroke = digitalRead(13);
    unsigned long cMil = millis();
  
    if (cycleTimeout >= 0) 
    {
        cycleTimeout -= (cMil - prevMil);
        if (cycleTimeout < 0) 
        {
            cycleTimeout = -1;
            hasFinishedTimeout = true;
        }
    } 
    else if(stroke == 1 && cycleTimeout < 0) 
    {
        if (hasFinishedTimeout == true) 
        { 
            cadence = 60000 / (cMil - lastContactWithStroke);
            hasFinishedTimeout = false;
        } 
        else
        {
            cycleTimeout = 60;
            lastContactWithStroke = cMil; 
        }
    }

    prevMil = cMil;
}

void paceMode() 
{
    cadenceMeter();
  
    digitalWrite(LED_1, HIGH);
    stroke = digitalRead(13);
    if(stroke == 1)
    {
        if(c > 0 && c <= 60)
        {
            gear--;
            cadence = 0;
        }
        else if(c >= 70)
        { 
            gear++;
            cadence = 0;
        }
    }

    if (gear <= 0) gear = 0;
    if (gear >= 5) gear = 5;

    shifter.write(GEAR_VALUES[g]);

    Serial.println(cadence);
    Serial.println(gear);
    Serial.println(stroke);
}

void manualMode() 
{
    digitalWrite(LED_2, HIGH);
}

void changeMode() 
{
    int uS = digitalRead(UPSHIFT_BUTTON); //upshift status
    int dS = digitalRead(DOWNSHIFT_BUTTON); //downshift status
    bool mS = true; //mode status

    if (uS == 1)
    {
        delay(200);
        mS = !mS;
    }

    if (mS == true) paceMode();
    if (mS == false) manualMode;
}
