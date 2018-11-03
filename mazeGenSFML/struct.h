#ifndef STRUCTS
#define STRUCTS
enum side
{
	sTop,
	sLeft,
	sBottom,
	sRight
};

struct cell
{
	int x;
	int y;
};

struct cellPack
{
	cell cells[4];
	int size;
};

enum direction
{
	up,
	dright,
	down,
	dleft
};

struct validDirs
{
	int size;
	int dirs[4];
};
#endif