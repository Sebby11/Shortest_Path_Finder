# Shortest_Path_Finder<br />
**NOTE: The project as it stands is unfinished. I have yet to implement A***
<br /><br />
Warming up with C for upcoming courses.<br />
This program takes file input representing a dungeon map then finds the shortest paths from point A to B using various graph algorithms. 
<br /><br />
Example Map (map.txt):<br />

```
##########
#.......B#
#....##..#
#.....####
#........#
########.#
#........#
#A.......#
##########
```
### Key
'#' = Wall<br />
'.' = Floor<br />
A,B = Start, End<br />
<br />
### Algorithms<br />
- Dijkstra's<br />
- A*<br />

<br />**To Use:**<br />
Run Makefile to compile "Pathing"
```
make pathing
```
Run
```
./pathing
```
If you have valgrind you could also see there are no memory leaks
```
valgrind ./pathing
```
To get rid of the executable
```
make clean
```
<br /><br />
