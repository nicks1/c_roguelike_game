/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "prior.h"

void read_file(struct point map[21][80]){
    FILE *f;
    char *directory = get_dir();
    f = fopen(directory, "r");
    if (!f) {
        fprintf(stderr, "Couldn't open file\n");
        return;
    }
    reset_map(map);
    char name[13];
    unsigned char hardness[1680];
    int *version = malloc(sizeof(int));
    int *size = malloc(sizeof(int));
    int count = 0;
    int num_rooms = 0;
           
    reset_map(map);
    unsigned char room_data[4];
           
    fread(name, sizeof(char), 12, f);//file marker
    count += 12;
           
    fread(version, sizeof(int), 1, f);// version number
    *version = be32toh(*version);
    count += sizeof(int)*1;
           
    fread(size, sizeof(int), 1, f);//Total size of the file. need to reverse endiness
    *size = be32toh(*size);
    count += sizeof(int)*1;
           
    num_rooms = ((*size-1700)/4);
    fread(hardness, 1, 1680, f);//hardness info
    count += 1680;
    fill_map(map,hardness);

    struct room rooms[num_rooms]; //rooms info
    for(int i=0; i<num_rooms; ++i){
        fread(room_data, 1, 4, f);
        rooms[i].ycord = (int)room_data[0];
        rooms[i].xcord = (int)room_data[1];
        rooms[i].ysize = (int)room_data[2];
        rooms[i].xsize = (int)room_data[3];
    }
    int j = 0;
    while(j<num_rooms){
        for(int k=0; k<rooms[j].ysize; ++k){
            for(int l=0; l<rooms[j].xsize; ++l){
                map[rooms[j].ycord+k][rooms[j].xcord+l].is_room = true;
                map[rooms[j].ycord+k][rooms[j].xcord+l].hardness = 0;
            }
        }
        ++j;
    }

    fclose(f);
    print_dungeon(map);

            
}

bool write_file(struct point map[21][80], int num_rooms, struct room rooms[num_rooms], bool map_generated){
    FILE *f;
    if(!map_generated)
        create_print(map, num_rooms, rooms);
    else
        print_dungeon(map);
    char *directory = get_dir();
    f = fopen(directory, "w");
    if (!f) {
        fprintf(stderr, "Couldn't open file\n");
        return false;
    }
    
    char name[13];
    unsigned char hardness[1680];
    char marker[] = "RLG327-S2018";
    int version = 0;
    int size = htobe32(1700+(num_rooms*4));
    unsigned char room_data[4];

    int fwrite_ = fwrite(marker,sizeof(char),12,f);

    fwrite(&version, sizeof(int), 1, f);

    fwrite(&size, sizeof(int), 1, f);

    fill_hardness(map, hardness);
    fwrite(hardness, 1, 1680, f);


    for(int i=0; i<num_rooms; ++i){
        room_data[0] = (unsigned char)rooms[i].ycord;
        room_data[1] = (unsigned char)rooms[i].xcord;
        room_data[2] = (unsigned char)rooms[i].ysize;
        room_data[3] = (unsigned char)rooms[i].xsize;
        fwrite(room_data, 1, 4, f);
    }
    fclose(f);
    return true;
}

void fill_hardness(struct point map[21][80], unsigned char hardness[1680]){
    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
             hardness[j+(i*80)] = (unsigned char)map[i][j].hardness;
        }
    }
}

void fill_map(struct point map[21][80],unsigned char hardness[1680]){

    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
            map[i][j].hardness = (int)hardness[j+(i*80)];
        }
    }
}

char* get_dir(){
    int length = strlen(getenv("HOME")) + strlen("/.rlg327/dungeon");
    char *p = malloc(length*sizeof(char));
    strcpy(p, getenv("HOME"));
    strcat(p,"/.rlg327/dungeon");
    return p;
}

void create_print(struct point map[21][80], int num_rooms, struct room rooms[num_rooms]){
    srand(time(NULL));
    
    reset_map(map);
    set_hardness(map);
    rooms_halls(map,num_rooms, rooms);
    print_dungeon(map); 
}

