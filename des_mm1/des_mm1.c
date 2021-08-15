#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lib\sim.h"
#include "lib\rngs.h"

#define MAX_NUM 10000000

double library_routing(int state_enum);
int event_rounting(int next_event_type);
int time_routing();
void sim_MM1Q();


enum State{ idle, busy };
enum Event{ nul, arr, dep};

int state, event, people_in_record;

double lambda_arr, lambda_dep, main_clock, last_clock, total;
Node * future_list, * current_event;

void init(){
    // waiting_list = NULL;
    future_list = createNode(1, 1.0);
    current_event = NULL;

    state = idle;
    event = nul;    

    main_clock = 0; 
    total = 0.0;
    people_in_record = 0;

    main_clock = 0.0;
    last_clock = 0.0;
    // printf("init complete...");
}

int main(int argc, char *argv[]){
    lambda_arr = atof(argv[1]);
    lambda_dep = atof(argv[2]);
    init();
    sim_MM1Q();
}

void sim_MM1Q(){
    int i = 0;
    double divid_lambda = lambda_arr / (2 * lambda_dep);
    double expected_value = (2 * divid_lambda) / (1 - pow(divid_lambda,2));
    do{
        i = time_routing();
        event_rounting(i); 
        // printf("bruh");   
        // printf("bruh waiting list\n");
        // printLists(waiting_list);
    }while (main_clock <= MAX_NUM);
    printf("lambda 1    lambda_2    理論值  模擬值\n");
    printf("%8lf %8lf %8lf %8lf\n", lambda_arr, lambda_dep
                                  , expected_value
                                  ,total / (double)MAX_NUM); 
    printf("total = %8lf\n", total);                            
}

int time_routing(){
    // printf("start time_routing...");
    current_event = NULL;
	if (future_list == NULL) {
		return 0;
	}else {
		future_list = removeNode(future_list, &current_event);
		main_clock = current_event->clock;
		return current_event->type;
	}
    return 0;
}

int event_rounting(int next_event_type){
    // printf("total = %d\n", total);
    if (next_event_type == nul) {
		return 0;
	}
	else if(next_event_type == arr){

        total += (main_clock - last_clock) * people_in_record;
        people_in_record ++;
        double next_arr_time = main_clock + library_routing(arr);
        future_list = insertNode(future_list, createNode(arr, next_arr_time));
        if (people_in_record == 2 || people_in_record == 1){
            double next_dep_time = main_clock + library_routing(dep);
            future_list = insertNode(future_list, createNode(dep, next_dep_time));
        }
		// if (future_list == NULL)  future_list = createNode(arr, main_clock + library_routing(arr));
		// else future_list = insertNode(future_list, createNode(arr, main_clock + library_routing(arr)));

		// if (waiting_list == NULL)  waiting_list = current_event;
		// else waiting_list = insertNode(waiting_list, current_event);

		// if ( state == idle) {
		// 	state = busy;
		// 	if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
		// 	else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		// }else {
		// 	total += (getCount(waiting_list) - 1) * (int)(main_clock - last_clock);
		// }
	}else {
        total += (main_clock - last_clock) * people_in_record;
        people_in_record --;
        if (people_in_record >= 2){
            double next_dep_time = main_clock + library_routing(dep);
            future_list = insertNode(future_list, createNode(dep, next_dep_time));
        }
		// total += getCount(waiting_list) * (int)(main_clock - last_clock);
		// waiting_list = removeNode(waiting_list, &current_event);
		// if (waiting_list == NULL) {
		// 	state = idle;
		// }else {
		// 	if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
		// 	else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		// }
	}
	last_clock = main_clock;
    // printf("bruh\n");
    return 1;
}

double library_routing(int state_enum){
    double result = 0.0;
    
    if(state_enum == arr) result = exp_test(0, lambda_arr);
    else result = exp_test(1, lambda_dep);

    return result;
}