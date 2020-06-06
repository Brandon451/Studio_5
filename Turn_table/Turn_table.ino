//------------------------------------------------------------
//Hardware Connection Pins
#define microStep1 3
#define microStep2 4
#define trigger 5
#define dirPin 6
#define pulPin 7
#define ledIndicator 13

#define steps 400
#define stepMultiplier 32
#define accelTime 5
//------------------------------------------------------------
//System Defined Values
#define LEDBlink 100
//------------------------------------------------------------
//User Defined Values
//------------------------------------------------------------
//System Required Variables
bool buttonStatus = LOW;
bool triggerCheck = LOW;
bool ledLevel = HIGH;
unsigned long prevMillis;
unsigned long currMillis;
unsigned int dly = 200;
unsigned long stepCounter = 0;
unsigned int stepMultiplierCounter = 0;
//------------------------------------------------------------

bool checkTrigger(){
    currMillis = micros();
    delay(1);
    if (digitalRead(trigger) && buttonStatus == LOW){
        delay(1);
        buttonStatus = digitalRead(trigger);
    }
    if (buttonStatus){
        if (!ledLevel){
            digitalWrite(ledIndicator, HIGH);
            ledLevel = HIGH;
        }
        if (!digitalRead(trigger)){
            buttonStatus = LOW;
            return true;
        }
    }
    return false;
}

void setup(){
    Serial.begin(115200);

    pinMode(dirPin, OUTPUT);
    pinMode(pulPin, OUTPUT);
    pinMode(microStep1, OUTPUT);
    pinMode(microStep2, OUTPUT);
    pinMode(ledIndicator, OUTPUT);

    pinMode(trigger, INPUT);

    digitalWrite(microStep1, HIGH);
    digitalWrite(microStep2, HIGH);

    stepCounter = 0;
    stepMultiplierCounter = 0;
}

void loop(){
    if (!triggerCheck){
        triggerCheck = checkTrigger();
    }
    stepCounter = 0;
    while (triggerCheck)
    {
        currMillis = millis();
        if(stepCounter < 64000){
            digitalWrite(pulPin, HIGH);
            delayMicroseconds(1);
            digitalWrite(pulPin, LOW);
            delayMicroseconds(dly);
            stepCounter++;
        }
        if(currMillis - prevMillis > accelTime && dly>10 && stepCounter < 12800)
        {
            dly--;
            prevMillis = currMillis;
        }
        if(currMillis - prevMillis > accelTime && dly>10 && stepCounter > 51200)
        {
            dly++;
            prevMillis = currMillis;
        }
        if(stepCounter>64000)
        {
            triggerCheck = LOW;
        }
    }
}
