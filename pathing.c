/*
	File: pathing.c

	What: File for testing functions of graphmap.c

	By: Sebastian Medina

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphmap.h"


int main(){
	
	FILE * file_read;
	FILE * file_write;
	char * mapName = "map.txt";
	char line[11];

	file_read = fopen(mapName, "r");
	if(!file_read){
		perror("Failed to open file.");
		exit(1);
	}

	file_write = fopen("outfile.txt", "w");

	//	(filestream, numRows, numCols, starting x, starting y, ending x, ending y)
	graphmap map = newGraphMap(file_read, 9, 10, 'A', 'B');

	printf("Original Map: \n");
	printMap(stdout, map);

	printf("\nNew Map: \n");
	Dijkstra(file_write, map);



	freeGraphMap(map);

	fclose(file_read);
	fclose(file_write);
	
	return(0);
}
