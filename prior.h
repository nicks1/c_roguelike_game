/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   prior.h
 * Author: nicholas
 *
 * Created on January 31, 2018, 2:12 PM
 */

#ifndef PRIOR_H
#define PRIOR_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <endian.h>

struct room{
    int xcord;
    int ycord;
    int xsize;
    int ysize;
};
struct point{
    int hardness;
    bool is_room;
    struct being being;
};
struct being{
    char name[];
    
};


void set_hardness(struct point map[21][80]);
void print_dungeon(struct point map[21][80]);
void print_hardness(struct point map[21][80]);
void reset_map(struct point map[21][80]);
struct room generate_room();
void rooms_halls(struct point map[21][80], int num_rooms, struct room rooms[num_rooms]);
bool check_room(struct point map[21][80],int xpoint,int ypoint,int xsize,int ysize);
void print_rooms(struct point map[21][80]);
void read_file(struct point map[21][80]);
bool write_file(struct point map[21][80],int num_rooms, struct room rooms[num_rooms], bool map_generated);
char* get_dir();
void create_print(struct point map[21][80], int num_rooms, struct room rooms[num_rooms]);
void fill_map(struct point map[21][80],unsigned char hardness[1680]);
void fill_hardness(struct point map[21][80], unsigned char hardness[1680]);

#endif /* PRIOR_H */

