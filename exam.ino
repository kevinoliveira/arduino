// CONSTANTS
int LDR_PIN = A0;

// DATA
int ldr_value = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    ldr_value = analogRead(LDR_PIN);
    Serial.println(ldr_value);
}