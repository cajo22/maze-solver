#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "convert.h"

// Converts a 2D maze .txt file into nodes.
//
// Mazes should follow the following format:
// # is used for walls.
// - is used for traversable space.
// Both # and - should be seperated by a single whitespace character.
// There should be only one traversable space in the first and last rows.
// These are the start and goal nodes respectively.
//
// A valid maze layout example is as follows:
// # - # # # # # # # # # # #
// # - - - - - - - - - - - #
// # # # # - # # # # # - # #
// # # - - - - - - - # - # #
// # # # # # # - - - - - # #
// # # # # # # # # # - # # #

typedef struct Node NODE;

struct Node {
	int id;      // ID of this node
	int adj[4];  // IDs of adjacent nodes
};


void set_node_details(FILE *, int *, int *, int *);
int *convert_maze(FILE *);
static void construct_node_array(NODE **, FILE *, int, int);
static NODE *create_node(int, int, int, int, int);

int main(void)
{
	FILE *file;
	int node_count = 0;
	int tile_count = 0;
	int width = 0;      			     // No. of nodes, no. of spaces (for 2D array construction), width
	NODE **array_ptr;                            // Pointer to node array
	int i;

	// First open the maze file and validate it exists
	file = fopen("maze-Easy.txt", "r");
	if (file == NULL)
	{
		printf("Input file not found\n");
		exit(EXIT_FAILURE);
	}

	// Get the count, height, width
	set_node_details(file, &node_count, &tile_count, &width);

	if (!(array_ptr = (NODE **)malloc(sizeof(NODE **) * node_count)))
	{
		printf("Out of memory\n");
		exit(EXIT_FAILURE);
	}

	construct_node_array(array_ptr, file, tile_count / width, width);

	for (i = 0; i < node_count; i++)
	{
		printf("Node %d is connected to %d, %d, %d, %d\n", array_ptr[i]->id, array_ptr[i]->adj[0], array_ptr[i]->adj[1], array_ptr[i]->adj[2], array_ptr[i]->adj[3]);
	}
}

void set_node_details(FILE *file, int *node_count, int *tile_count, int *width)
{
	char line;
	bool width_flag = false;

	do
	{
		line = fgetc(file);
		if (line == '-' || line == '#')
		{
			if (line == '-')
			{
				(*node_count)++; // Valid node
			}
			(*tile_count)++;
		}
		if (line == '\n' && !(width_flag))
		{
			*width = *tile_count; // Need to only get width on line 1
			width_flag = true;
		}
	} while (!(feof(file)));
}

static void construct_node_array(NODE **nodes, FILE *file, int height, int width)
{
	char line;
	int x = 0;
        int y = 0; // x, y of the array
	int i = 1;
	int j = 1;
	int k = 1;
        int l = 1; // counters
	int grid[width][height];

	// First a 2D array is constructed that matches the grid.
	// The file is read from left to right, top to bottom.
	// Walls are set as 0 in the array, and spaces are set to what will be their node ID.
	// Node ID increments with each node.

	rewind(file);

	do
	{
		line = fgetc(file);
		
		if (line == '-')
		{
			grid[x][y] = i;
			i++;
			x++;
		}
		else if (line == '#')
		{
			grid[x][y] = 0;
			x++;
		}

		if (line == '\n')
		{
			y++;
			x = 0;
		}
	} while (!(feof(file)));

	// Now we have the array, we can fill the array of nodes, which is what we really want.

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			// Check if node should be inserted
			if (grid[x][y] != 0)
			{
				i = 0;
				j = 0;
				k = 0;
				l = 0;

				// Check for adjacent nodes
				if (y > 0)
				{
					if (grid[x][y - 1])
						i = grid[x][y - 1];
				}
				if (x < width)
				{
					if (grid[x + 1][y])
						j = grid[x + 1][y];
				}
				if (y < height)
				{
					if (grid[x][y + 1])
						k  = grid[x][y + 1];
				}
				if (x > 0)
				{
					if (grid[x - 1][y])
						l = grid[x - 1][y];
				}

				nodes[grid[x][y] - 1] = create_node(grid[x][y], i, j, k, l);

			}

		}
	}

	printf("Successfully filled nodes array\n");

}

static NODE *create_node(int id, int north, int east, int south, int west)
{
	NODE *nd;

	if (!(nd = (NODE *)malloc(sizeof(NODE))))
	{
		printf("Out of memory\n");
		exit(EXIT_FAILURE);
	}

	nd->id = id;
	nd->adj[0] = north;
	nd->adj[1] = east;
	nd->adj[2] = south;
	nd->adj[3] = west;

	return nd;
}

