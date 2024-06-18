#include <stdmem.h>
#include <unistd.h>
// I have decided that the correct implementation for a buddy system's memory
// map is a binary tree Of course, since a binary tree is traditionally just a
// two-tailed list there are two options Use a naïve malloc to manage this
// binary tree

// Or implement as
// An array binary tree
// Like barbarian

// The latter is a
// Haiku. I will do that now
// Enjoy this journey

#define HEAD_SIZE_M 2

#define IS_BIT_SET(x, i) (((x)[(i) >> 3] & (1 << ((i)&7))) != 0)
#define SET_BIT(x, i) (x)[(i) >> 3] |= (1 << ((i)&7))
#define CLEAR_BIT(x, i) (x)[(i) >> 3] &= (1 << ((i)&7)) ^ 0xFF

#define IS_LEFT(i) ((i) % 2)
#define GET_LEFT(i) (((i) << 1) + 1)
#define GET_RIGHT(i) (((i) << 1) + 2)
#define GET_PARENT(i) (((i)-1) >> 1)
#define GET_BROTHER(i)                                                         \
  (IS_LEFT(i) ? GET_RIGHT(GET_PARENT(i)) : GET_LEFT(GET_PARENT(i)))

#define MAP_START_M (((char *)sbrk(0)) + MEM_SIZE_M)
#define MAP_END_M (((char *)sbrk(0)) + HEAP_SIZE_M)
#define MEM_START_M ((char *)sbrk(0))
#define MEM_END_M (((char *)sbrk(0)) + MEM_SIZE_M)

typedef enum states_m { EMPTY = 0, SPLIT, FULL, ALLOCATED } states_m;


void malloc_init() {
  // Initialize binary tree
  for (char *i = MAP_START_M; i < MAP_END_M; i++)
    *((uint64_t *)i) = EMPTY;
}

void cascade_state(char *x, size_t_m i, states_m s);

size_t_m round_to_power_of_two(size_t_m s) {
  if (!s)
    return 0;
  int i = 1;
  while ((i << 1) <= s)
    i <<= 1;
  return i;
}

void set_node_state(char *x, size_t_m i, states_m s) {
  size_t_m bit_index = (i << 1);
  switch (s) {
  case EMPTY:
    CLEAR_BIT(x, bit_index);
    CLEAR_BIT(x, bit_index + 1);
    break;
  case SPLIT:
    CLEAR_BIT(x, bit_index);
    SET_BIT(x, bit_index + 1);
    break;
  case FULL:
    SET_BIT(x, bit_index);
    CLEAR_BIT(x, bit_index + 1);
    break;
  case ALLOCATED:
    SET_BIT(x, bit_index);
    SET_BIT(x, bit_index + 1);
    break;
  default:
    break;
  }
  // 0 index is root node, does not cascade
  if (i)
    cascade_state(x, i, s);
}

states_m read_state(char *x, size_t_m i) {
  if (IS_BIT_SET(x, i << 1)) {
    if (IS_BIT_SET(x, (i << 1) + 1))
      return ALLOCATED;
    return FULL;
  }
  if (IS_BIT_SET(x, (i << 1) + 1)) {
    return SPLIT;
  }
  return EMPTY;
}

void cascade_state(char *x, size_t_m i, states_m s) {
  states_m aux;
  switch (s) {
  case EMPTY:
    aux = read_state(x, GET_PARENT(i));
    if (aux == FULL)
      set_node_state(x, GET_PARENT(i), SPLIT);
    else if (aux == SPLIT && read_state(x, GET_BROTHER(i)) == EMPTY)
      set_node_state(x, GET_PARENT(i), EMPTY);
    break;
  case SPLIT:
    if (read_state(x, GET_PARENT(i)) != SPLIT)
      set_node_state(x, GET_PARENT(i), SPLIT);
    break;
  case FULL:
  case ALLOCATED:
    aux = read_state(x, GET_PARENT(i));
    if (aux == EMPTY)
      set_node_state(x, GET_PARENT(i), SPLIT);
    else if (aux == SPLIT && (read_state(x, GET_BROTHER(i)) == FULL ||
                              read_state(x, GET_BROTHER(i)) == ALLOCATED))
      set_node_state(x, GET_PARENT(i), FULL);
    break;
  default:
    break;
  }
}

size_t_m height(size_t_m index) {
  size_t_m ans = 0;
  while (index) {
    ans++;
    index >>= 1;
  }
  return ans;
}

/**
 * @brief returns the size in bytes of the memory allocation represented by the
 * node located at this index in the memory map
 */
size_t_m mem_size(size_t_m index) { return (MEM_SIZE_M) >> height(index); }

size_t_m map_index_to_mem_index(size_t_m index) {
  size_t_m result = 0;
  size_t_m jump = mem_size(index);
  while (index > 0) {
    if (!IS_LEFT(index))
      result += jump;
    index = GET_PARENT(index);
    jump <<= 1;
  }
  return result;
}

size_t_m mem_index_to_map_index(size_t_m index) {
  size_t_m probe = 0;
  size_t_m jump = (MEM_SIZE_M) >> 2;
  size_t_m map_index = 0;
  while (probe < index && jump > 0) {
    if (probe + jump <= index) {
      probe += jump;
      map_index = GET_RIGHT(map_index);
    } else {
      map_index = GET_LEFT(map_index);
    }
    jump >>= 1;
  }

  if (!jump) {
    return map_index;
  }
  while (read_state(MAP_START_M, map_index) != ALLOCATED) {
    map_index = GET_LEFT(map_index);
  }
  return map_index;
}

size_t_m find_buddy(size_t_m size, size_t_m index, size_t_m current_size);

void *malloc(size_t_m size) {
  if (size > MEM_SIZE_M)
    return NULL;
  size_t_m index = find_buddy(size * 2, 0, MEM_SIZE_M);
  if (index == -1) {
    return NULL;
  }
  return (void *)(MEM_START_M + map_index_to_mem_index(index));
}

/**
 * @brief dfs algorithm, searches for a node that represents the smallest unit
 * of memory that can fit the request
 */
size_t_m find_buddy(size_t_m size, size_t_m index, size_t_m current_size) {
  // if does not fit in the space denoted by the node, do not look further
  if (size > current_size) {
    return -1;
  }
  // if does not fit in half the space of the node, you are in a smallest node
  // that can fit it If current_size is smaller than BLOCK_M, you have reached
  // minimum allowed memory unit
  if (size > current_size / 2 || current_size / 2 <= BLOCK_M) {
    if (read_state(MAP_START_M, index) == EMPTY) {
      set_node_state(MAP_START_M, index, ALLOCATED);
      return index;
    }
    return -1;
  }
  // If a node is full, the children are considered full as well
  if (read_state(MAP_START_M, index) == FULL ||
      read_state(MAP_START_M, index) == ALLOCATED) {
    return -1;
  }
  // dfs recursion: search left, if not found, return right search
  size_t_m left = find_buddy(size, GET_LEFT(index), current_size / 2);
  if (left == -1) {
    return find_buddy(size, GET_RIGHT(index), current_size / 2);
  }
  return left;
}

void *realloc(void *ptr, size_t_m size) {
  free(ptr);
  return malloc(size);
}

void free(void *ptr) {
  if (((char *)ptr) >= MEM_START_M && ((char *)ptr) <= MEM_END_M)
    set_node_state(MAP_START_M,
                   mem_index_to_map_index(((char *)ptr) - MEM_START_M), EMPTY);
}
