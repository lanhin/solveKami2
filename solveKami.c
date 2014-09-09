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

/* Print the matrix. For debug. */
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

/* bit vector operation, set [index] in bits to 1*/
int set_bits(int * bits, int index){
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

/* For debug. Print a bits vector. */
int show_bits(int * bits){
  for (int i = 0; i < (xlength * ylength); ++i){
    if (test_bits_single(bits, i))
      printf ("%d:", i);
    printf ("%d ", test_bits_single(bits, i));
  }
  putchar('\n');
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
int location_stack_push(int index){
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
  create_color_memory();

  color_kinds = 0;
  total_blocks = 0;
  total_colors = 0;

  blocks_weight = 1;
  colors_weight = 3;
  
  return 0;
}

/* Destory all the allocated data structures. */
int destory_all(){
  destory_matrix();
  destory_location_stack();
  destory_loc_bits(tmp_bits);
  destory_color_memory();
  return 0;
}

/* The left node of [index]. If it doesn't exist in matrix, -1 is returned. */
int left_node(int index){
  int left_one = index - 1;
  if (left_one < 0 || left_one >= (xlength * ylength))
    return -1;
  if ( ((left_one+1) % xlength) == 0) //index in the most left row
    return -1;
  return left_one;
}

/* The right node of [index]. If it doesn't exist in matrix, -1 is returned. */
int right_node(int index){
  int right_one = index + 1;
  if (right_one < 0 || right_one >= (xlength * ylength))
    return -1;
  if ( (right_one % xlength) == 0) //index in the most right row
    return -1;
  return right_one;
}

/* The top node of [index]. If it doesn't exist in matrix, -1 is returned. */
int top_node(int index){
  int top_one = index - xlength;
  if (top_one < 0 || top_one >= (xlength * ylength))
    return -1;
  return top_one;
}

/* The bottom node of [index]. If it doesn't exist in matrix, -1 is returned. */
int bottom_node(int index){
  int bottom_one = index + xlength;
  if (bottom_one < 0 || bottom_one >= (xlength * ylength))
    return -1;
  return bottom_one;
}

/* Create color blocks for the matrix. */
color_block *  create_color_blocks(){
  color_block * a_new_block = (color_block * )malloc(sizeof(color_block));
  if (a_new_block == NULL){
    printf ("ERROR: cannot allocate space for color block in create_color_blocks!\n");
    exit(1);
  }
  a_new_block -> location_bits = create_loc_bits();
  clrall_bits(a_new_block -> location_bits);
  a_new_block -> next = NULL;
  return a_new_block;
}

/* Destory color blocks for the calculation progress. */
int destory_color_blocks(color_block * blocks){
  if (blocks == NULL)
    return 0;
  
  destory_loc_bits(blocks -> location_bits);
  destory_color_blocks(blocks -> next);
  free(blocks);
  return 0;
}

/* Initialize the color blocks, before all the calculation */
color_block *  init_color_blocks(){
  int num_of_checked = 0;
  color_block * matrix_color_blocks = create_color_blocks();
  color_block * tail = matrix_color_blocks; //To store the tail node
  num_of_checked += calculate_a_block(tail, 0);
  total_blocks ++;
  while (num_of_checked < (xlength * ylength)){// calculate next color block
    bits_or_and_equ(tmp_bits_total, tail -> location_bits);
    tail -> next = create_color_blocks();
    tail = tail -> next;
    num_of_checked += calculate_a_block(tail, smallest_zero_index(tmp_bits_total));
    total_blocks ++;
  }

  return matrix_color_blocks;
  
}

/* Calculate a block, set the location bits and color, then return the number of nodes in the block */
int calculate_a_block(color_block * an_empty_block, int index){
  int number_of_nodes_in_this_block = 0;
  int node_index = index;
  clrall_bits(tmp_bits);
  an_empty_block -> color = matrix[index];
  set_bits(an_empty_block -> location_bits, node_index);
  set_bits(tmp_bits, node_index);
  number_of_nodes_in_this_block ++;

  /* Check the left, right, top and bottom node of the present one. */
  int left_one_node, right_one_node, top_one_node, bottom_one_node;
  left_one_node = left_node(node_index);
  right_one_node = right_node(node_index);
  top_one_node = top_node(node_index);
  bottom_one_node = bottom_node(node_index);
  /* The folllowing four "if"s deal with the check. */
  if ( (left_one_node != -1) && (matrix[left_one_node] == an_empty_block -> color)){
    if (DEBUG){
      printf("Left node identified");
    }
    location_stack_push(left_one_node);
    set_bits(tmp_bits, left_one_node);
  }
  if ( (right_one_node != -1) && (matrix[right_one_node] == an_empty_block -> color)){
    if (DEBUG){
      printf("Right node identified");
    }
    location_stack_push(right_one_node);
    set_bits(tmp_bits, right_one_node);
  }
  if ( (top_one_node != -1) && (matrix[top_one_node] == an_empty_block -> color)){
    if (DEBUG){
      printf("Top node identified");
    }
    location_stack_push(top_one_node);
    set_bits(tmp_bits, top_one_node);
  }
  if ( (bottom_one_node != -1) && (matrix[bottom_one_node] == an_empty_block -> color)){
    if (DEBUG){
      printf("Bottom node identified");
    }
    location_stack_push(bottom_one_node);
    set_bits(tmp_bits, bottom_one_node);
  }

  /* Deal with the nodes in the stack. */
  while (top > base){
    node_index = location_stack_pop();
    set_bits(an_empty_block -> location_bits, node_index);
    number_of_nodes_in_this_block ++;

    left_one_node = left_node(node_index);
    right_one_node = right_node(node_index);
    top_one_node = top_node(node_index);
    bottom_one_node = bottom_node(node_index);

    /* Check the nodes of four directions. */
    if ( (left_one_node != -1) && (matrix[left_one_node] == an_empty_block -> color) && !(test_bits_single(tmp_bits, left_one_node)) ){
    location_stack_push(left_one_node);
    set_bits(tmp_bits, left_one_node);
    }
    if ( (right_one_node != -1) && (matrix[right_one_node] == an_empty_block -> color) && !(test_bits_single(tmp_bits, right_one_node)) ){
    location_stack_push(right_one_node);
    set_bits(tmp_bits, right_one_node);
    }
    if ( (top_one_node != -1) && (matrix[top_one_node] == an_empty_block -> color) && !(test_bits_single(tmp_bits, top_one_node)) ){
    location_stack_push(top_one_node);
    set_bits(tmp_bits, top_one_node);
    }
    if ( (bottom_one_node != -1) && (matrix[bottom_one_node] == an_empty_block -> color) && !(test_bits_single(tmp_bits, bottom_one_node)) ){
    location_stack_push(bottom_one_node);
    set_bits(tmp_bits, bottom_one_node);
    }
  }
  return number_of_nodes_in_this_block;
}

/* dest |= src */
int bits_or_and_equ(int * dest, int * src){
  for (int i = 0; i < (xlength * ylength / BITSPERWORD + 1); i++){
    dest[i] |= src[i];
  }
  return 0;
}

/* For debug. Print the matrcx color blocks. */
int show_color_blocks(color_block * matrix_color_blocks){
  color_block * color_block_pointer = matrix_color_blocks;
  int i=0;
  do{
    i++;
    printf ("%d color: %c\n", i, color_block_pointer -> color);
    show_bits(color_block_pointer -> location_bits);
    color_block_pointer = color_block_pointer -> next;
  }while(color_block_pointer != NULL);
  return 0;
}

/* Calculate the weight of the present matrix. */
int get_weight(){
  return blocks_weight * total_blocks + colors_weight * total_colors;
}

/* Create color memory manager, the global varilable. Allocate space, set all the counters to 0. */
int create_color_memory(){
  return 0;
}

/* Destory color memory manager. Free the allocated space. */
int destory_color_memory(){
  return 0;
}

/* Increse the number of blocks of color. */
int color_mem_increse(char color){
  return 0;
}

/* Decrese the number of blocks of color. */
int color_mem_decrese(char color){
  return 0;
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

  if (DEBUG){
    show_matrix();
  }

  if(DEBUG){
    location_stack_push(0);
    location_stack_push(10);
    location_stack_push(53);
    location_stack_push(62);
    location_stack_push(74);
    location_stack_push(82);
    
    while (top != base){
      printf ("%d ", location_stack_pop());
    }
  }

  color_block * matrix_color_blocks = init_color_blocks();

  if (DEBUG){
    show_color_blocks(matrix_color_blocks);
  }
  
  destory_all();
  
  return 0;
}
