//
// Created by mandr on 2019-01-30.
//

#include <stdbool.h>
#include "my_fifo_queue.h"
#include "job.h"

struct my_fifo_queue create_fifo_queue(){
    struct my_fifo_queue queue;
    queue.read=0;
    queue.write=0;
    queue.length=100;
    return queue;
}

bool is_empty_fq(struct my_fifo_queue *queue){
    return queue->write==queue->read;
}


void add_fq(struct my_fifo_queue *queue, struct job j){
    queue->buff[queue->write]=j;
    queue->write=queue->write+1%queue->length;
}

struct job remove_fq(struct my_fifo_queue *queue){
    struct job j = queue->buff[queue->read];
    queue->read=queue->read+1%queue->length;
    return j;
}

int size_fq(struct my_fifo_queue queue){
    return queue.write-queue.read;
}