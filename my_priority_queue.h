//
// Created by Adam Berck on 2019-01-30.
//

#ifndef PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_PRORITY_QUEUE_H
#define PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_PRORITY_QUEUE_H


#include "job.h"

#define LENGTH 2000

struct my_priority_queue{
    int size;
    struct job list[LENGTH];
};


struct my_priority_queue create_p_queue();


void add_pq(struct my_priority_queue *queue, struct job jb);

bool is_empty_pq(struct my_priority_queue *queue);

struct job remove_pq(struct my_priority_queue* queue);


#endif //PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_PRORITY_QUEUE_H