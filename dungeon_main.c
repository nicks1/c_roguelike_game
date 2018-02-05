/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: nicholas
 *
 * Created on January 16, 2018, 4:05 PM
 */

#include "prior.h"
#include "prior.c"

struct dist_point{
    int hardness;
    int distance;
    struct dist_point child;
    struct dist_point parent;
};


int main(int argc, char** argv) {
    bool map_generated = false;
    struct point map[21][80];
    int num_rooms = ((rand()%3)+6);
    struct room rooms[num_rooms];
    
    if (argc==1){
        create_print(map,num_rooms, rooms);
        return(EXIT_SUCCESS);
    }
    set_pc(map);
    
//    for(int i = 1; i<argc; ++i){
//       if(strcmp("--save", argv[i])==0){
//           write_file(map, num_rooms, rooms, map_generated);
//       } 
//       else if (strcmp("--load", argv[i])==0){
//          read_file(map); 
//          map_generated = true;
//       }
//       else{
//           printf("There seems to be an invalid input.\n");
//           return(-1);
//       }
//    }
    
    
    return (EXIT_SUCCESS);
}

void digging_path(){
    
}

void non_digging(){
    
}

void set_pc(struct point map[21][80]){
    srand = time(NULL);
    struct being pc;
    pc.name = "PC";
    int xpoint = rand()%80;
    int ypoint = rand()%21;
    
    if(map[ypoint][xpoint].hardness==0){
        map[ypoint][xpoint].being = pc;
    }
    
    
}

void copy_map(struct point o_map[21][80], struct dist_point c_map[21][80]){

    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
            c_map[i][j].hardness = o_map[i][j].hardness;
            c_map[i][j].distance = 999;
            c_map[i][j].child = NULL;
            c_map[i][j].parent = NULL;
            if(o_map[i][j].being!=NULL && !strcmp(o_map[i][j].being.name,"PC")){
                c_map[i][j].distance = 0;
            }
        }
    }
}