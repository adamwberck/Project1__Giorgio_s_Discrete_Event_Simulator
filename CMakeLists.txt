cmake_minimum_required(VERSION 3.12)
project(Project1__Giorgio_s_Discrete_Event_Simulator C)

set(CMAKE_C_STANDARD 99)

add_executable(Project1__Giorgio_s_Discrete_Event_Simulator main.c my_random.c my_random.h my_fifo_queue.c my_fifo_queue.h job.c job.h my_priority_queue.c my_priority_queue.h)