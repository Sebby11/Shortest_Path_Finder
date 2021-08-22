#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphmap.h"


int main(){
	
	FILE * file_read;
	char * mapName = "map.txt";
	char line[11];

	file_read = fopen(mapName, "r");
	if(!file_read){
		perror("Failed to open file.");
		exit(1);
	}

	//	(filestream, numRows, numCols, starting x, starting y, ending x, ending y)
	graphmap map = newGraphMap(file_read, 9, 10, 7, 1, 1, 8);
	printMap(stdout, map);

	Dijkstra(map);

	freeGraphMap(map);

	fclose(file_read);
	
	return(0);
}
