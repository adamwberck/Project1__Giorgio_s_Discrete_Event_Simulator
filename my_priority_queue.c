//
// Created by mandr on 2019-01-30.
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "my_priority_queue.h"
#include "job.h"

struct my_priority_queue create_p_queue(){
    struct my_priority_queue queue;
    queue.size=0;
    return queue;
}


void add_pq(struct my_priority_queue *queue, struct job jb){
    //check full
    if(queue->size+1>LENGTH-1){
        printf("Priority Queue OverFlow\n");
        exit(-1);
    }

    int i=0;
    while(jb.time>queue->list[i].time && i<queue->size){
        i++;
    }
    for(int j=(*queue).size;j>=i;j--) {
        queue->list[j + 1] = queue->list[j];
    }
    queue->list[i]=jb;
    queue->size++;
}

struct job remove_pq(struct my_priority_queue *queue){
    struct job j = queue->list[0];
    for(int i=0;i<queue->size-1;i++){
        queue->list[i]=queue->list[i+1];
    }
    queue->size--;
    return j;
}

bool is_empty_pq(struct my_priority_queue *queue){
    return queue->size==0;
}