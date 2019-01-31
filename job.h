//
// Created by mandr on 2019-01-30.
//

#ifndef PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H
#define PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H


enum Job_Type{
    Arrival,
    CPU_Begin,CPU_Finish,
    Disk1_Arrival,Disk1_Finish,
    Disk2_Arrival,Disk2_Finish,
    Exit,
    SIM_END
};

struct job{
    int time;
    enum Job_Type type;
    char name[15];
};


struct job create_job(char name[],int time,enum Job_Type type);


#endif //PROJECT1_GIORGIO_S_DISCRETE_EVENT_SIMULATOR_JOB_H