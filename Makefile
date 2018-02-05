
all: dungeon_main prior

dungeon_main: dungeon_main.c
	gcc -o dungeon_main dungeon_main.c
	
prior: prior.c
	gcc -c prior.c
	
clean: 
	rm -f GenerateDungeon