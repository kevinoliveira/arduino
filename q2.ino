const byte LED_1_PIN = 7;
const byte LDR_PIN = A0;
int interval = 1023;
bool led_state = false;
unsigned long previous_time;

void setup() {
    pinMode(LED_1_PIN,OUTPUT);
}

void led_handler(){
    unsigned long current_time = millis();

    if(current_time - previous_time >= interval){
        led_state = !led_state;
        previous_time = current_time;
        digitalWrite(LED_1_PIN, led_state);
    }
}

void loop() {
    int raw_ldr_value = analogRead(LDR_PIN);
    interval = 1023 - raw_ldr_value;    
    led_handler();
}