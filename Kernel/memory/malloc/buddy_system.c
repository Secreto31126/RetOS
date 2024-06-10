// I have decided that the correct implementation for a buddy system's memory map is a binary tree
// Of course, since a binary tree is traditionally just a two-tailed list there are two options
// Use a naïve malloc to manage this binary tree

// Or implement it
// As a tree in an array
// Like barbarian

// The latter is a
// Haiku. I will do that now
// Enjoy this journey

/*

void print_m_rec(size_t_k i, size_t_k height, char avoid_empty);

void print_m(char avoid_empty)
{
    printf("Printing memory-map tree\n");
    printf("-----------------------------------------------------\n");
    print_m_rec(0, 0, avoid_empty);
    printf("-----------------------------------------------------\n");
    printf("Printing map as binary\n");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < map_end - map_start; i++)
    {
        printf("%d", IS_BIT_SET(((char *)map_start), i));
        if (i % 2)
            printf(" ");
    }
    printf("\n");
    printf("-----------------------------------------------------\n");
}

void print_m_data()
{
    printf("Map start:%lu\n\
Map end:%lu\n\
Dif:%lu\n\
Mem start:%lu\n\
Mem end:%lu\n\
Dif:%lu\n",
           (uint64_t)map_start, (uint64_t)map_end, (uint64_t)(map_end - map_start), (uint64_t)mem_start, (uint64_t)mem_end, (uint64_t)(mem_end - mem_start));
}

void print_m_rec(size_t_k i, size_t_k height, char avoid_empty)
{
    if (i < 0 || i > (map_end - map_start) * 2)
        return;

    states s = read_state(map_start, i);
    // avoid empty branches
    if (s == EMPTY && avoid_empty)
        return;

    print_m_rec(GET_LEFT(i), height + 1, avoid_empty);
    for (int i = 0; i < height; i++)
        printf("  ");

    if (s == EMPTY)
        printf("E\n");
    else if (s == SPLIT)
        printf("S\n");
    else if (s == FULL)
        printf("F\n");
    else
        printf("A\n");

    print_m_rec(GET_RIGHT(i), height + 1, avoid_empty);
}
*/

// Damn, this project works
// It is slightly beautiful
// I am slightly proud
