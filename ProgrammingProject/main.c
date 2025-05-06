#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "recipe.h"

void header()
{
    printf("Thank You For Using RecipeVault!\n\n");
    printf("---------------------\n");
    printf("|1 - Display Recipes|\n");
    printf("|2 - Add Recipes    |\n");
    printf("|3 - Edit Recipes   |\n");
    printf("|4 - Delete Recipes |\n");
    printf("|0 - Exit           |\n");
    printf("---------------------\n\n");
}

void display(char path[])
{
    system("cls");
    FILE *filePointer = fopen(path, "r");

    printf("---------------------\n");
    printf("    Recipe Display   \n");
    printf("---------------------\n\n");

    if (filePointer == NULL)
    {
        printf("The file cannot be accessed.\n");
        return;
    }

    char search[100];
    printf("Enter search term (or leave empty to show all): ");
    getchar(); // clear newline
    fgets(search, sizeof(search), stdin);
    search[strcspn(search, "\n")] = '\0';

    for (int i = 0; search[i]; i++)
        search[i] = tolower(search[i]);

    char line[256];
    int print = 0;
    int matched = 0;

    while (fgets(line, sizeof(line), filePointer))
    {
        line[strcspn(line, "\n")] = '\0';

        if (strchr(line, ':'))
        {
            char lowered[256];
            strcpy(lowered, line);
            for (int i = 0; lowered[i]; i++)
                lowered[i] = tolower(lowered[i]);

            if (strlen(search) == 0 || strstr(lowered, search))
            {
                print = 1;
                matched = 1;
                printf("\033[96m%s\033[0m\n", line);
            }
            else
            {
                print = 0;
            }
        }
        else if (print)
        {
            if (strstr(line, "-"))
            {
                char *start = strstr(line, "-");
                start++;
                int prefixLength = start - line;
                char prefix[256];
                strncpy(prefix, line, prefixLength);
                prefix[prefixLength] = '\0';

                char temp[256];
                strcpy(temp, start);
                char *token = strtok(temp, " ");
                char *tokens[50];
                int count = 0;

                while (token != NULL && count < 50)
                {
                    tokens[count++] = token;
                    token = strtok(NULL, " ");
                }

                printf("%s-", prefix);
                for (int i = 0; i < count; i++)
                {
                    if (i == count - 1)
                        printf("\033[93m%s\033[0m", tokens[i]);
                    else
                        printf("%s ", tokens[i]);
                }
                printf("\n");
            }
            else
            {
                printf("%s\n", line);
            }
        }
    }

    if (!matched)
        printf("No matching recipes found.\n");

    fclose(filePointer);
    printf("\nPress Enter to go back...");
    getchar();
    system("cls");
}

void add(char path[])
{
    system("cls");
    FILE *filePointer = fopen(path, "a");

    if (filePointer == NULL)
    {
        printf("The file cannot be accessed.\n");
        return;
    }

    Recipe recipe;
    printf("---------------------\n");
    printf("     Recipe Adder    \n");
    printf("---------------------\n\n");

    printf("Enter recipe name:\n");
    getchar();
    fgets(recipe.name, sizeof(recipe.name), stdin);
    recipe.name[strcspn(recipe.name, "\n")] = '\0';

    while (1)
    {
        printf("Enter number of ingredients:\n");
        if (scanf("%d", &recipe.ingredientCount) != 1 || recipe.ingredientCount <= 0 || recipe.ingredientCount > MAX_INGREDIENTS)
        {
            printf("Invalid input. Please enter a valid number between 1 and %d.\n", MAX_INGREDIENTS);
            while (getchar() != '\n')
                ;
        }
        else
        {
            while (getchar() != '\n')
                ;
            break;
        }
    }

    printf("Enter each ingredient and amount (e.g., Sugar 2 tbsp):\n");
    for (int i = 0; i < recipe.ingredientCount; i++)
    {
        printf("Ingredient %d: ", i + 1);
        fgets(recipe.ingredients[i], MAX_NAME_LEN, stdin);
        recipe.ingredients[i][strcspn(recipe.ingredients[i], "\n")] = '\0';
    }

    fprintf(filePointer, "%s:\n%d Ingredients\n", recipe.name, recipe.ingredientCount);
    for (int i = 0; i < recipe.ingredientCount; i++)
    {
        fprintf(filePointer, "      -%s\n", recipe.ingredients[i]);
    }
    fprintf(filePointer, "\n");
    fclose(filePointer);

    system("cls");
    printf("Recipe \"%s\" with %d ingredients has been added.\n\n", recipe.name, recipe.ingredientCount);
    printf("Press Enter to continue...");
    while (getchar() != '\n')
        ;
    system("cls");
}

