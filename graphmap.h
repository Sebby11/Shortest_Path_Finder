#ifndef _GRAPHMAP_H_INCLUDE_
#define _GRAPHMAP_H_INCLUDE_

typedef struct coord {
	int x;
	int y;
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
						int startR, int startE, int endR, int endE);

coord newCoord(int x, int y);

void txtToArray(char ** map, FILE * file_read, int row, int col);

void printMap(FILE* out, graphmap A);

void freeGraphMap(graphmap A);

void Dijkstra(graphmap A);

#endif