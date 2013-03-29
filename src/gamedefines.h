#ifndef GAMEDEFINES_H
#define GAMEDEFINES_H
#define WORLDSCALE 10
#define TILESIZE 10

enum CellState {EMPTY, ALIVE, LEFT, RIGHT, UP, DOWN, SOLID};
enum TileType {NORMNEIGH, DIANEIGH, STRAIGHTNEIGH};
enum WrapMode {USEOFFMAP, MIRROR, WRAP};

#endif