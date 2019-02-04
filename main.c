#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <stdio.h>
#include "my_random.h"
#include "my_fifo_queue.h"
#include "my_priority_queue.h"
#include "job.h"
#include "test.h"

unsigned int SEED;
int INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX;
double QUIT_PROB;


bool cpu_idle   = true;
bool disk1_idle = true;
bool disk2_idle = true;

int job_count=1;
FILE *log_file;
FILE *stats_file;

//stats
int pq_size  = 0;
int pq_max   = 0;

int cpu_size = 0;
int cpu_max = 0;
int cpu_busy=0;
int cpu_complete=0;
int cpu_time=0;

int d1_size = 0;
int d1_max = 0;
int d1_busy=0;
int d1_complete=0;
int d1_time=0;

int d2_size = 0;
int d2_max = 0;
int d2_busy=0;
int d2_complete=0;
int d2_time=0;

int parse_int(const char *string);

void handle_event(int g_time, struct job current_job, struct my_priority_queue *priority_queue,
                  struct my_fifo_queue *cpu_queue, struct my_fifo_queue *disk1_queue,
                  struct my_fifo_queue *disk2_queue);

void log_constants(const char *buf, int number);

void handle_server(int g_time, struct my_priority_queue *priority_queue,struct my_fifo_queue *fifo_queue,
        enum Job_Type type);

void record_stats(struct my_fifo_queue *queue, int dif_time,int *size,int *max,int *busy,bool idle);

void write_stats();

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
        log_constants(buf, number);
    }
    fclose(read_file);
    init_random(SEED);

    int g_time = INIT_TIME;

    struct job job1 = create_job("Job1",0,CPU_Arrival,0);
    job_count++;
    add_pq(&priority_queue,job1);
    struct job endSim = create_job("END SIM",FIN_TIME,SIM_END,0);
    add_pq(&priority_queue,endSim);





    //main loop
    while(!is_empty_pq(&priority_queue)) {
        //Pop Priority Queue
        struct job current_job = remove_pq(&priority_queue);

        //stats
        int dif_time =  current_job.time-g_time;
        pq_size += dif_time*priority_queue.size;
        pq_max = pq_max > priority_queue.size ? pq_max : priority_queue.size;
        record_stats(&cpu_queue, dif_time,&cpu_size,&cpu_max,&cpu_busy,cpu_idle);
        record_stats(&disk1_queue, dif_time,&d1_size,&d1_max,&d1_busy,disk1_idle);
        record_stats(&disk2_queue, dif_time,&d2_size,&d2_max,&d2_busy,disk2_idle);
        //log
        fprintf(log_file,"%s %s %d\n",current_job.name,type_string(current_job.type),current_job.time);

        //Handle Event
        g_time = current_job.time;
        handle_event(g_time, current_job, &priority_queue, &cpu_queue, &disk1_queue, &disk2_queue);
        //Process CPU
        if (!is_empty_fq(&cpu_queue) && cpu_idle) {
            handle_server(g_time, &priority_queue, &cpu_queue,CPU_Begin);
        }
        //Process Disk1
        if (!is_empty_fq(&disk1_queue) && disk1_idle) {
            handle_server(g_time,&priority_queue,&disk1_queue,Disk1_Begin);
        }
        //Process Disk2
        if (!is_empty_fq(&disk2_queue) && disk2_idle) {
            handle_server(g_time,&priority_queue,&disk2_queue,Disk2_Begin);
        }
    }
    return 1;
}

void record_stats(struct my_fifo_queue *queue, int dif_time,int *size, int *max, int *busy, bool idle) {
    *size += dif_time * (*queue).size;
    *max = *max > (*queue).size ? *max : (*queue).size;
    *busy += idle ? 0 : dif_time;
}

void handle_server(int g_time, struct my_priority_queue *priority_queue, struct my_fifo_queue *fifo_queue,
        enum Job_Type type) {
    struct job pop_job = remove_fq(fifo_queue);
    struct job new_job = create_job(pop_job.name,g_time,type, pop_job.arrive);
    add_pq(priority_queue, new_job);
}

