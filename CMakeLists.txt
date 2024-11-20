cmake_minimum_required(VERSION 3.29)
project(os_term_project C)

set(CMAKE_C_STANDARD 11)

add_executable(os_term_project
        main.c
        message_queue.c
        process.c
        scheduler.c
        utils.c
        message_queue.h
        process.h
        scheduler.h
        utils.h
)
