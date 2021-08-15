#include <stdio.h>
#include <stdlib.h>

#include "lib\sim.h"
#include "lib\rngs.h"

#define MAX_NUM 10000

double library_routing(int state_enum);
int event_rounting(int next_event_type);
int time_routing();
int roll_the_yes_no_dice();
void sim_MM1Q();


enum State{ idle, busy };
enum Event{ nul, arr, dep, pat};

int state, event, unpatient_client_total, total;
double lambda_arr, lambda_dep, lambda_pat, main_clock, last_clock;
Node * waiting_list, * future_list, * current_event;

void init(){
    waiting_list = NULL;
    future_list = createNode(1, 0.0);
    current_event = NULL;

    state = idle;
    event = nul;    

    unpatient_client_total = 0;

    main_clock = 0.0;
    last_clock = 0.0;
    total = 0;
    // printf("init complete...");
}

int main(int argc, char *argv[]){
    lambda_arr = atof(argv[1]);
    lambda_dep = atof(argv[2]);
    lambda_pat = atof(argv[3]);

    // double temp= library_routing(lambda_pat);
    // printf("%f", temp);

    init();
    sim_MM1Q();
}

void sim_MM1Q(){
    int i = 0;
    do{
        i = time_routing();
        event_rounting(i); 
        
        // printf("main clock = %lf\n", main_clock);
        // printf("\nbruh future list\n");
        // printLists(future_list);  
        // printf("bruh waiting list\n");
        // printLists(waiting_list); 
        
    }while (main_clock < MAX_NUM);
    printf("lambda 1    lambda_2      lambda_3      theroy           sim_value         leaved_client  total\n");
    printf("%8lf    %8lf      %8lf      %8lf        %8lf     %8d            %8d\n", lambda_arr, lambda_dep, lambda_pat
                                  , lambda_arr / (lambda_dep-lambda_arr)
                                  ,(double)total / (double)MAX_NUM
                                  ,unpatient_client_total
                                  ,total); 
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
    // printf("now the event type is %d and state is %d\n", next_event_type, state);
    // printf("total = %d \n", total);
    if (next_event_type == nul) {
		if (future_list == NULL)  future_list = createNode(arr, main_clock + library_routing(arr));
		else future_list = insertNode(future_list, createNode(arr, main_clock + library_routing(arr)));
        return 1;
	}
	else if(next_event_type == arr){
		if (future_list == NULL)  future_list = createNode(arr, main_clock + library_routing(arr));
		else future_list = insertNode(future_list, createNode(arr, main_clock + library_routing(arr)));

		if (waiting_list == NULL)  waiting_list = current_event;
		else waiting_list = insertNode(waiting_list, current_event);

        if(roll_the_yes_no_dice() == 1 && state == busy){
            future_list = insertNode(future_list, createNode(pat, main_clock + library_routing(pat)));
        }
        if ( state == idle) {
            state = busy;
            if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
            else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
        }else {
            // total += (getCount(waiting_list) - 1) * (main_clock - last_clock);
        }
	}else if(next_event_type == dep){
		// total += getCount(waiting_list) * (main_clock - last_clock);
        if (waiting_list != NULL) waiting_list = removeNode(waiting_list, &current_event);
		if (waiting_list == NULL) {
			state = idle;
		}else {
			if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
			else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		}
	}else{
        unpatient_client_total += 1;
        // total += (getCount(waiting_list) - 1) * (main_clock - last_clock);
        if (waiting_list != NULL) waiting_list = removeNode(waiting_list, &current_event);
		if (waiting_list == NULL) {
			state = idle;
		}
        else {
			if (future_list == NULL)  future_list = createNode(dep, main_clock + library_routing(dep));
			else future_list = insertNode(future_list, createNode(dep, main_clock + library_routing(dep)));
		}
    }
    total += getCount(waiting_list) * (int)(main_clock - last_clock);
	last_clock = main_clock;
    // printf("bruh\n");
    return 1;
}

int roll_the_yes_no_dice(){
    SelectStream(23333);
    if(Random()>Random()) return 1;
    else return 0;
}

double library_routing(int state_enum){
    double result = 0.0;
    
    if(state_enum == arr) result = exp_test(0, lambda_arr);
    else if(state_enum == dep) result = exp_test(1, lambda_dep);
    else result = exp_test(2, lambda_pat); 

    return result/0.05;
}