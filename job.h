//
// Created by mandr on 2019-01-30.
//

#ifndef PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H
#define PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H


enum Job_Type{
    Arrival,Continue,
    CPU_Begin,CPU_Finish,
    Disk1_Arrival,Disk1_Begin,Disk1_Finish,
    Disk2_Arrival,Disk2_Begin,Disk2_Finish,
    Exit,
    SIM_END
};

struct job{
    int time;
    enum Job_Type type;
    char name[15];
    int cpu_arrive;
    int cpu_complete;
    int disk1_arrive;
    int disk1_complete;
    int disk2_arrive;
    int disk2_complete;

};

char *type_string(enum Job_Type type);

struct job create_job(char name[],int time,enum Job_Type type);


#endif //PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H