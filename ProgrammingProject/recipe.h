#ifndef RECIPE_H
#define RECIPE_H

#define MAX_INGREDIENTS 100
#define MAX_NAME_LEN 100

typedef struct {
    char name[MAX_NAME_LEN];
    char ingredients[MAX_INGREDIENTS][MAX_NAME_LEN];
    int ingredientCount;
} Recipe;

#endif
