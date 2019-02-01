#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <stdio.h>
#include "my_random.h"
#include "my_fifo_queue.h"
#include "my_priority_queue.h"
#include "job.h"

unsigned int SEED;
int INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX;
double QUIT_PROB;


bool cpu_idle   = true;
bool disk1_idle = true;
bool disk2_idle = true;

int job_count=1;
FILE *log_file;


int parse_int(const char *string);

void handle_event(int g_time, struct job current_job, struct my_priority_queue *priority_queue,
                  struct my_fifo_queue *cpu_queue, struct my_fifo_queue *disk1_queue,
                  struct my_fifo_queue *disk2_queue);


//void pop_fifo_queue(struct my_priority_queue *priority_queue, struct my_fifo_queue *queue,enum Job_Type type,
//        int min,int max);

int main() {
    //create queues
    struct my_priority_queue priority_queue = create_p_queue();
    struct my_fifo_queue cpu_queue = create_fifo_queue();
    struct my_fifo_queue disk1_queue = create_fifo_queue();
    struct my_fifo_queue disk2_queue = create_fifo_queue();

    FILE *read_file;
    char buf[1000];

    read_file=fopen("config.txt","r");
    if (!read_file) {
        printf("Could not find config.txt\n");
        return 1;
    }
    //setup log file
    log_file = fopen("log.txt","w+");

    //Read Config File
    while (fgets(buf,1000, read_file)!=NULL) {
        int i=0;
        int j=0;
        char num[8] = "\0\0\0\0\0\0\0\0";
        bool hit_space=false;
        do {
            if (buf[i]== ' ') {
                hit_space=true;
            }
            if (hit_space) {
                if ((buf[i] >= 48 && buf[i] <= 57) || buf[i] == '.') {
                    num[j++] = buf[i];
                }
            }
            i++;
        }while(buf[i]!=';');

        //parse num
        int number = parse_int(num);

        if(strstr(buf,"SEED")){
            SEED = (unsigned int) number;
            fprintf(log_file,"SEED %d;\n",SEED);
        } else if(strstr(buf,"INIT_TIME")){
            INIT_TIME = number;
            fprintf(log_file,"INIT_TIME %d;\n",INIT_TIME);
        } else if(strstr(buf,"FIN_TIME")){
            FIN_TIME = number;
            fprintf(log_file,"FIN_TIME %d;\n",FIN_TIME);
        } else if(strstr(buf,"ARRIVE_MIN")) {
            ARRIVE_MIN = number;
            fprintf(log_file,"ARRIVE_MIN %d;\n",ARRIVE_MIN);
        } else if(strstr(buf,"ARRIVE_MAX")){
            ARRIVE_MAX = number;
            fprintf(log_file,"ARRIVE_MAX %d;\n",ARRIVE_MAX);
        } else if(strstr(buf,"QUIT_PROB")){
            QUIT_PROB=((double)(number))/100.0;
            fprintf(log_file,"QUIT_PROB %lf;\n",QUIT_PROB);
        } else if(strstr(buf,"CPU_MIN")){
            CPU_MIN = number;
            fprintf(log_file,"CPU_MIN %d;\n",CPU_MIN);
        } else if(strstr(buf,"CPU_MAX")){
            CPU_MAX = number;
            fprintf(log_file,"CPU_MAX %d;\n",CPU_MAX);
        } else if(strstr(buf,"DISK1_MIN")){
            DISK1_MIN = number;
            fprintf(log_file,"DISK1_MIN %d;\n",DISK1_MIN);
        } else if(strstr(buf,"DISK1_MAX")){
            DISK1_MAX = number;
            fprintf(log_file,"DISK1_MAX %d;\n",DISK1_MAX);
        } else if(strstr(buf,"DISK2_MIN")){
            DISK2_MIN = number;
            fprintf(log_file,"DISK2_MIN %d;\n",DISK2_MIN);
        } else if(strstr(buf,"DISK2_MAX")){
            DISK2_MAX = number;
            fprintf(log_file,"DISK2_MAX %d;\n",DISK2_MAX);
        }
    }
    fclose(read_file);

    init_random(SEED);
    int g_time = INIT_TIME;






    struct job job1 = create_job("Job1",0,Arrival);
    job_count++;
    add_pq(&priority_queue,job1);
    struct job endSim = create_job("END SIM",FIN_TIME,SIM_END);
    add_pq(&priority_queue,endSim);




    //main loop
    while(!is_empty_pq(&priority_queue)) {
        //Pop Priority Queue
        struct job current_job = remove_pq(&priority_queue);
        g_time = current_job.time;

        //Handle Event
        handle_event(g_time, current_job, &priority_queue, &cpu_queue, &disk1_queue, &disk2_queue);
        //Pop CPU Queue
        if (!is_empty_fq(&cpu_queue) && cpu_idle) {
            struct job arrival = remove_fq(&cpu_queue);
            struct job cpu_job = create_job(arrival.name,g_time,CPU_Begin);
            add_pq(&priority_queue,cpu_job);
        }
        //Pop Disk1 Queue
        if (!is_empty_fq(&disk1_queue) && disk1_idle) {
            struct job arrival = remove_fq(&disk1_queue);
            struct job disk_job = create_job(arrival.name,g_time,Disk1_Begin);
            add_pq(&priority_queue,disk_job);
        }
        //Pop Disk2 Queue
        if (!is_empty_fq(&disk2_queue) && disk2_idle) {
            struct job arrival = remove_fq(&disk2_queue);
            struct job disk_job = create_job(arrival.name,g_time,Disk2_Begin);
            add_pq(&priority_queue,disk_job);
        }
    }
    return -1;
}
/*
void pop_fifo_queue(struct my_priority_queue *priority_queue, struct my_fifo_queue *queue,enum Job_Type type,
        int min,int max) {


    struct job fifo_job = remove_fq(queue);


    char *type_str = type_string(fifo_job.type);
    //fprintf(log_file,"%s %s %d\n",fifo_job.name,type_str,fifo_job.time);

    int rand = my_random(min, max);
    int time = fifo_job.time += rand;
    fifo_job = create_job(fifo_job.name, time, type);
    add_pq(priority_queue, fifo_job);
}
*/


