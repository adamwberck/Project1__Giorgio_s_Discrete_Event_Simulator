//
// Created by mandr on 2019-01-30.
//

#include <memory.h>
#include "job.h"

struct job create_job(char name[],int time,enum Job_Type type){
    struct job j1;
    j1.time = time;
    j1.type = type;
    stpncpy(j1.name,name,15);
    return j1;
}