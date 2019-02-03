//
// Created by mandr on 2019-01-30.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "my_fifo_queue.h"
#include "job.h"

struct my_fifo_queue create_fifo_queue(){
    struct my_fifo_queue queue;
    queue.read=0;
    queue.write=0;
    queue.size=0;
    return queue;
}

bool is_empty_fq(struct my_fifo_queue *queue){
    return queue->write==queue->read;
}


void add_fq(struct my_fifo_queue *queue, struct job j){
    if(queue->size+1>=LENGTH){
        printf("FIFO Queue OverFlow\n");
        exit(1);
    }
    queue->buff[queue->write]=j;
    queue->write=(queue->write+1)%LENGTH;
    //char* type = type_string(queue->buff[queue->read].type);
    queue->size++;
}

struct job remove_fq(struct my_fifo_queue *queue){
    struct job j = queue->buff[queue->read];
    queue->read = (queue->read+1)%LENGTH;
    queue->size--;
    return j;
}