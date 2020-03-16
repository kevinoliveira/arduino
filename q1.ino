const byte LED_1_PIN = 7;
const byte LED_2_PIN = 6;
const byte PUSH_BUTTON_PIN = 3;
volatile byte state = 0; // 0-3


void setup() {
    Serial.begin(9600);
    pinMode(LED_1_PIN,OUTPUT);
    pinMode(LED_2_PIN,OUTPUT);
    pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), push_button_interrupt, RISING);
}

void push_button_interrupt(){
    if(state == 3) state = 0;
    else state++; 
    Serial.println(state);
}

void control_led(bool led_1_state,bool led_2_state ){
    digitalWrite(LED_1_PIN,led_1_state);
    digitalWrite(LED_2_PIN,led_2_state);
}

void loop() {
    switch(state) {
        case 0 : control_led(LOW,LOW);   break;
        case 1 : control_led(HIGH,LOW);  break;
        case 2 : control_led(LOW,HIGH);  break;
        case 3 : control_led(HIGH,HIGH); break;
    }   
}