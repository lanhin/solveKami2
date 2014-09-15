/* Author: Lin Han
 * 
 * lanhin1@gmail.com
 *
 * CS09@USTC
 *
 */

#include"solveKami.h"

/* Create matrix, to store color values*/
char * create_matrix(){
    char * matrix = NULL;
    matrix = (char *)malloc((xlength * ylength + 1) * sizeof(char));
    if(matrix == NULL){
      printf("ERROR: cannot allocate space for matrix!\n");
      exit(1);
    }
    return matrix;
}

/* full in the matrix with color */
int init_matrix(){
  char color;
  color = getchar();// Get the '\n' and throw it.
  for(int i = 0; i < xlength*ylength; i++){
    //color = getchar();
    scanf("%c", &color);
    matrix[i] = color;
  }
  return 0;
}

/* Copy matrix from src to dest. */
int copy_matrix(char * dest, char * src){
  for (int i = 0; i <= (xlength * ylength); i++){
    dest[i] = src[i];
  }
}

/* Print the matrix. For debug. */
int show_matrix(char * matrix){
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
int destory_matrix(char * matrix){
    free(matrix);
    #if 0
    if (DEBUG)
      printf("Matrix destoryed!\n");
    #endif
    return 0;
}

/* Create calc_vector and set all the elements in it to 0. */
int * create_calc_vector(){
  int * new_calc_vector = NULL;
  new_calc_vector = (int *)malloc(2 * steps_forward * sizeof(int));
  if (new_calc_vector == NULL){
    printf ("ERROR: cannot allocate space for calc_vector!\n");
    exit(1);
  }
  for (int i=0; i < 2*steps_forward; i++)
    new_calc_vector[i] = 0;
  return new_calc_vector;
}

/* Destory calc_vector. */
int destory_calc_vector(int * calc_vector){
  free (calc_vector);
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
  #if 0
  if (DEBUG)
    printf ("Location bits destoryed!\n");
  #endif
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
  while( test_bits_single(bits, smallest_index) ){
    smallest_index ++;
  }
  return smallest_index;
}

/* Return the smallest one's index in the location bit vector. */
int smallest_one_index(int * bits){
  int smallest_index = 0;
  while( !test_bits_single(bits, smallest_index) ){
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
  #if 0
  if (DEBUG)
    printf("Location stack destoryed!\n");
  #endif
  return 0;
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
  return 0;
}

/* Pop operation for locatin stack. */
int location_stack_pop(){
  if (top == base){
    printf ("ERROR: no element left in stack, but there's a pop!\n");
    exit(1);
  }
  return location_stack[top--];
}

/* Initialize all the data structures needed. */
int init(){
  matrix = create_matrix();
  matrix_in_calc = create_matrix();
  
  create_location_stack();
  
  calc_vector = create_calc_vector();
  calc_vector_limit = create_calc_vector();
  best_vector = create_calc_vector();
  
  tmp_bits = create_loc_bits();//used in calculate_a_block()
  clrall_bits(tmp_bits);
  
  tmp_bits_total = create_loc_bits();//used in init_color_blocks()
  clrall_bits(tmp_bits_total);
  
  //tmp_bits_adjacency = create_loc_bits();
  //clrall_bits(tmp_bits_adjacency);
    
  color_memory_manager = create_color_memory(1);
  //tmp_color_memory = create_color_memory(0);

  //color_kinds = 0;
  total_blocks = 0;
  total_colors = color_kinds;
  step_num = 0;

  //blocks_weight = 1;
  //colors_weight = 3;

  last_vector_flag = 0;

  return 0;
}

/* Destory all the allocated data structures. */
int destory_all(){
  destory_matrix(matrix);
  destory_matrix(matrix_in_calc);
  
  destory_location_stack();

  destory_calc_vector(calc_vector);
  destory_calc_vector(calc_vector_limit);
  destory_calc_vector(best_vector);
  
  destory_loc_bits(tmp_bits);
  destory_loc_bits(tmp_bits_total);
  //destory_loc_bits(tmp_bits_adjacency);
  
  destory_color_memory(color_memory_manager);
  //destory_color_memory(tmp_color_memory);
  destory_color_blocks(matrix_color_blocks);
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
color_block *  new_color_block(){
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

/* Calculate the color blocks and color_memory_manager. Set total_blocks. */
color_block *  calculate_color_blocks(char * matrix){
  int num_of_checked = 0;
  total_blocks = 0;
  reset_color_mem(color_memory_manager);
  clrall_bits(tmp_bits_total);
  matrix_color_blocks = new_color_block();
  color_block * tail = matrix_color_blocks; //To store the tail node
  
  num_of_checked += calculate_a_block(matrix, tail, 0);
  total_blocks ++;
  
  while (num_of_checked < (xlength * ylength)){// calculate next color block
    bits_or_and_equ(tmp_bits_total, tail -> location_bits);
    tail -> next = new_color_block();
    tail = tail -> next;
    num_of_checked += calculate_a_block(matrix, tail, smallest_zero_index(tmp_bits_total));
    total_blocks ++;
  }
  
  return matrix_color_blocks;
}

/* Calculate a block, set the location bits and color, then return the number of nodes in the block */
int calculate_a_block(char * matrix, color_block * an_empty_block, int index){
  int number_of_nodes_in_this_block = 0;
  int node_index = index;
  clrall_bits(tmp_bits);
  an_empty_block -> color = matrix[index];
  color_mem_increse(color_memory_manager, an_empty_block -> color);
 
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
    #if 0
    if (DEBUG){
      printf("Left node identified");
    }
    #endif
    location_stack_push(left_one_node);
    set_bits(tmp_bits, left_one_node);
  }
  if ( (right_one_node != -1) && (matrix[right_one_node] == an_empty_block -> color)){
    #if 0
    if (DEBUG){
      printf("Right node identified");
    }
    #endif
    location_stack_push(right_one_node);
    set_bits(tmp_bits, right_one_node);
  }
  if ( (top_one_node != -1) && (matrix[top_one_node] == an_empty_block -> color)){
    #if 0
    if (DEBUG){
      printf("Top node identified");
    }
    #endif
    location_stack_push(top_one_node);
    set_bits(tmp_bits, top_one_node);
  }
  if ( (bottom_one_node != -1) && (matrix[bottom_one_node] == an_empty_block -> color)){
    #if 0
    if (DEBUG){
      printf("Bottom node identified");
    }
    #endif
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
  total_colors = get_total_colors();
  return blocks_weight * total_blocks + colors_weight * total_colors;
}

/* Calculate weight on the base of given vars. */
int calculate_weight(int blocks_number, int color_number){
  return blocks_weight * blocks_number + colors_weight * color_number;
}

/* Create color memory manager, the global varilable. Allocate space, set all the counters to 0. */
color_memory *  create_color_memory(int flag){
  /* flag: whether to init the color memory. */
  color_memory * color_memory_manager_local = NULL;
  color_memory_manager_local = (color_memory *)malloc(sizeof(color_memory));
  if (color_memory_manager_local == NULL){
    printf ("ERROR: cannot allocate space for color memory manager!\n");
    exit(1);
  }
  
  color_memory_manager_local -> all_the_colors = NULL;
  color_memory_manager_local -> all_the_colors = (char *)malloc(color_kinds * sizeof(char));
  if (color_memory_manager_local -> all_the_colors == NULL){
    printf ("ERROR: cannot allocate space for color_memory_manager -> all_the_colors!\n");
    exit(1);
  }

  color_memory_manager_local -> counters_of_colors = NULL;
  color_memory_manager_local -> counters_of_colors = (int *)malloc(color_kinds * sizeof(int));
  if (color_memory_manager_local -> counters_of_colors == NULL){
    printf ("ERROR: cannot allocate space for color_memory_manager.counters_of_colors!\n");
    exit(1);
  }

  if ( !flag )
    return color_memory_manager_local;
  
  getchar();// Get the '\n' and throw it.
  char color;
  for (int i = 0; i < color_kinds; i++){
    color = getchar();
    color_memory_manager_local -> all_the_colors[i] = color;
    color_memory_manager_local -> counters_of_colors[i] = 0;

    if(DEBUG){
      printf ("%dth member in all_the_colors:%c  counter:%d\n", i, color_memory_manager_local -> all_the_colors[i], color_memory_manager_local -> counters_of_colors[i]);
    }
  }

  return color_memory_manager_local;
}

/* Destory color memory manager. Free the allocated space. */
int destory_color_memory(color_memory * color_memory_manager_local){
  free(color_memory_manager_local -> all_the_colors);
  free(color_memory_manager_local -> counters_of_colors);
  #if 0
  if (DEBUG){
    printf ("Color memory manager destoryed!\n");
  }
  #endif
  return 0;
}

/* Reset the counters in the color memory.  */
int reset_color_mem(color_memory * color_memory_manager){
  for (int i=0; i < color_kinds; i++){
    color_memory_manager -> counters_of_colors[i] = 0;
  }
}

/* Increse the number of blocks of color. */
int color_mem_increse(color_memory * color_memory_manager_local, char color){
  for (int i = 0; i < color_kinds; i++){
    if (color_memory_manager_local -> all_the_colors[i] == color){
      color_memory_manager_local -> counters_of_colors[i] ++;
    }
  }
  return 0;
}

/* Decrese the number of blocks of color. */
int color_mem_decrese(color_memory * color_memory_manager_local, char color){
  for (int i = 0; i < color_kinds; i++){
    if (color_memory_manager_local -> all_the_colors[i] == color){
      if (color_memory_manager_local -> counters_of_colors[i] <= 0){
	printf ("ERROR: color_mem_decrese failed! The counter is 0!\n");
	exit(1);
      }
      color_memory_manager_local -> counters_of_colors[i] --;
    }
  }
  return 0;
}

/* For debug, print the color memory manager. */
int show_color_mem(color_memory * color_memory_manager){
  for (int i = 0; i < color_kinds; i++){
    printf ("Color %c: %d\n", color_memory_manager -> all_the_colors[i], color_memory_manager -> counters_of_colors[i]);
  }
  return 0;
}

/* Print all the global varilables in some format to show the running clearly. */
int show_all_global_vars(){
  return 0;
}

/* Run one round, step forward. This is the main calculation progress of this program, at least now. */
int run_one_round(){
  weight = get_weight();
  step_num ++;

  int min_weight = weight;
  int next_weight;
  color_block * color_block_pointer = matrix_color_blocks;
  color_block * best_block = color_block_pointer;
  char best_color;
  char * color_pointer = (char *)malloc(sizeof(char));
  *color_pointer = '1';//For debug
  if (color_pointer == NULL){
    printf ("ERROR: cannot allocate space for best_color!\n");
    exit(1);
  }

  while( color_block_pointer != NULL ){
    next_weight = min_weight_of_one_block(color_block_pointer, color_pointer);
    if (next_weight < min_weight){
      min_weight = next_weight;
      best_color = *color_pointer;
      best_block = color_block_pointer;
    }
    color_block_pointer = color_block_pointer -> next;
  }

  int index = smallest_one_index(best_block -> location_bits);
  printf ("Step %d:location: %d, %d. Color: %c \n", step_num, index % xlength, index / xlength, best_color);
  do_real_change(best_block, best_color);//change the color blocks

  #if 0
  if (DEBUG){
    show_color_blocks(matrix_color_blocks);
    show_color_mem(color_memory_manager);
  }
  #endif
  
  return 0;
}

/* Calculate the min weight coming from changing one block's color. Return the minimum weight, set the color changed to. */
int min_weight_of_one_block(color_block * color_block_pointer, char * color_pointer){
  int total_blocks_changed = total_blocks;
  int total_colors_changed = total_colors;
  int min_weight_founded = weight;
  char color_to_change;
  color_block * to_check_pointer;

  copy_color_mem(tmp_color_memory, color_memory_manager);
  
  color_mem_decrese(tmp_color_memory, color_block_pointer -> color);
  if ( !has_this_color(tmp_color_memory, color_block_pointer -> color) ){
    total_colors_changed --;
  }
  
  /* Calculate the adjacent bit vector of color_block_pointer's. */
  clrall_bits(tmp_bits_adjacency);
  set_adjacent_nodes(tmp_bits_adjacency, color_block_pointer -> location_bits);
  
  for ( int i = 0; i < color_kinds; i++){
    color_to_change = color_memory_manager -> all_the_colors[i];
    if ( color_to_change == (color_block_pointer ->color)  )
      continue;
    total_blocks_changed = total_blocks;
    
    //color_mem_increse(tmp_color_memory, color_to_change);
    /* Calculate the number of blocks after change. */
    to_check_pointer = matrix_color_blocks;
    while( to_check_pointer != NULL ){
      if ( to_check_pointer == color_block_pointer ){
	to_check_pointer = to_check_pointer -> next;
	continue;
      }
      
      if ( (has_common_bits(tmp_bits_adjacency, to_check_pointer -> location_bits)) && (to_check_pointer -> color == color_to_change) )
	total_blocks_changed --;
      
      to_check_pointer = to_check_pointer -> next;
    }
    //color_mem_decrese(tmp_color_memory, color_to_change);

    if( (min_weight_founded) > (calculate_weight(total_blocks_changed, total_colors_changed)) ){
      min_weight_founded =  calculate_weight(total_blocks_changed, total_colors_changed);
      (*color_pointer) = color_to_change;
    }
    
  }

  return min_weight_founded;
}

/* Do real change to the color blocks. Ready for the next round of running. */
int do_real_change(color_block * best_block, char best_color){
  /* Calculate the adjacent bit vector of color_block_pointer's. */
  #if 0
  if (DEBUG){
    printf ("do_real_change start...\n");
    printf ("best color: %c  best_block:\n", best_color);
    show_bits(best_block -> location_bits);
  }
  #endif
  
  clrall_bits(tmp_bits_adjacency);
  set_adjacent_nodes(tmp_bits_adjacency, best_block -> location_bits);

  color_mem_decrese(color_memory_manager, best_block -> color);
  if ( !has_this_color(color_memory_manager, best_block -> color) ){
    total_colors --;
  }

  best_block -> color = best_color;
  color_mem_increse(color_memory_manager, best_color);

  /* Calculate the number of blocks after change. */
  color_block * to_check_pointer = matrix_color_blocks;
  while( to_check_pointer != NULL ){
    if ( to_check_pointer == best_block ){
      to_check_pointer = to_check_pointer -> next;
      continue;
    }
    
    if ( (has_common_bits(tmp_bits_adjacency, to_check_pointer -> location_bits)) && (to_check_pointer -> color == best_color) ){
      #if 0
      if (DEBUG){
	printf ("common bits with: ");
	show_bits(to_check_pointer -> location_bits);
      }
      #endif
      total_blocks --;
      color_mem_decrese(color_memory_manager, best_color);
      
      bits_or_and_equ(best_block -> location_bits, to_check_pointer -> location_bits);
      to_check_pointer = delete_from_color_blocks(to_check_pointer);
      continue;
    }
    
    to_check_pointer = to_check_pointer -> next;
  }

}

/* Delete block_to_delete from matrix_color_blocks, return the "next" of block_to_delete. */
color_block * delete_from_color_blocks(color_block * block_to_delete){
  if (block_to_delete == NULL)
    return NULL;
  if (block_to_delete == matrix_color_blocks){
    matrix_color_blocks = matrix_color_blocks -> next;
    destory_single_block(block_to_delete);
    return matrix_color_blocks;
  }

  /* An ordinary block, not NULL, not matrix_color_blocks. */
  color_block * pre_pointer = matrix_color_blocks;
  while ( (pre_pointer -> next) != block_to_delete )
    pre_pointer = pre_pointer -> next;

  pre_pointer -> next = block_to_delete -> next;
  destory_single_block(block_to_delete);
  return pre_pointer -> next;
}

/* Destory one single block from matrix_color_blocks. Ignore the other ones and the next pointer. */
int destory_single_block(color_block * block_to_delete){
  if (block_to_delete == NULL)
    return 0;

  destory_loc_bits(block_to_delete -> location_bits);
  free (block_to_delete);
}

/* Calculate adjacent bit vector. */
int set_adjacent_nodes(int * bits_adjacency, int * bits){
  for ( int i=0; i < (xlength * ylength); i++ ){
    if (test_bits_single(bits, i)){
      if (left_node(i) != -1)
	set_bits(bits_adjacency, left_node(i));
      if (right_node(i) != -1)
	set_bits(bits_adjacency, right_node(i));
      if (top_node(i) != -1)
	set_bits(bits_adjacency, top_node(i));
      if (bottom_node(i) != -1)
	set_bits(bits_adjacency, bottom_node(i));
    }
  }
  return 0;
}

/* Check if there are common "1"s in two bit vectors. */
int has_common_bits(int * bits1, int * bits2){
  /* The length of the location bits is 1+xlength*ylength/BITSPERWORD. */
  for ( int i=0; i <= xlength * ylength / BITSPERWORD; i++){
    if (bits1[i] & bits2[i])
      return 1;
  }
  return 0;
}

/* Copy color memory, from src to dest. */
int copy_color_mem(color_memory * dest, color_memory * src){
  /* The color memory dest and src are both created already. */
  for (int i=0; i < color_kinds; i++){
    dest -> all_the_colors[i] = src -> all_the_colors[i];
    dest -> counters_of_colors[i] = src -> counters_of_colors[i];
  }
  return 0;
}

/* Return the counters of color int the color memory. */
int has_this_color(color_memory * color_memory_manager, char color){
  for (int i=0; i < color_kinds; i++){
    if (color_memory_manager -> all_the_colors[i] == color)
      return color_memory_manager -> counters_of_colors[i];
  }

  printf ("ERROR: has_this_color() failed! Cannot find color %c in the given color_memory.\n", color);
  exit(1);
}

/* For debug. */
int show_mem_tatalvars(){
  if (DEBUG){
    show_color_mem(color_memory_manager);
    printf ("total blocks: %d\n", total_blocks);
    printf ("total colors: %d\n", total_colors);
  }
}

/* Set total_blocks and color_kinds in the limit vector. */
int set_limit_vector(int step_num){
  calc_vector_limit[2*step_num] = total_blocks;
  calc_vector_limit[2*step_num+1] = color_kinds;

  return 0;
}

/* Change the matrix's nodes' color. */
int do_change_to_matrix(char * matrix, int block_index, int color_index){
  color_block * color_block_pointer = matrix_color_blocks;
  for (int i=0; i < block_index; i++)
    color_block_pointer = color_block_pointer -> next;

  char color_to_change = color_memory_manager -> all_the_colors[color_index];

  for (int i=0; i < (xlength * ylength); i++){
    if ( test_bits_single (color_block_pointer -> location_bits, i) )
      matrix[i] = color_to_change;
  }
  
  return 0;
}

/* Print change info. */
int footprint(int block_index, int color_index){
  color_block * color_block_pointer = matrix_color_blocks;
  int matrix_index;
  for (int i=0; i < block_index; i++)
    color_block_pointer = color_block_pointer -> next;

  char color_to_change = color_memory_manager -> all_the_colors[color_index];
  
  for (int i=0; i < (xlength * ylength); i++){
    if ( test_bits_single(color_block_pointer -> location_bits, i) ){
      matrix_index = i;
      break;
    }
  }

  printf ("Step %d: %d, %d  color: %c\n", steps_forward * forward_round + step_num, matrix_index % xlength, matrix_index / xlength, color_to_change);
  return 0;
}

/* Calculate the total colors, according to color_memory_manager. */
int get_total_colors(){
  int number_of_colors = 0;

  for (int i=0; i < color_kinds; i++){
    if (color_memory_manager -> counters_of_colors[i] > 0)
      number_of_colors ++;
  }
  
  return number_of_colors;
}

/* Copy vector from src to dest. Their length are both 2*steps_forward. */
int copy_vector(int * dest, int * src){
  for ( int i=0; i < 2 * steps_forward; i++ ){
    dest[i] = src[i];
  }
  return 0;
}

/* Check the last_vector_flag. If it's 1, set it to 0 and return 1. 0 would be returned in other cases. */
int this_is_the_last_vector(){
  if (last_vector_flag){
    last_vector_flag = 0;
    return 1;
  }

  return 0;
}

/* Set the calc_vector to the next value. When overflow happens, the calc_vector would be set to all 0s and last_vector_flag would be set to 1. */
int increse_vector(int index){
  if (index < 0){//overflow
    last_vector_flag = 1;
    for (int i=0; i < 2 * steps_forward; i++)
      calc_vector[i] = 0;

    return 0;
  }

  if (DEBUG && !index){
    printf ("a little part more...\n");
  }
  //index >= 0
  if ((calc_vector[index] + 1) == calc_vector_limit[index]){
    calc_vector[index] = 0;
    increse_vector(index - 1);
  }else{
    calc_vector[index] ++;
  }

  return 0;
}

/* For debug. */
int show_calc_vector(int * vector){
  for (int i=0; i<2*steps_forward; i++){
    printf ("%d ", vector[i]);
  }
  putchar('\n');
}

int main(int argc, char * argv[]){
  printf ("xlength:\n");
  scanf("%d", &xlength);
  printf ("ylength:\n");
  scanf("%d", &ylength);
  printf ("kinds of color:\n");
  scanf("%d", &color_kinds);
  printf ("step forward:\n");
  scanf("%d", &steps_forward);
  printf ("colors weight:\n");
  scanf("%d", &colors_weight);
  printf ("blocks weight:\n");
  scanf("%d", &blocks_weight);

  if (DEBUG){
    printf ("xlength: %d\n", xlength);
    printf ("ylength: %d\n", ylength);
    printf ("kinds of color: %d\n", color_kinds);
    printf ("step forward: %d\n", steps_forward);
  }

  //  steps_forward = 2;
  init();
  
  init_matrix();

  #if 0
  if (DEBUG){
    show_matrix();
  }
  #endif

  matrix_color_blocks = calculate_color_blocks(matrix);
  min_weight = get_weight();
  
  best_steps_found = NOT_VALID_STEPS;
  int success_flag = 0;
  int ahead_steps = steps_forward;
  forward_round = 0;
  while(!success_flag){
  do{
      
      for (step_num = 0; step_num < steps_forward; step_num++){
	//look forward for steps_forward steps
	if (step_num == 0){
	  copy_matrix(matrix_in_calc, matrix);

	  destory_color_blocks(matrix_color_blocks);
	  matrix_color_blocks = calculate_color_blocks(matrix_in_calc);	  
	}
	block_index = calc_vector[2*step_num];
	color_index = calc_vector[2*step_num+1];
	 
	set_limit_vector(step_num);
	do_change_to_matrix(matrix_in_calc, block_index, color_index);

	destory_color_blocks(matrix_color_blocks);
	matrix_color_blocks = calculate_color_blocks(matrix_in_calc);
	
	#if 0
	if (DEBUG){
	  printf ("matrix:\n");
	  show_matrix(matrix);
	  printf ("matrix_in_calc:\n");
	  show_matrix(matrix_in_calc);
	}
	#endif
	
	if (get_total_colors() == 1){
	  if (step_num < best_steps_found){
	    best_steps_found = step_num;
	    copy_vector(best_vector, calc_vector);
	  }
	  break;
	}
      }
      
      weight = get_weight();
      if ( (weight < min_weight) && (best_steps_found == NOT_VALID_STEPS) ){
	copy_vector(best_vector, calc_vector);
	min_weight = weight;
      }
      
      increse_vector(2 * steps_forward - 1);
      
  }while( !this_is_the_last_vector());//last vector means it's overflow

  if (DEBUG){
    printf ("best_steps_found: %d\n", best_steps_found);
    printf ("min_weight: %d\n", min_weight);
  }
    #if 1
    //then this is the last vector. all the strps_forward steps has finished
    if (best_steps_found != NOT_VALID_STEPS){//a good solution found
      success_flag = 1;
      ahead_steps = best_steps_found+1;
    }
    for (step_num = 0; step_num < ahead_steps; step_num++){
      block_index = best_vector[2*step_num];
      color_index = best_vector[2*step_num+1];
    
      destory_color_blocks(matrix_color_blocks);
    
      matrix_color_blocks = calculate_color_blocks(matrix);
    
      do_change_to_matrix(matrix, block_index, color_index);
      footprint(block_index, color_index);
    }
    forward_round ++;
  }
  #endif
  
#if 0
  if (DEBUG){
    show_color_blocks(matrix_color_blocks);
    show_calc_vector(best_vector);
  }
#endif

  #if 0
  if (DEBUG){
    show_color_mem(color_memory_manager);
    printf ("total blocks: %d\n", total_blocks);
    printf ("total colors: %d\n", total_colors);
  }
  #endif

  #if 0
  while ( total_colors > 1 ){
    run_one_round();
  }

  printf ("fjaioej\n");
  show_mem_tatalvars();
  #endif

  
  destory_all();

  printf ("******Successed!******\n");
  
  return 0;
}
