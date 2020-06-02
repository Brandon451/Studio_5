//------------------------------------------------------------
//Hardware Connection Pins
#define microStep 4
#define trigger 5
#define dirPin 6
#define pulPin 7
#define ledIndicator 13

#define steps 400
#define stepMultiplier 32
#define accelTime 20
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
unsigned int dly = 300;
unsigned int stepCounter = 0;
unsigned int stepMultiplierCounter = 0;
//------------------------------------------------------------

bool checkTrigger(){
    currMillis = millis();
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
    pinMode(microStep, OUTPUT);
    pinMode(ledIndicator, OUTPUT);

    pinMode(trigger, INPUT);

    stepCounter = 0;
    stepMultiplierCounter = 0;
}

void loop(){
    if (!triggerCheck){
        triggerCheck = checkTrigger();
    }
    if (triggerCheck){
        currMillis = millis();
        Serial.println("TRIGGERED!!!!");
        if (stepCounter < steps){
            if (stepMultiplierCounter < stepMultiplier){
                digitalWrite(pulPin, HIGH);
                delay(1);
                digitalWrite(pulPin, LOW);
                delay(dly);
                stepMultiplierCounter++;
            }
            stepCounter++;
        }
        if (currMillis - prevMillis > accelTime && dly > 5){
            dly--;
            prevMillis = currMillis;
        }
    }
}