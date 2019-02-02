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

char *type_string(enum Job_Type type){
    switch (type){
        case Arrival:
            return "Arrival";
        case Continue:
            return "Continue";
        case CPU_Begin:
            return "CPU_Begin";
        case CPU_Finish:
            return "CPU_Finish";
        case Disk1_Arrival:
            return "Disk1_Arrival";
        case Disk1_Begin:
            return "Disk1_Begin";
        case Disk1_Finish:
            return "Disk1_Finish";
        case Disk2_Arrival:
            return "Disk2_Arrival";
        case Disk2_Begin:
            return "Disk2_Begin";
        case Disk2_Finish:
            return "Disk2_Finish";
        case Exit:
            return "Exit";
        case SIM_END:
            return "Simulation End";
    }
};