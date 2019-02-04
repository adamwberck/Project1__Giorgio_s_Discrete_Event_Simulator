//
// Created by Adam Berck on 2019-01-30.
//

#ifndef PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_FIFO_QUEUE_H
#define PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_FIFO_QUEUE_H


#include "job.h"

#define LENGTH 500

struct my_fifo_queue {
    struct job buff[LENGTH];
    int read;
    int write;
    int size;
};

bool is_empty_fq(struct my_fifo_queue *queue);

struct my_fifo_queue create_fifo_queue();

void add_fq(struct my_fifo_queue *queue, struct job j1);


struct job remove_fq(struct my_fifo_queue *queue);


#endif //PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_MY_FIFO_QUEUE_H