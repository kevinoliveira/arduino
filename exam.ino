// LDR
int LDR_PIN = A0;
int ldr_value = 0;
int min_ldr_value = 1023;
int max_ldr_value = 0;
int ldr_level = 0; // from 0 to 9
int number_of_steps = 10; // 0-1, 1-2, 2-3, 3-4, 4-5, 5-6, 6-7, 7-8, 8-9, 9-10 

// 74HC595
int LATCH_PIN = 6; // ST_CP
int CLOCK_PIN = 5; // SH_CP
int DATA_PIN  = 7; // DS

int A = 2;
int B = 4;
int C = 8;
int D = 16;
int E = 32;
int F = 64;
int G = 128;
int DP = 1;

int LEVEL_DISPLAY [12] = {
    A+B+C+D+E+F,    // 0
    B+C,            // 1
    A+B+G+E+D,      // 2
    A+B+G+C+D,      // 3
    F+G+B+C,        // 4
    A+F+G+C+D,      // 5
    A+C+D+E+F+G,    // 6
    A+B+C,          // 7
    A+B+C+D+E+F+G,  // 8
    A+B+C+D+F+G,    // 9
    A+F+E+D,        // CALIBRATE MAX
    A+F+E+D+DP,     // CALIBRATE MIN
};


void display_number(int number){
    if(number < 0 || number > 11) return;
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, LEVEL_DISPLAY[number]);  
    digitalWrite(LATCH_PIN, HIGH);
}

void ldr_handler(int raw_ldr_value) {
    ldr_value = raw_ldr_value;
    if (raw_ldr_value >= max_ldr_value) max_ldr_value = raw_ldr_value;
    if (raw_ldr_value <= min_ldr_value) min_ldr_value = raw_ldr_value;

    pretty_log_int("raw: ",raw_ldr_value);
    pretty_log_int("min: ",min_ldr_value);
    pretty_log_int("max: ",max_ldr_value);
    pretty_log_int("lvl: ",value_to_ldr_level(raw_ldr_value));
    blank_line();
    display_number(value_to_ldr_level(raw_ldr_value));
}


int value_to_ldr_level(int value) {
    int level = 0;
    for(;level<number_of_steps-1;level++){
        if(value <= step_celing(level, min_ldr_value, max_ldr_value, number_of_steps)) return level;
    }
    return level;
}

int step_floor(
    int step,
    int min_value,
    int max_value,
    int number_of_steps
    ) {
    return  min_value + ( ( (max_value - min_value) / number_of_steps ) * step );
}

int step_celing(
    int step,
    int min_value,
    int max_value,
    int number_of_steps
    ) {
    return step_floor(step+1,min_value,max_value,number_of_steps); 
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
}

void loop() {
    ldr_handler(analogRead(LDR_PIN));
    delay(300);
}