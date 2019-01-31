//
// Created by mandr on 2019-01-30.
//

#include <stdbool.h>
#include "my_priority_queue.h"
#include "job.h"

struct my_priority_queue create_p_queue(){
    struct my_priority_queue queue;
    queue.size=0;
    return queue;
}


void add_pq(struct my_priority_queue *qp, struct job jb){
    int i=0;
    while(jb.time>qp->list[i].time && i<qp->size){
        i++;
    }
    for(int j=(*qp).size;j>=i;j--) {
        qp->list[j + 1] = qp->list[j];
    }
    qp->list[i]=jb;
    qp->size++;
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