#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphmap.h"

coord newCoord(int x, int y){
	coord crd = malloc(sizeof(coordObj));
	crd->x    = x;
	crd->y    = y;
	return(crd);
}

graphmap newGraphMap(FILE * file_read, int numRows, int numCols, 
						int startR, int startE, int endR, int endE){
	graphmap newMap    = malloc(sizeof(graphmapObj));

	newMap->map        = (char **)malloc(9 * sizeof(char *));
	for(int i = 0; i < 9; i++){
		newMap->map[i] = (char *)malloc(10 * sizeof(char));
	}

	newMap->startPoint = newCoord(startR, startE);
	newMap->endPoint   = newCoord(endR, endE);
	newMap->mapSize    = newCoord(numRows, numCols);

	txtToArray(newMap->map, file_read, newMap->mapSize->x, newMap->mapSize->y);

	return(newMap);
}

void txtToArray(char ** map, FILE * file_read, int row, int col){
	char line[11];

	for(int i = 0; i < row; i++){
		fgets(line, 11, file_read);	// Grab actual map line

		for(int j = 0; j < col; j++){
			map[i][j] = line[j];
		}

		fgets(line, 2, file_read);	// Get rid of newline
	}
}

void Dijkstra(graphmap A) {
	/*
	PseudoCode:

	Create set of nodes, Q

	for each node (n) in graph:
		distance[n] = INF
		previous[n] = UNDEF
		add n to Q
	dist[startNode] = 0


	while Q isn't empty:
		u = vertex in Q w/ min dist (starts at startPoint)
		remove u from Q

		if prev[target] defined or target == source :
			while target is defined:
				u = target
				insert u at beginning of empty sequence S
				u = prev[u]

		for each neighbor v of u:
			alt = dist[u] + (edge Weight)
			if alt < dist[v]:
				dist[v] = alt
				prev[v] = u
	*/
}

void printMap(FILE* out, graphmap A){
	for(int i = 0; i < (A->mapSize->x); i++){
		for(int j = 0; j < (A->mapSize->y); j++){
			fprintf(out, "%c", A->map[i][j]);
		}
		printf("\n");
	}
}

void freeGraphMap(graphmap A) {
	// Free 2d array
	for(int i = 0; i < A->mapSize->x; i++){
		free(A->map[i]);
	}
	free(A->map);
	A->map        = NULL;

	// Free coord objects
	free(A->startPoint);
	A->startPoint = NULL;

	free(A->endPoint);
	A->endPoint   = NULL;

	free(A->mapSize);
	A->mapSize    = NULL;

	free(A);
	A             = NULL;
}