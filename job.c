//
// Created by Adam Berck on 2019-01-30.
//

#include <memory.h>
#include "job.h"

struct job create_job(char name[],int time,enum Job_Type type,int arrive){
    struct job j;
    j.time = time;
    j.type = type;
    j.arrive = arrive;
    //j.cpu_arrive = cpu_arrive;
    //j.disk1_arrive = disk1_arrive;
    //j.disk2_arrive = disk2_arrive;
    stpncpy(j.name,name,15);
    return j;
}

char *type_string(enum Job_Type type){
    switch (type){
        case CPU_Arrival:
            return "CPU_Arrival";
        case CPU_Continue:
            return "CPU_Continue";
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