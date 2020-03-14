// LDR
int LDR_PIN = A0;
int ldr_value = 0;
int min_ldr_value = 1023;
int max_ldr_value = 0;
int ldr_level = 0; // from 0 to 9

void ldr_handler(int raw_ldr_value) {
    ldr_value = raw_ldr_value;
    if (raw_ldr_value >= max_ldr_value) max_ldr_value = raw_ldr_value;
    if (raw_ldr_value <= min_ldr_value) min_ldr_value = raw_ldr_value;

    pretty_log_int("raw: ",raw_ldr_value);
    pretty_log_int("min: ",min_ldr_value);
    pretty_log_int("max: ",max_ldr_value);
    blank_line();
}

// HELPERS
void pretty_log_int(char * str,int value) {
    Serial.print(str);Serial.println(value);
}
void blank_line(){
    Serial.println("");
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    ldr_handler(analogRead(LDR_PIN));
    delay(300);
}