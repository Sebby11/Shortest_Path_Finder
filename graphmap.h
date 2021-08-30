/*
	File: graphmap.h

	What: Header file for graphmap.c 'class' file

	By: Sebastian Medina

*/

#ifndef _GRAPHMAP_H_INCLUDE_
#define _GRAPHMAP_H_INCLUDE_

typedef struct coord {
	int x;
	int y;
	int m;
} coordObj;

typedef coordObj * coord;

typedef struct graphmap {
	char ** map;
	coord startPoint;
	coord endPoint;
	coord mapSize;
} graphmapObj;

typedef graphmapObj * graphmap;

graphmap newGraphMap(FILE * file_read, int numRows, int numCols, 
						char startVal, char endVal);

coord newCoord(int x, int y);

void txtToArray(char ** map, FILE * file_read, int row, int col);

void printMap(FILE* out, graphmap A);

void freeGraphMap(graphmap A);

void Dijkstra(FILE * file_write, graphmap A);

void getNeighbors(graphmap A, int r, int c, coord * neighbors);

void findPoint(graphmap A, coord toAlter, char toFind);

#endif