void rooms_halls(struct point map[21][80], int num_rooms, struct room rooms[num_rooms]){
    
    for(int i=0; i<num_rooms; ++i){
        rooms[i] = generate_room();
    }
    int j = 0;
    while(j<num_rooms){
       int xrand = ((rand()%77)+1);
       int yrand = ((rand()%17)+1);
       
       
       if(((rooms[j].xsize+xrand)<79) && ((rooms[j].ysize+yrand)<20) && check_room(map, xrand, yrand, rooms[j].xsize, rooms[j].ysize)){
           
           rooms[j].xcord = xrand;
           rooms[j].ycord = yrand;
           
           for(int k=0; k<rooms[j].ysize; ++k){
               for(int l=0; l<rooms[j].xsize; ++l){
                   map[yrand+k][xrand+l].is_room = true;
                   map[yrand+k][xrand+l].hardness = 0;
               }
           }
           ++j;
       }
    }
    j=0;
    //Halls - The most simple way
    while(j<num_rooms-1){
        //x-axis
        int k=0,l=0;
        if(rooms[j].xcord<rooms[j+1].xcord){
           for(k; k<=(rooms[j+1].xcord-rooms[j].xcord); ++k){
             map[rooms[j].ycord+l][rooms[j].xcord+k].hardness = 0;  
           }
           --k;
        }
        else{
           for(k; k>=(rooms[j+1].xcord-rooms[j].xcord); --k){
              map[rooms[j].ycord+l][rooms[j].xcord+k].hardness = 0; 
           }
           ++k;
        }
        
        //y-axis
        if(rooms[j].ycord<rooms[j+1].ycord){
           for(l; l<=(rooms[j+1].ycord-rooms[j].ycord); ++l){
             map[rooms[j].ycord+l][rooms[j].xcord+k].hardness = 0;  
           }
        }
        else{
           for(l; l>=(rooms[j+1].ycord-rooms[j].ycord); --l){
              map[rooms[j].ycord+l][rooms[j].xcord+k].hardness = 0; 
           }
        }
        ++j;
    }
    
    
}

bool check_room(struct point map[21][80],int xpoint,int ypoint,int xsize,int ysize){ 
  for(int i=ypoint-1; i<ysize+ypoint+1; ++i){
      for(int j=xpoint-1; j<xsize+xpoint+1; ++j){
          if(map[i][j].is_room || map[i][j].hardness==255){
              return false;
          }
      }
  }  
  return true;
}


struct room generate_room(){
    //larger than 3 units in x & 3 units in y
    int x = ((rand()%7)+3);
    int y = ((rand()%7)+3);
    struct room room = {0, 0, x, y};
    return room;
}

void set_hardness(struct point map[21][80]){
   int i,j;
   
   for(i=0; i<21; ++i){
       for(j=0; j<80; ++j){
           if(i==0 || j==0 || i==20 || j==79){
               map[i][j].hardness = 255;
           }
           else{
               map[i][j].hardness = ((rand()%70)+1);
           }
       }
   }
}

void print_dungeon(struct point map[21][80]){
    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
            if(map[i][j].hardness==0){
                if(map[i][j].is_room)
                    printf(".");
                else
                    printf("#");
            }
            else if(map[i][j].hardness>0 && map[i][j].hardness<255){
                printf(" ");
            }
            else if(map[i][j].hardness==255){
                if(i==0 || i==20)
                    printf("-");
                else if(j==0 || j== 79)
                    printf("|");
//                printf("|");
            }
            else{
                printf("Something went wrong!");
            }
        }
        printf("\n");
    }
}

void reset_map(struct point map[21][80]){
    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
           map[i][j].hardness = 0;
           map[i][j].is_room = false;
           map[i][j].being = NULL;
        }
    }
}

void print_hardness(struct point map[21][80]){
    
    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
            printf("%2d", map[i][j].hardness);
        }
        printf("\n");
    }
}

void print_rooms(struct point map[21][80]){
    
    for(int i=0; i<21; ++i){
        for(int j=0; j<80; ++j){
            if(map[i][j].is_room)
                printf("T");
            else
                printf("F");
        }
        printf("\n");
    }
}