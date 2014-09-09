#include"solveKami.h"

/* Create matrix, to store color values*/
int create_matrix(){
    matrix = NULL;
    matrix = (char *)malloc((xlength * ylength + 1) * sizeof(char));
    if(matrix == NULL){
      printf("ERROR: cannot allocate space for matrix!\n");
      exit(1);
    }
    return 0;
}

/* full in the matrix with color */
int init_matrix(){
  char color;
  color = getchar();
  for(int i = 0; i < xlength*ylength; i++){
    //color = getchar();
    scanf("%c", &color);
    matrix[i] = color;
  }
  return 0;
}

/* print the matrix */
int show_matrix(){
  if (DEBUG){
    printf ("xlength = %d\n", xlength);
    printf ("ylength = %d\n", ylength);
  }
  for (int j = 0; j < ylength; j++){
    for (int i = 0; i< xlength; i++){
      putchar(matrix[j*xlength+i]);//Note: the mapping method is different from 2D array.
    }
    putchar('\n');
  }
  /*
  for (int i = 0; i < xlength*ylength; ++i){
    putchar(matrix[i]);
  }*/
}

/* Free the allocated space for matrix. To avoid memory leak. */
int destory_matrix(){
    free(matrix);
    if (DEBUG)
      printf("Matrix destoryed!\n");
    return 0;
}

/* Create location bit vector */
int * create_loc_bits(){
  int * bits = (int *)malloc((1 + xlength * ylength / BITSPERWORD) * sizeof(int));
  if (bits == NULL){
    printf ("ERROR: cannot locate space in create_loc_bits!\n");
    exit(1);
  }

  return bits;
}

/* Destory location bits, free the space. */
int destory_loc_bits(int * bits){
  free(bits);
  if (DEBUG)
    printf ("Location bits destoryed!\n");
  return 0;
}

/* bit vector operation, set [x,y] in bits to 1*/
int set_bits(int * bits, int x, int y){
  int index = y * xlength + x;
  bits[index >> SHIFT] |= (1 << (index & MASK));
  return 0;
}

/* bit vector operation, test [x,y] in bits */
int test_bits(int * bits, int x, int y){
  int index = y * xlength + x;
  return bits[index >> SHIFT] & (1 << (index & MASK));
}

/* bit vector operation, test [x] in bits  */
int test_bits_single(int * bits, int x){
  return bits[x >> SHIFT] & (1 << (x & MASK));
}

/* set all bits in bit vector to 0 */
int clrall_bits(int * bits){
  for (int i = 0; i < (xlength * ylength / BITSPERWORD + 1); i++){
    bits[i] = 0;
  }
  return 0;
}

/* Return the smallest zore's index in the location bit vector. */
int smallest_zero_index(int * bits){
  int smallest_index = 0;
  while( test_bits_single(bits, smallest_index)){
    smallest_index ++;
  }
  return smallest_index;
}

/* Get the color of location [x,y] int the matrix */
char color_of_matrix(int x, int y){
  int index = y * xlength + x;
  return matrix[index];
}

/* Create location stack, locate memory space for it. */
int create_location_stack(){
  location_stack = (int *)malloc((xlength * ylength + 1) * sizeof(int));
  if (location_stack == NULL){
    printf("ERROR: cannot locate space for location stack!\n");
    exit(1);
  }
  return 0;
}

/* Destory location stack, free the allocated space. */
int destory_location_stack(){
  free(location_stack);
  if (DEBUG)
    printf("Location stack destoryed!\n");
}

/* Push operation for location stack. */
int location_stack_push(int x, int y){
  int index = y * xlength + x;
  if (top < xlength * ylength){
    location_stack[++top] = index;
  }
  else{
    printf ("ERROR: too much elements in stack!\n");
    exit(1);
  }
}

/* Pop operation for locatin stack. */
int location_stack_pop(){
  if (top == base){
    printf ("ERROR: no element left in stack, but there's a pop!\n");
    exit(1);
  }
  return location_stack[top--];
}

/* Initialize all the data structure needed. */
int init(){
  create_matrix();
  create_location_stack();
  tmp_bits = create_loc_bits();
  clrall_bits(tmp_bits);
  tmp_bits_total = create_loc_bits();
  clrall_bits(tmp_bits_total);
  return 0;
}

/* Destory all the allocated data structures. */
int destory_all(){
  destory_matrix();
  destory_location_stack();
  destory_loc_bits(tmp_bits);
  return 0;
}

/* The left node of [x,y]. If it doesn't exist in matrix, -1 is returned. */
int left_node(int x, int y){
  return (y * xlength + x - 1);
}

/* The right node of [x,y]. If it doesn't exist in matrix, -1 is returned. */
int right_node(int x, int y){
  return (y * xlength + x + 1);
}

/* The top node of [x,y]. If it doesn't exist in matrix, -1 is returned. */
int top_node(int x, int y){
  return ((y-1) * xlength + x);
}

/* The bottom node of [x,y]. If it doesn't exist in matrix, -1 is returned. */
int bottom_node(int x, int y){
  return ((y+1) * xlength + x);
}

/* Create color blocks for the matrix. */
color_block *  create_color_blocks(){
  color_block * a_new_block = (color_block * )malloc(sizeof(color_block));
  if (a_new_block == NULL){
    printf ("ERROR: cannot allocate space for color block in create_color_blocks!\n");
    exit(1);
  }
  a_new_block->location_bits = create_loc_bits();
  clrall_bits(a_new_block->location_bits);
  a_new_block->next = NULL;
  return a_new_block;
}

/* Destory color blocks for the calculation progress. */
int destory_color_blocks(color_block * blocks){
  if (blocks == NULL)
    return 0;
  
  destory_loc_bits(blocks->location_bits);
  destory_color_blocks(blocks->next);
  free(blocks);
  return 0;
}

/* Initialize the color blocks, before all the calculation */
color_block *  init_color_blocks(){
  int num_of_checked = 0;
  color_block * matrix_color_blocks = create_color_blocks();
  color_block * tail = matrix_color_blocks; //To store the tail node
  num_of_checked += calculate_a_block(tail, 0);
  while (num_of_checked < (xlength * ylength)){// calculate next color block
    bits_or_and_equ(tmp_bits_total, tail->location_bits);
    tail->next = create_color_blocks();
    tail = tail->next;
    num_of_checked += calculate_a_block(tail, smallest_zero_index(tmp_bits_total));
  }

  return matrix_color_blocks;
  
}

/* Calculate a block, set the location bits and color, then return the number of nodes in the block */
int calculate_a_block(color_block * an_empty_block, int index){
  
}

/* dest |= src */
int bits_or_and_equ(int * dest, int * src){
}

/* Run one round, step forward. This is the main calculation progress of this program, at least now. */
int run_one_round(){
}

int main(int argc, char * argv[]){
  printf ("xlength:\n");
  scanf("%d", &xlength);
  printf ("ylength:\n");
  scanf("%d", &ylength);

  if (DEBUG){
    printf ("xlength: %d\n", xlength);
    printf ("ylength: %d\n", ylength);
  }

  //create_matrix();
  init();
  
  init_matrix();

  show_matrix();

  if(DEBUG){
    location_stack_push(0,0);
    location_stack_push(0,1);
    location_stack_push(3,5);
    location_stack_push(2,6);
    location_stack_push(4,7);
    location_stack_push(2,8);
    
    while (top != base){
      printf ("%d ", location_stack_pop());
    }
  }

  destory_all();
  
  return 0;
}