void edit(const char *path)
{
    system("cls");
    printf("---------------------\n");
    printf("     Recipe Editor   \n");
    printf("---------------------\n\n");

    FILE *file = fopen(path, "r");
    if (!file)
    {
        printf("Failed to open file.\n");
        return;
    }

    Recipe recipes[100];
    int recipeCount = 0;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strchr(line, ':'))
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(recipes[recipeCount].name, line);
        }
        else if (strstr(line, "Ingredients"))
        {
            sscanf(line, "%d", &recipes[recipeCount].ingredientCount);
            for (int i = 0; i < recipes[recipeCount].ingredientCount; i++)
            {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = '\0';
                char *start = strstr(line, "-");
                if (start)
                {
                    start++; // skip dash
                    while (*start == ' ')
                        start++;
                    strcpy(recipes[recipeCount].ingredients[i], start);
                }
            }
            recipeCount++;
        }
    }
    fclose(file);

    printf("\n=== Available Recipes ===\n");
    for (int i = 0; i < recipeCount; i++)
    {
        printf("%d. %s\n", i + 1, recipes[i].name);
    }

    int choice;
    printf("\nEnter recipe number to edit: ");
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > recipeCount)
    {
        printf("Invalid choice. Try again: ");
        while (getchar() != '\n')
            ;
    }
    while (getchar() != '\n')
        ;

    Recipe *selected = &recipes[choice - 1];
    system("cls");
    printf("Editing Recipe: %s\n", selected->name);

    printf("Current name: %s\n", selected->name);
    printf("Enter new name (leave blank to keep): ");
    char input[MAX_NAME_LEN];
    fgets(input, MAX_NAME_LEN, stdin);
    input[strcspn(input, "\n")] = '\0';
    if (strlen(input) > 0)
    {
        strcpy(selected->name, input);
    }

    printf("Current number of ingredients: %d\n", selected->ingredientCount);
    printf("Enter new number (or same): ");
    int newCount;
    while (scanf("%d", &newCount) != 1 || newCount <= 0 || newCount > MAX_INGREDIENTS)
    {
        printf("Invalid number. Try again: ");
        while (getchar() != '\n')
            ;
    }
    selected->ingredientCount = newCount;
    while (getchar() != '\n')
        ;

    for (int i = 0; i < selected->ingredientCount; i++)
    {
        printf("Ingredient %d (leave blank to keep): ", i + 1);
        fgets(input, MAX_NAME_LEN, stdin);
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0)
        {
            strcpy(selected->ingredients[i], input);
        }
    }

    file = fopen(path, "w");
    if (!file)
    {
        printf("Failed to open file for writing.\n");
        return;
    }

    for (int i = 0; i < recipeCount; i++)
    {
        fprintf(file, "%s\n", recipes[i].name);
        fprintf(file, "%d Ingredients\n", recipes[i].ingredientCount);
        for (int j = 0; j < recipes[i].ingredientCount; j++)
        {
            fprintf(file, "      -%s\n", recipes[i].ingredients[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\nRecipe updated successfully!\n");
    printf("Press Enter to continue...");
    getchar();
    system("cls");
}

void delete_recipe(const char *path)
{
    system("cls");
    printf("---------------------\n");
    printf("     Recipe Deleter  \n");
    printf("---------------------\n\n");

    FILE *file = fopen(path, "r");
    if (!file)
    {
        printf("Failed to open file.\n");
        return;
    }

    Recipe recipes[100];
    int recipeCount = 0;
    char line[256];

    // Load recipes
    while (fgets(line, sizeof(line), file))
    {
        if (strchr(line, ':'))
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(recipes[recipeCount].name, line);
        }
        else if (strstr(line, "Ingredients"))
        {
            sscanf(line, "%d", &recipes[recipeCount].ingredientCount);
            for (int i = 0; i < recipes[recipeCount].ingredientCount; i++)
            {
                fgets(line, sizeof(line), file);
                line[strcspn(line, "\n")] = '\0';
                char *start = strstr(line, "-");
                if (start)
                {
                    start++;
                    while (*start == ' ')
                        start++;
                    strcpy(recipes[recipeCount].ingredients[i], start);
                }
            }
            recipeCount++;
        }
    }
    fclose(file);

    if (recipeCount == 0)
    {
        printf("No recipes found.\n");
        printf("\nPress Enter to return to menu...");
        getchar();

        return;
    }

    // Display list
    for (int i = 0; i < recipeCount; i++)
    {
        printf("%d. %s\n", i + 1, recipes[i].name);
    }

    // Choose which to delete
    int choice;
    printf("\nEnter recipe number to delete: ");
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > recipeCount)
    {
        printf("Invalid input. Try again: ");
        while (getchar() != '\n')
            ;
    }
    while (getchar() != '\n')
        ;

    // Confirm deletion
    printf("Are you sure you want to delete \"%s\"? (y/n): ", recipes[choice - 1].name);
    char confirm = getchar();
    while (getchar() != '\n')
        ; // clear input buffer

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Deletion cancelled.\n");
        printf("Press Enter to return to menu...");
        getchar();
        system("cls");
        return;
    }

    // Rewrite file excluding selected recipe
    file = fopen(path, "w");
    if (!file)
    {
        printf("Failed to open file for writing.\n");
        return;
    }

    for (int i = 0; i < recipeCount; i++)
    {
        if (i == choice - 1)
            continue;
        fprintf(file, "%s\n", recipes[i].name);
        fprintf(file, "%d Ingredients\n", recipes[i].ingredientCount);
        for (int j = 0; j < recipes[i].ingredientCount; j++)
        {
            fprintf(file, "      -%s\n", recipes[i].ingredients[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\nRecipe deleted successfully!\n");
    printf("Press Enter to return to menu...");
    getchar();
    system("cls");
}

void menu(int op, char path[])
{
    switch (op)
    {
    case 1:
        display(path);
        break;
    case 2:
        add(path);
        break;
    case 3:
        edit(path);
        break;
    case 4:
        delete_recipe(path);
        break;
    default:
        system("cls");
        printf("EXIT!\n");
    }
}

int main()
{
    char path[] = "file.txt";
    int option;

    do
    {
        header();
        printf("Enter option: ");

        if (scanf("%d", &option) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        menu(option, path);
    } while (option > 0 && option <= 4);

    return 0;
}
