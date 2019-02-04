//
// Created by Adam Berck on 2019-02-03.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <stdio.h>
#include "my_random.h"
#include "my_fifo_queue.h"
#include "my_priority_queue.h"
#include "job.h"


int test_of_pq(){
    struct my_priority_queue test_p_queue = create_p_queue();
    for(int i=0;i<100;i++){
        int rand = my_random(1,5000);
        struct job j = create_job("job",rand,Exit,0);
        add_pq(&test_p_queue,j);
    }
    int max = 0;
    for(int i=0;i<100;i++){
        struct job j = remove_pq(&test_p_queue);
        printf("%d\n",j.time);
        max = max > j.time ? max : j.time ;
        if(max!=j.time){
            printf("p_queue test_of_pq failed");
        }
    }
    return 0;
}

int test_of_my_random(){
    int test=0;
    double p = .20;
    for(int i=0;i<1000000;i++){
        test +=(chance(p) ? 1 :0);
    }
    printf(" Goal: %lf Actual: %lf \n",p,test/1000000.0);
}