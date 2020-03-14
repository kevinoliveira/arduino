#include <iostream>


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

int value_to_ldr_level(int value) {
    int level = 0;
    for(;level<9;level++){
        if(value <= step_celing(level, 312, 1020, 10)) return level;
    }
    return level;
}


using namespace std;

int main()
{

    for(int i = 0;i < 10;i++){
        printf(
            "i:%d f: %d c:%d\n",
            i,
            step_floor(i,312,1020,10),
            step_celing(i,312,1020,10)
        );
    }

    for(int j = 312;j < 1020;j = j+10) printf("raw:%d level:%d\n",j, value_to_ldr_level(j));



    cout<<"Hello World";

    return 0;
}