void handle_event(int g_time, struct job current_job, struct my_priority_queue *priority_queue,
                  struct my_fifo_queue *cpu_queue, struct my_fifo_queue *disk1_queue,
                  struct my_fifo_queue *disk2_queue){
    if (current_job.type == Arrival) {

        //log
        char *type_str = type_string(current_job.type);
        fprintf(log_file,"%s %s %d\n",current_job.name,type_str,current_job.time);

        //Add new job
        char name[15];
        sprintf(name, "%s%d", "Job", job_count++);

        int rand = my_random(ARRIVE_MIN, ARRIVE_MAX);
        struct job new_job = create_job(name, g_time + rand, Arrival);
        add_pq(priority_queue,new_job);

        //Arrival add job to cpu queue
        add_fq(cpu_queue,current_job);
        //struct job arrival_job = create_job(current_job.name, current_job.time, CPU_Begin);
        //add_pq(priority_queue, arrival_job);

    } else if (current_job.type == CPU_Begin) {
        //log
        char *type_str = type_string(current_job.type);
        fprintf(log_file,"%s %s %d\n",current_job.name,type_str,current_job.time);


        //Add Job to CPU queue
        //add_fq(cpu_queue, current_job);
        cpu_idle=false;
        int rand = my_random(CPU_MIN,CPU_MAX);
        struct job finish_job = create_job(current_job.name,g_time+rand,CPU_Finish);
        add_pq(priority_queue,finish_job);
        printf("PQ CPU Begin\n");
    } else if (current_job.type == CPU_Finish) {
        //log
        char *type_str = type_string(current_job.type);
        fprintf(log_file,"%s %s %d\n",current_job.name,type_str,current_job.time);

        //Free CPU and Change job to exit or to goto the disks
        cpu_idle = true;
        enum Job_Type type;
        //check if you should quit
        if(chance(QUIT_PROB)){
            type = Exit;
        } else if (size_fq(*disk1_queue) < size_fq(*disk2_queue)) {
            type = Disk1_Arrival;
        } else if (size_fq(*disk1_queue) > size_fq(*disk2_queue)) {
            type = Disk2_Arrival;
        } else {
            int r = my_random(0, 2);
            type = r == 0 ? Disk1_Arrival : Disk2_Arrival;
        }
        struct job disk_job = create_job(current_job.name,current_job.time,type);
        add_pq(priority_queue,disk_job);
    } else if (current_job.type == Disk1_Arrival) {

        add_fq(disk1_queue, current_job);
        printf("PQ D1 Begin\n");
    } else if (current_job.type == Disk1_Begin) {
        disk1_idle=false;
        int rand = my_random(DISK1_MIN,DISK1_MAX);
        struct job finish = create_job(current_job.name,g_time+rand,Disk1_Finish);
        add_pq(priority_queue,finish);
    } else if (current_job.type == Disk2_Arrival) {
        add_fq(disk2_queue, current_job);
        printf("PQ D2 Begin\n");
    } else if(current_job.type == Disk2_Begin){
        disk1_idle=false;
        int rand = my_random(DISK2_MIN,DISK2_MAX);
        struct job finish = create_job(current_job.name,g_time+rand,Disk2_Finish);
        add_pq(priority_queue,finish);
    } else if (current_job.type == Disk1_Finish || current_job.type == Disk2_Finish) {
        //log
        char *type_str = type_string(current_job.type);
        fprintf(log_file,"%s %s %d\n",current_job.name,type_str,current_job.time);

        if(current_job.type == Disk1_Finish){
            disk1_idle=true;
        } else{
            disk2_idle=true;
        }

        struct job d_job = create_job(current_job.name,current_job.time,Arrival);
        add_pq(priority_queue,d_job);
        printf("PQ Disk Finish\n");
    } else if (current_job.type == Exit) {
        //char *type_str = type_string(current_job.type);
        //fprintf(log_file,"%s %s %d\n",current_job.name,type_str,current_job.time);
        printf("PQ Job Exit\n");
    } else if (current_job.type == SIM_END) {
        printf("Simulation End\n");
        fclose(log_file);
        exit(0);
    }
}

int parse_int(const char *string) {
    int digit,number=0,k=1;
    digit = (int) (strlen(string) - 1);
    while(digit>=0)
        {
            number=number + k*(string[digit]-'0');
            digit=digit-1;
            k=k*10;
        }
    return number;
}