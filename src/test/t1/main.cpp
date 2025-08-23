
// main.cpp

#include <stdio.h>

#include "base.h"

int
main()
{
    printf("Hello, World!\n");

    Vector2 vec(1.0f, 2.0f);
    printf("Vector2: (%f, %f)\n", vec.x, vec.y);

    return 0;
}
