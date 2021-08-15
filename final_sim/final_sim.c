#include <stdio.h>
#include <stdlib.h>

#include "lib\sim.h"
#include "lib\rngs.h"

#define MAX_NUM 67108864

double library_routing(int state_enum);
int event_rounting(int next_event_type);
int time_routing();
void sim_MM1Q();


enum State{ idle, busy };
enum Event{ nul, arr, dep};

int state, event, total;

double lambda_arr, lambda_dep, main_clock, last_clock;
Node * waiting_list, * future_list, * current_event;

void init(){
    waiting_list = NULL;
    future_list = createNode(1, 1.0);
    current_event = NULL;

    state = idle;
    event = nul;    

    main_clock = 0; 
    total = 0;

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
    int event_routing_state;
    int i = 0;
    do{
        if (main_clock > MAX_NUM) {
			break;
		}
        i = time_routing();
        event_routing_state = event_rounting(i); 
        // printf("bruh");   
    }while (event_routing_state);
    printf("lambda 1    lambda_2    理論值  模擬值\n");
    printf("%lf    %lf  %lf %lf\n", lambda_arr, lambda_dep, lambda_arr / (lambda_dep-lambda_arr), (double)total / (double)MAX_NUM); 
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
    if (next_event_type == nul) {
		return 0;
	}
	else if(next_event_type == arr){
		if (future_list == NULL)  future_list = createNode(arr, main_clock + library_routing(arr));
		else future_list = insertNode(future_list, createNode(arr, main_clock + library_routing(arr)));

		if (waiting_list == NULL)  waiting_list = current_event;
		else waiting_list = insertNode(waiting_list, current_event);

		if ( state == idle) {
			state = busy;
			if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
			else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		}else {
			total += (getCount(waiting_list) - 1) * (int)(main_clock - last_clock);
		}
	}else {
		total += getCount(waiting_list) * (int)(main_clock - last_clock);
		waiting_list = removeNode(waiting_list, &current_event);
		if (waiting_list == NULL) {
			state = idle;
		}else {
			if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
			else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		}
	}
	last_clock = main_clock;
    // printf("bruh\n");
    return 1;
}

double library_routing(int state_enum){
    double result = 0.0;
    while((int)(result / 0.05) == 0){
        // printf("stock in library_routing...\n");
        if(state_enum == arr) result = exp_test(0, lambda_arr);
        else result = exp_test(1, lambda_dep);
    }
    // printf("%lf\n", result / 0.05);
    return (result / 0.05);
}