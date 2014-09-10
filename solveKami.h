#ifndef SOLVEKAMI_H
#define SOLVEKAMI_H
#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
#define BITSPERWORD 32 /* use int as vector element*/
#define SHIFT 5
#define MASK 0x1f /* the last 5 bits are 1 */
#define DEBUG 1

/* To store color and locations of a block */
typedef struct block{
  char color;
  int * location_bits; //location vector in the matrix
  struct block * next;
}color_block;

/* To manager the kinds of color in the matrix. */
typedef struct color_mem{
  char * all_the_colors;
  int * counters_of_colors; //Count number of blocks for each kind of color.
}color_memory;

/* Global vars */
int xlength, ylength;// The x and y size of the matrix.
int color_kinds; // Different colors in the matrix.
char * matrix;
int * location_stack;
int top, base; //top and base of location stack
int * tmp_bits;// bit vector used for calculation
int * tmp_bits_total;// bit vector used to stand for all the checked nodes in the matrix
int * tmp_bits_adjacency;// bit vector of one block's adjacent nodes
color_block * matrix_color_blocks;

color_memory * color_memory_manager;//global color memory
color_memory * tmp_color_memory;//used in min_weight_of_one_block
int total_blocks;// The number of blocks in total
int total_colors;// The number of colors in total
int blocks_weight;// The weight of blocks
int colors_weight;// The weight of colors. It should be larger than blocks_weight
int weight;// The weight of the present matrix
int step_num;// The counter of steps.

/* Create matrix, to store color values*/
int create_matrix();

/* full in the matrix with color */
int init_matrix();

/* Print the matrix. For debug. */
int show_matrix();

/* Free the allocated space for matrix. To avoid memory leak. */
int destory_matrix();

/* Create location bit vector */
int * create_loc_bits();

/* Destory location bits, free the space. */
int destory_loc_bits(int * bits);

/* bit vector operation, set [index] in bits to 1*/
int set_bits(int * bits, int index);

/* bit vector operation, test [x,y] in bits */
int test_bits(int * bits, int x, int y);

/* bit vector operation, test [x] in bits  */
int test_bits_single(int * bits, int x);

/* set all bits in bit vector to 0 */
int clrall_bits(int * bits);

/* For debug. Print a bits vector. */
int show_bits(int * bits);

/* Return the smallest zore's index in the location bit vector. */
int smallest_zero_index(int * bits);

/* Return the smallest one's index in the location bit vector. */
int smallest_one_index(int * bits);

/* Get the color of location [x,y] int the matrix */
char color_of_matrix(int x, int y);

/* Create location stack, locate memory space for it. */
int create_location_stack();

/* Destory location stack, free the allocated space. */
int destory_location_stack();

/* Push operation for location stack. */
int location_stack_push(int index);

/* Pop operation for locatin stack. */
int location_stack_pop();

/* Initialize all the data structure needed. */
int init();

/* Destory all the allocated data structures. */
int destory_all();

/* The left node of [index]. If it doesn't exist in matrix, -1 is returned. */
int left_node(int index);

/* The right node of [index]. If it doesn't exist in matrix, -1 is returned. */
int right_node(int index);

/* The top node of [index]. If it doesn't exist in matrix, -1 is returned. */
int top_node(int index);

/* The bottom node of [index]. If it doesn't exist in matrix, -1 is returned. */
int bottom_node(int index);

/* Create a node of color_block. */
//color_block * new_color_block();

/* Create color blocks for the matrix. */
color_block * create_color_blocks();

/* Destory color blocks for the calculation progress. */
int destory_color_blocks(color_block * blocks);

/* Initialize the color blocks, before all the calculation */
color_block * init_color_blocks();

/* Calculate a block, set the location bits and color, then return the number of nodes in the block */
int calculate_a_block(color_block * an_empty_block, int index);

/* dest |= src */
int bits_or_and_equ(int * dest, int * src);

/* For debug. Print the matrcx color blocks. */
int show_color_blocks(color_block * blocks);

/* Calculate the weight of the present matrix. */
int get_weight();

/* Calculate weight on the base of given vars. */
int calculate_weight(int blocks_number, int color_number);

/* Create color memory manager, the global varilable. Allocate space, set all the counters to 0. */
color_memory *  create_color_memory(int flag);

/* Destory color memory manager. Free the allocated space. */
int destory_color_memory(color_memory * color_memory_manager_local);

/* Increse the number of blocks of color. */
int color_mem_increse(color_memory * color_memory_manager_local, char color);

/* Decrese the number of blocks of color. */
int color_mem_decrese(color_memory * color_memory_manager_local, char color);

/* Copy color memory, from src to dest. */
int copy_color_mem(color_memory * dest, color_memory * src);

/* Return the counters of color int the color memory. */
int has_this_color(color_memory * color_memory_manager, char color);

/* For debug, print the color memory manager. */
int show_color_mem(color_memory * color_memory_manager);

/* Print all the global varilables in some format to show the running clearly. */
int show_all_global_vars();

/* Run one round, step forward. This is the main calculation progress of this program, at least now. */
int run_one_round();

/* Calculate the min weight coming from changing one block's color. Return the minimum weight, set the color changed to. */
int min_weight_of_one_block(color_block * color_block_pointer, char * color_pointer);

/* Do real change to the color blocks. Ready for the next round of running. */
int do_real_change(color_block * best_block, char best_color);

/* Calculate adjacent bit vector. */
int set_adjacent_nodes(int * bits_adjacency, int * bits);

/* Check if there are common "1"s in two bit vectors. */
int has_common_bits(int * bits1, int * bits2);

/* Delete block_to_delete from matrix_color_blocks, return the "next" of block_to_delete. */
color_block * delete_from_color_blocks(color_block * block_to_delete);

/* Destory one single block from matrix_color_blocks. Ignore the other ones and the next pointer. */
int destory_single_block(color_block * block_to_delete);

/* For debug. */
int show_mem_tatalvars();

#endif