void log_constants(const char *buf, int number) {
    if(strstr(buf, "SEED")){
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


void handle_event(int g_time, struct job current_job, struct my_priority_queue *priority_queue,
                  struct my_fifo_queue *cpu_queue, struct my_fifo_queue *disk1_queue,
                  struct my_fifo_queue *disk2_queue){
    if (current_job.type == CPU_Arrival || current_job.type == CPU_Continue) {
        //Add job to cpu queue
        current_job.arrive = g_time;
        add_fq(cpu_queue,current_job);

        //Add new job
        if(current_job.type == CPU_Arrival) {
            char name[15];
            sprintf(name, "%s%d", "Job", job_count++);
            int rand = my_random(ARRIVE_MIN, ARRIVE_MAX);
            struct job new_job = create_job(name, g_time + rand, CPU_Arrival,0);
            add_pq(priority_queue, new_job);
        }

    } else if (current_job.type == CPU_Begin) {
        cpu_idle=false;
        int rand = my_random(CPU_MIN,CPU_MAX);
        struct job finish_job = create_job(current_job.name,g_time+rand,CPU_Finish,current_job.arrive);
        add_pq(priority_queue,finish_job);
    } else if (current_job.type == CPU_Finish) {
        //Free CPU and Change job to exit or to goto the disks
        cpu_time += g_time-current_job.arrive;
        cpu_complete++;
        cpu_idle = true;
        enum Job_Type type;
        //check if you should quit
        if(chance(QUIT_PROB)){
            type = Exit;
        } else if (disk1_queue->size < disk2_queue->size) {
            type = Disk1_Arrival;
        } else if (disk1_queue->size >disk2_queue->size) {
            type = Disk2_Arrival;
        } else {
            int r = my_random(0, 2);
            type = r == 0 ? Disk1_Arrival : Disk2_Arrival;
        }
        struct job disk_job = create_job(current_job.name,current_job.time,type,0);
        add_pq(priority_queue,disk_job);
    } else if (current_job.type == Disk1_Arrival) {
        current_job.arrive = g_time;
        add_fq(disk1_queue, current_job);
    } else if (current_job.type == Disk1_Begin) {
        disk1_idle=false;
        int rand = my_random(DISK1_MIN,DISK1_MAX);
        struct job finish = create_job(current_job.name,g_time+rand,Disk1_Finish,current_job.arrive);
        add_pq(priority_queue,finish);
    } else if (current_job.type == Disk2_Arrival) {
        current_job.arrive = g_time;
        add_fq(disk2_queue, current_job);
    } else if(current_job.type == Disk2_Begin){
        disk2_idle=false;
        int rand = my_random(DISK2_MIN,DISK2_MAX);
        struct job finish = create_job(current_job.name,g_time+rand,Disk2_Finish,current_job.arrive);
        add_pq(priority_queue,finish);
    } else if (current_job.type == Disk1_Finish || current_job.type == Disk2_Finish) {

        if(current_job.type == Disk1_Finish){
            disk1_idle=true;
            d1_time+=g_time-current_job.arrive;
            d1_complete++;
        } else{
            disk2_idle=true;
            d2_time+=g_time-current_job.arrive;
            d2_complete++;
        }

        struct job d_job = create_job(current_job.name,current_job.time,CPU_Continue,0);
        add_pq(priority_queue,d_job);
    } else if (current_job.type == Exit) {
    } else if (current_job.type == SIM_END) {
        printf("Simulation End\n%d Jobs\n",job_count);
        write_stats();

        exit(0);
    }
}

void write_stats() {
    double pq_avg_size  = (((double)pq_size) / FIN_TIME);

    double cpu_avg_size  = (((double)cpu_size)/FIN_TIME);
    double cpu_avg_busy  = (((double)cpu_busy)/FIN_TIME)*100;
    double cpu_avg_thro  = (((double)cpu_complete)/FIN_TIME);
    double cpu_rtime = (((double)cpu_time)/cpu_complete);

    double d1_avg_size  = (((double)d1_size)/FIN_TIME);
    double d1_avg_busy  = (((double)d1_busy)/FIN_TIME)*100;
    double d1_avg_thro  = (((double)d1_complete)/FIN_TIME);
    double d1_rtime = (((double)d1_time)/d1_complete);

    double d2_avg_size  = (((double)d2_size)/FIN_TIME);
    double d2_avg_busy  = (((double)d2_busy)/FIN_TIME)*100;
    double d2_avg_thro  = (((double)d2_complete)/FIN_TIME);
    double d2_rtime = (((double)d2_time)/d2_complete);

    stats_file = fopen("stats.txt","w");
    fprintf(stats_file,"PQ AVG_SIZE    %.4f   MAX    %2d \n",pq_avg_size,pq_max);
    fprintf(stats_file,"CPU AVG_SIZE   %.4f   MAX    %2d BUSY PERCENT     %.3f%% THROUGHPUT %.4f AVG R TIME %.4f \n",
                cpu_avg_size, cpu_max, cpu_avg_busy, cpu_avg_thro,cpu_rtime);
    fprintf(stats_file,"Disk1 AVG_SIZE %.4f MAX   %2d BUSY PERCENT     %.3f%% THROUGHPUT %.4f AVG R TIME %.4f\n",
                d1_avg_size,d1_max, d1_avg_busy, d1_avg_thro,d1_rtime);
    fprintf(stats_file,"Disk2 AVG_SIZE %.4f MAX   %2d BUSY PERCENT     %.3f%% THROUGHPUT %.4f AVG R TIME %.4f\n",
                d2_avg_size,d2_max,d2_avg_busy, d2_avg_thro,d2_rtime);
    fclose(log_file);
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