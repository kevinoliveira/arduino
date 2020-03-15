// LDR
const int LDR_PIN = A0;
int min_ldr_value = 1023;
int max_ldr_value = 0;
int ldr_level = 0; // from 0 to 9
const int NUMBER_OF_STEPS = 10; // 0-1, 1-2, 2-3, 3-4, 4-5, 5-6, 6-7, 7-8, 8-9, 9-10 

// 74HC595
const int LATCH_PIN = 6; // ST_CP
const int CLOCK_PIN = 5; // SH_CP
const int DATA_PIN  = 7; // DS

const int A = 2;
const int B = 4;
const int C = 8;
const int D = 16;
const int E = 32;
const int F = 64;
const int G = 128;
const int DP = 1;

const int LEVEL_DISPLAY [22] = {

    // MANUAL NUMBERS
    A+B+C+D+E+F,        // 0 - manual
    B+C,                // 1 - manual
    A+B+G+E+D,          // 2 - manual
    A+B+G+C+D,          // 3 - manual
    F+G+B+C,            // 4 - manual
    A+F+G+C+D,          // 5 - manual
    A+C+D+E+F+G,        // 6 - manual
    A+B+C,              // 7 - manual
    A+B+C+D+E+F+G,      // 8 - manual
    A+B+C+D+F+G,        // 9 - manual

    // AUTOMATIC NUMBERS
    A+B+C+D+E+F+DP,     // 0 - automaico
    B+C+DP,             // 1 - automaico
    A+B+G+E+D+DP,       // 2 - automaico
    A+B+G+C+D+DP,       // 3 - automaico
    F+G+B+C+DP,         // 4 - automaico
    A+F+G+C+D+DP,       // 5 - automaico
    A+C+D+E+F+G+DP,     // 6 - automaico
    A+B+C+DP,           // 7 - automaico
    A+B+C+D+E+F+G+DP,   // 8 - automaico
    A+B+C+D+F+G+DP,     // 9 - automaico

    // CALIBRATION DISPLAY
    A+F+B+G,            // CALIBRATE MAX C_MAX
    G+E+C+D,         // CALIBRATE MIN C_MIN
};

const int C_MAX = 20;
const int C_MIN = 21;

// ALARM 
const int LED_PIN = A1;
const int DESTIVATE_PUSH_BUTTON_PIN = 2;
const int INTERVAL = 100;
bool led_state = false;
bool is_alarm_activated = false;
unsigned long previous_time;


// CALIBRATION
const int CALIBRATION_PIN = 3;
volatile int calibration_mode = 0; // 0 - automatic, 1- calibrate max, 2 - calibrate min, 3 - operate with manual calibration
const byte AUTO_MODE = 0;
const byte CAL_MAX_MODE = 1;
const byte CAL_MIN_MODE = 2;
const byte MANUAL_MODE = 3;




void display_number(int number){
    if(number < 0 || number > 9) return;
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, LEVEL_DISPLAY[ calibration_mode == AUTO_MODE? number + NUMBER_OF_STEPS : number ]);  
    digitalWrite(LATCH_PIN, HIGH);
}

void display_calibration(int type) {
    if(type != C_MAX && type != C_MIN) return;
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, LEVEL_DISPLAY[type]);  
    digitalWrite(LATCH_PIN, HIGH);
}



void calibrate_max(){
    display_calibration(C_MAX);
    int raw_ldr_value = analogRead(LDR_PIN);
    if (raw_ldr_value >= max_ldr_value ) max_ldr_value = raw_ldr_value;
}

void calibrate_min(){
    display_calibration(C_MIN);
    int raw_ldr_value = analogRead(LDR_PIN);
    if (raw_ldr_value <= min_ldr_value) min_ldr_value = raw_ldr_value;

}

void ldr_handler() {
    int raw_ldr_value = analogRead(LDR_PIN);
    if (raw_ldr_value >= max_ldr_value && calibration_mode == AUTO_MODE) max_ldr_value = raw_ldr_value;
    if (raw_ldr_value <= min_ldr_value && calibration_mode == AUTO_MODE) min_ldr_value = raw_ldr_value;

    int level = value_to_ldr_level(raw_ldr_value);
    if(level >= 8) active_alarm();

    pretty_log_int("raw: ",raw_ldr_value);
    pretty_log_int("min: ",min_ldr_value);
    pretty_log_int("max: ",max_ldr_value);
    pretty_log_int("lvl: ",level);
    blank_line();
    display_number(level);
}

void led_handler(){
    unsigned long current_time = millis();

    if(current_time - previous_time >= INTERVAL && is_alarm_activated){
        led_state = !led_state;
        previous_time = current_time;
        digitalWrite(LED_PIN, led_state);
    }
}


void active_alarm(){
    is_alarm_activated = true;
}
void desativate_alarm(){
    is_alarm_activated = false;
    led_state = false;
    digitalWrite(LED_PIN, led_state);
}


int value_to_ldr_level(int value) {
    int level = 0;
    for(;level<NUMBER_OF_STEPS-1;level++){
        if(value <= step_celing(level, min_ldr_value, max_ldr_value, NUMBER_OF_STEPS)) return level;
    }
    return level;
}

int step_floor(
    int step,
    int min_value,
    int max_value,
    int NUMBER_OF_STEPS
    ) {
    return  min_value + ( ( (max_value - min_value) / NUMBER_OF_STEPS ) * step );
}

int step_celing(
    int step,
    int min_value,
    int max_value,
    int NUMBER_OF_STEPS
    ) {
    return step_floor(step+1,min_value,max_value,NUMBER_OF_STEPS); 
}

void calibrate_interruption() {
    if(calibration_mode == 3) calibration_mode = 0;
    else calibration_mode++; 
    switch (calibration_mode) {
        case CAL_MAX_MODE:
            max_ldr_value = 0;
            break;
        case CAL_MIN_MODE:
            min_ldr_value = 1023;
            break;
    }
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
    pinMode(LATCH_PIN,OUTPUT);
    pinMode(CLOCK_PIN,OUTPUT);
    pinMode(DATA_PIN,OUTPUT);
    pinMode(LED_PIN,OUTPUT);
    pinMode(DESTIVATE_PUSH_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CALIBRATION_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DESTIVATE_PUSH_BUTTON_PIN), desativate_alarm, CHANGE);
    attachInterrupt(digitalPinToInterrupt(CALIBRATION_PIN), calibrate_interruption, FALLING);
}

void loop() {
    switch(calibration_mode) {
        case AUTO_MODE : 
            ldr_handler();
            led_handler();
            break;
        case CAL_MAX_MODE : 
            calibrate_max();
            break;
        case CAL_MIN_MODE : 
            calibrate_min();
            break;
        case MANUAL_MODE:
            ldr_handler();
            led_handler();
            break;

    }   
}