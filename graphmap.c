/*
	File: graphmap.c

	What: 'Class' file creating and handling graphmap objects

	By: Sebastian Medina

	TODO: 
		- Add a*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graphmap.h"

coord newCoord(int x, int y){
	/*
		Use:
			Create a new coord object representing a coordinate on a plane
			or a tuple of size 2.

		Parameters:
			x - 1st value in coord; representing position on x axis
			y - 2nd value in coord; representing position on y axis
	*/
	
	coord crd = malloc(sizeof(coordObj));
	crd->x    = x;
	crd->y    = y;
	return(crd);
}

graphmap newGraphMap(FILE * file_read, int numRows, int numCols, 
						char startVal, char endVal){
	/*
		Use:
			Create a new graphmap object containing:
				- 2d array for a 'map'
				- starting position
				- end position
				- size of 2d array

		Parameters:
			file_read - input map from a txt file
			numRows   - number of rows in input map
			numCols   - number of columns in input map
			startVal  - start point character
			endVal    - end point character

	*/

	graphmap newMap    = malloc(sizeof(graphmapObj));

	newMap->map        = (char **)malloc(9 * sizeof(char *));
	for(int i = 0; i < 9; i++){
		newMap->map[i] = (char *)malloc(10 * sizeof(char));
	}

	newMap->startPoint = newCoord(-1, -1);
	newMap->endPoint   = newCoord(-1, -1);
	newMap->mapSize    = newCoord(numRows, numCols);

	txtToArray(newMap->map, file_read, newMap->mapSize->x, newMap->mapSize->y);

	findPoint(newMap, newMap->startPoint, startVal);
	findPoint(newMap, newMap->endPoint, endVal);

	printf("startpoint: x: %d y: %d\n", newMap->startPoint->x, newMap->startPoint->y);
	printf("endPoint: x: %d y: %d\n", newMap->endPoint->x, newMap->endPoint->y);
	

	return(newMap);
}

void txtToArray(char ** map, FILE * file_read, int row, int col){
	/*
		Use:
			Transform a txt map into a 2d array.

		Parameters:
			map 	  - empty 2d character array
			file_read - file where the original txt map is stored
			row 	  - number of rows in original mamp
			col 	  - number of columns in original map

	*/

	char line[11];

	for(int i = 0; i < row; i++){
		fgets(line, 11, file_read);	// Grab actual map line

		for(int j = 0; j < col; j++){
			map[i][j] = line[j];
		}

		fgets(line, 2, file_read);	// Get rid of newline
	}
}

void Dijkstra(FILE * file_write, graphmap A) {
	/*
		Use:
			Runs Dijkstra's shortest path algorithm to find
			a path from one point to another.

		Parameters:
			file_write - file to overwrite the shortest path map
			A 		   - graphmap containing the map

	*/

	int row    = A->mapSize->x;
	int col    = A->mapSize->y;
	int startX = A->startPoint->x;
	int startY = A->startPoint->y;
	int endX   = A->endPoint->x;
	int endY   = A->endPoint->y;
	int cnt    = 0;

	// Only works with paths of 99 steps
	coord dijkstraPath[100];

	coord Q[row][col];			// Set of nodes to go through
	coord previous[row][col];	// [i][j] contains its previous node
	int distance[row][col];		// [i][j] contains its distance from the start

	coord currCoord = newCoord(-1, -1);

	// Initialization
	for(int i = 0; i < row; i++){
		for(int j = 0; j < col; j++){
			distance[i][j] = INT_MAX;
			previous[i][j] = newCoord(-1, -1);
			Q[i][j]		   = newCoord(i, j);
		}
	}
	distance[startX][startY] = 0;

	// Start Main Loop
	for(int m = 0; m < row * col; m++){
		int tmpmin = INT_MAX;

		// Create a priority queue that's more efficient than
		//  going through the map every time.
		for(int i = 0; i < row; i++){
			for(int j = 0; j < col; j++){
				if(distance[i][j] < tmpmin && Q[i][j] != NULL){
					free(currCoord);			// Free the old one to get the new one
					tmpmin	  = distance[i][j];
					currCoord = newCoord(Q[i][j]->x, Q[i][j]->y);
				}
			}
		}

		// Remove current Node from List
		free(Q[currCoord->x][currCoord->y]);
		Q[currCoord->x][currCoord->y] = NULL;

		// If we get to the endPoint
		if(previous[endX][endY]->x != -1 || (endX == startX && endY	== startY)){
			int r = endX;
			int c = endY;
			int tmpr;
			int tmpc;
			while(previous[r][c]->x != -1){
				dijkstraPath[cnt] = newCoord(r, c);
				tmpr = previous[r][c]->x;
				tmpc = previous[r][c]->y;
				r 	 = tmpr;
				c 	 = tmpc;
				cnt++;
			}
			dijkstraPath[cnt] = newCoord(startX, startY);
			cnt++;

			// Now have shortest path
			fprintf(file_write, "PATH:\n");
			for(int i = cnt - 1; i >= 0; i--){
				fprintf(file_write, "%d: r: %d c: %d \n", cnt - i, dijkstraPath[i]->x, dijkstraPath[i]->y);
			}
			for(int i = 1; i < cnt - 1; i++){
				A->map[dijkstraPath[i]->x][dijkstraPath[i]->y] = '*';
				free(dijkstraPath[i]);
			}
			free(dijkstraPath[0]);
			free(dijkstraPath[cnt-1]);
			printMap(file_write, A);
			printMap(stdout, A);
			break;
		}
		
		// Get Neighbors of current Node
		coord * neighbors = (coord *)malloc(8 * sizeof(coord));
		getNeighbors(A, currCoord->x, currCoord->y, neighbors);

		// Update neighbor distances
		for(int i = 0; i < 8; i++){
			if(neighbors[i]->x == -1){
				continue;
			}
			int nX 		  = neighbors[i]->x;
			int nY 		  = neighbors[i]->y;
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

	if(cnt == 0){
		printf("NO AVAILABLE PATH FROM START TO FINISH.\n");
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
		Use:
			Finds the neighbors of a node in a 2d array, and
			turns those neighbors into coord objects to be 
			stored in 'neighbors' coord list.

		Parameters;
			A 		  - graphmap containing the node you want to find neighbors of
			r 		  - current row  
			c 		  - current column
			neighbors - pointer to coord list where neighbors will be saved

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
	/*
		Use:
			Print the dungeon map of graphmap A to out stream.

		Parameters:
			out - stream where the map will be printed
			A   - graphmap containing the map you want to print
	*/

	for(int i = 0; i < (A->mapSize->x); i++){
		for(int j = 0; j < (A->mapSize->y); j++){
			fprintf(out, "%c", A->map[i][j]);
		}
		fprintf(out, "\n");
	}
}

void freeGraphMap(graphmap A) {
	/*
		Use:
			Free all memory allocated by a graphmap.

		Parameters:
			A - graphmap object you'd like to free

	*/

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

void findPoint(graphmap A, coord crd, char toFind){
	/*
		Use:
			Find certain character, toFind, in A's 2d array map
			and enter its coordinates into crd.

		Parameters:
			A      - graphmap containing the char you want to find on its map
			crd    - coord object where char's coordinates are stored
			toFind - the char you'd like to find

	*/

	for(int i = 0; i < A->mapSize->x; i++){
		for(int j = 0; j < A->mapSize->y; j++){
			if(A->map[i][j] == toFind){
				crd->x = i;
				crd->y = j;
				return;
			}
		}
	}
}