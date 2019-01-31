#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include "my_random.h"
#include "my_fifo_queue.h"
#include "my_priority_queue.h"
#include "job.h"

unsigned int SEED = 01;
int INIT_TIME = 0;
int FIN_TIME = 1000;
int ARRIVE_MIN = 2;
int ARRIVE_MAX = 30;
double QUIT_PROB = .2;
int CPU_MIN = 10;
int CPU_MAX = 40;
int DISK1_MIN = 45;
int DISK1_MAX = 100;
int DISK2_MIN = 45;
int DISK2_MAX = 100;


bool cpu_idle   = true;
bool disk1_idle = true;
bool disk2_idle = true;

int job_count=2;
int main() {
    init_random(SEED);
    int g_time = INIT_TIME;



    struct my_priority_queue priority_queue = create_p_queue();
    struct my_fifo_queue cpu_queue = create_fifo_queue();
    struct my_fifo_queue disk1_queue = create_fifo_queue();
    struct my_fifo_queue disk2_queue = create_fifo_queue();

    struct job job1 = create_job("Job1",0,Arrival);
    add_pq(&priority_queue,job1);
    struct job endSim = create_job("END SIM",FIN_TIME,SIM_END);
    add_pq(&priority_queue,endSim);


    while(!is_empty_pq(&priority_queue)) {
        //Pop Priority Queue
        struct job current_job = remove_pq(&priority_queue);
        g_time = current_job.time;

        if (current_job.type == Arrival) {

            //Add new job
            int rand = my_random(ARRIVE_MIN, ARRIVE_MAX);
            char name[15];
            char label[4] = "Job";
            sprintf(name, "%s%d", label, job_count++);
            struct job new_job = create_job(name, g_time + rand, Arrival);
            add_pq(&priority_queue,new_job);

            //Arrival
            struct job arrival_job = create_job(current_job.name, current_job.time, CPU_Begin);
            add_pq(&priority_queue, arrival_job);
            printf("PQ Arrive\n");
        } else if (current_job.type == CPU_Begin) {
            //Add Job to CPU queue
            add_fq(&cpu_queue, current_job);
            printf("PQ CPU Begin\n");
        } else if (current_job.type == CPU_Finish) {
            //Free CPU and Change job to exit or to goto the disks
            cpu_idle = true;
            enum Job_Type type;
            if(chance(QUIT_PROB)){
                type = Exit;
            } else if (size_fq(disk1_queue) < size_fq(disk2_queue)) {
                type = Disk1_Arrival;
            } else if (size_fq(disk1_queue) > size_fq(disk2_queue)) {
                type = Disk2_Arrival;
            } else {
                int r = my_random(0, 2);
                type = r == 0 ? Disk1_Arrival : Disk2_Arrival;
            }
            struct job disk_job = create_job(current_job.name,current_job.time,type);
            add_pq(&priority_queue,disk_job);
            printf("PQ CPU Finish\n");
        } else if (current_job.type == Disk1_Arrival) {
            add_fq(&disk1_queue, current_job);
            printf("PQ D1 Begin\n");
        } else if (current_job.type == Disk2_Arrival) {
            add_fq(&disk2_queue, current_job);
            printf("PQ D2 Begin\n");
        } else if (current_job.type == Disk1_Finish || current_job.type == Disk2_Finish) {
            disk1_idle = true;
            struct job d_job = create_job(current_job.name,current_job.time,CPU_Begin);
            add_pq(&priority_queue,d_job);
            printf("PQ Disk Finish\n");
        } else if (current_job.type == Exit) {
            printf("PQ Job Exit\n");
        } else if (current_job.type == SIM_END) {
            printf("Simulation End\n");
            exit(0);
        }
        //Pop CPU Queue
        if (!is_empty_fq(&cpu_queue) && cpu_idle) {
            cpu_idle = false;
            struct job cpu_job = remove_fq(&cpu_queue);
            int rand = my_random(CPU_MIN, CPU_MAX);
            int time = cpu_job.time += rand;
            cpu_job = create_job(cpu_job.name, time, CPU_Finish);
            add_pq(&priority_queue, cpu_job);
        }
        //Pop Disk1 Queue
        if (!is_empty_fq(&disk1_queue) && disk1_idle) {
            disk1_idle = false;
            struct job disk1_job = remove_fq(&disk1_queue);
            int rand = my_random(DISK1_MIN, DISK1_MAX);
            int time = disk1_job.time += rand;
            disk1_job = create_job(disk1_job.name, time, Disk1_Finish);
            add_pq(&priority_queue, disk1_job);
        }
        //Pop Disk2 Queue
        if (!is_empty_fq(&disk2_queue) && disk2_idle) {
            disk2_idle = false;
            struct job disk2_job = remove_fq(&disk2_queue);
            int rand = my_random(DISK2_MIN, DISK2_MAX);
            int time = disk2_job.time += rand;
            disk2_job = create_job(disk2_job.name, time, Disk1_Finish);
            add_pq(&priority_queue, disk2_job);
        }

    }
    return 0;
}