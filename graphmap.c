#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
	int row    = A->mapSize->x;
	int col    = A->mapSize->y;
	int startX = A->startPoint->x;
	int startY = A->startPoint->y;
	int endX   = A->endPoint->x;
	int endY   = A->endPoint->y;

	// Only works with paths of 99 steps
	coord dijkstraPath[100];

	coord Q[row][col];	// Set of nodes to go through
	coord previous[row][col];	// [i][j] contains its previous node
	int distance[row][col];	// [i][j] contains its distance from the start

	coord currCoord = newCoord(-1, -1);

	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			distance[i][j] = INT_MAX;
			previous[i][j] = newCoord(-1, -1);
			Q[i][j]		   = newCoord(i, j);
		}
	}
	distance[startX][startY] = 0;

	for(int m = 0; m < row * col; m++){
		int tmpmin = INT_MAX;

		// Create a priority queue that's more efficient than
		//  going through the map every time.
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++){
				if(distance[i][j] < tmpmin && Q[i][j] != NULL){
					free(currCoord);			// Free the old one to get the new one
					tmpmin = distance[i][j];
					currCoord = newCoord(Q[i][j]->x, Q[i][j]->y);
				}
			}
		}

		// This is being freed twice, fix it.
		free(Q[currCoord->x][currCoord->y]);	// 'Removed' from list
		Q[currCoord->x][currCoord->y] = NULL;


		if(previous[endX][endY]->x != -1 || (endX == startX && endY	== startY)){
			int r = endX;
			int c = endY;
			int cnt = 0;
			while(previous[r][c]->x != -1){
				dijkstraPath[cnt] = newCoord(r, c);
				r = previous[r][c]->x;
				c = previous[r][c]->y;
				cnt++;
			}
			dijkstraPath[cnt] = newCoord(startX, startY);
			cnt++;

			// Now have shortest path
			printf("PATH:\n");
			for(int i = cnt - 1; i >= 0; i--){
				printf("%d: r: %d c: %d \n", cnt - i, dijkstraPath[i]->x, dijkstraPath[i]->y);
				//free(dijkstraPath[i]);
			}
			for(int i = 1; i < cnt - 1; i++){
				A->map[dijkstraPath[i]->x][dijkstraPath[i]->y] = '*';
				free(dijkstraPath[i]);
			}
			free(dijkstraPath[0]);
			free(dijkstraPath[cnt-1]);
			printMap(stdout, A);
			break;
		}
		
		coord * neighbors = (coord *)malloc(8 * sizeof(coord));
		getNeighbors(A, currCoord->x, currCoord->y, neighbors);

		
		for(int i = 0; i < 8; i++){
			if(neighbors[i]->x == -1){
				continue;
			}
			int nX = neighbors[i]->x;
			int nY = neighbors[i]->y;
			// Fix this later to have diagonal edge weights cost 1.5 not 1
			int totalDist = distance[currCoord->x][currCoord->y] + 1;
			if(totalDist < distance[nX][nY]){
				distance[nX][nY] = totalDist;
				free(previous[nX][nY]);
				previous[nX][nY] = newCoord(currCoord->x, currCoord->y);
			}
		}

		for(int i = 0; i < 8; i++){
			free(neighbors[i]);
		}
		free(neighbors);
	}

	free(currCoord);
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			if(Q[i][j] != NULL){
				free(Q[i][j]);
			}
			free(previous[i][j]);
		}
	}


	return;
}

// Given map & src point, return all 8 neighbors 
// You know as well as I do this shouldn't be hard coded >:(
void getNeighbors(graphmap A, int r, int c, coord * neighbors){
	/*
	Neighbor arrangement:
	0 - Top
	1 - Bottom
	2 - Left
	3 - Right
	4 - Top Left
	5 - Top Right
	6 - Bottom Left
	7 - Bottom Right
	*/
	
	// Top 
	if((r - 1 >= 0) && A->map[r - 1][c] != '#')
		neighbors[0] = newCoord(r - 1, c);
	else
		neighbors[0]  = newCoord(-1, -1);

	// Bottom
	if((r + 1 <= 8) && A->map[r + 1][c] != '#')
		neighbors[1] = newCoord(r + 1, c);
	else
		neighbors[1]  = newCoord(-1, -1);

	// Left
	if((c - 1 >= 0) && A->map[r][c - 1] != '#')
		neighbors[2] = newCoord(r, c - 1);
	else
		neighbors[2]  = newCoord(-1, -1);

	// Right
	if((c + 1 <= 9) && A->map[r][c + 1] != '#')
		neighbors[3] = newCoord(r, c + 1);
	else
		neighbors[3]  = newCoord(-1, -1);

	// Top Left
	if((r - 1 >= 0 && c - 1 >= 0) && A->map[r - 1][c - 1] != '#')
		neighbors[4] = newCoord(r - 1, c - 1);
	else
		neighbors[4]  = newCoord(-1, -1);

	// Top Right
	if((r - 1 >= 0 && c + 1 <= 9) && A->map[r - 1][c + 1] != '#')
		neighbors[5] = newCoord(r - 1, c + 1);
	else
		neighbors[5]  = newCoord(-1, -1);

	// Bottom Left
	if((r + 1 <= 8 && c - 1 >= 0) && A->map[r + 1][c - 1] != '#')
		neighbors[6] = newCoord(r + 1, c - 1);
	else
		neighbors[6]  = newCoord(-1, -1);

	// Bottom Right
	if((r + 1 <= 8 && c + 1 <= 9) && A->map[r + 1][c + 1] != '#')
		neighbors[7] = newCoord(r + 1, c + 1);
	else
		neighbors[7]  = newCoord(-1, -1);
	
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
