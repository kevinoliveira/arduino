// CONSTANTS
int LDR_PIN = 13;

// DATA
int sensorValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    sensorValue = analogRead(LDR_PIN);
    Serial.println(sensorValue);

    delay(100);
}