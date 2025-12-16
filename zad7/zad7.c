#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_DIDNT_OPEN_ERROR (-1)
#define MAX_LINE (1024)
#define MAX_FILE_NAME (256)
#define MEMORY_NOT_ALLOCATED_CORRECT (-1)


// Directory structure
struct _dir;
typedef struct _dir* Dir_position;
typedef struct _dir {
    char name[MAX_FILE_NAME];
    Dir_position sibling;
    Dir_position child;
}Dir;

// Stack structure
struct _stack;
typedef struct _stack* Stack_position;
typedef struct _stack {
    Dir_position directory;
    Stack_position next;
}stack;




int print_dir(Dir_position current);
int push_on_stack(Dir_position current, Stack_position head);
int free_stack(Stack_position top);

Dir_position make_new_dir(char* dir_name);
Dir_position insert_sorted(Dir_position current, Dir_position new_dir);
Dir_position change_dir(Dir_position current, char* name);
Dir_position cd_back(Dir_position current, Stack_position head);
Dir_position free_dir(Dir_position current);


int main(void) {

    int choice = 0;
    char name[MAX_FILE_NAME] = { 0 };

    Dir_position root = (Dir_position)malloc(sizeof(Dir));
    strcpy(root->name, "C");
    root->sibling = NULL;
    root->child = NULL;
    Dir_position current = root;


    stack top = { .directory = 0,.next = NULL };
    Stack_position head = &top;

    if (root == NULL) {
        return MEMORY_NOT_ALLOCATED_CORRECT;
    }


    // MENU
    while (1) {
        printf("\n");
        printf("============OPTIONS============");
        printf("\nChoose what you want to do:\n");
        printf("1 - md\n");
        printf("2 - cd dir\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - exit\n");
        printf("============OPTIONS============");

        printf("\nYour choice: \n");
        scanf(" %d", &choice);

        switch (choice)
        {
        case 1:
            printf("Type in name of the new directory: ");
            scanf(" %s", name);

            current->child = insert_sorted(current->child, make_new_dir(name));
            break;

        case 2:
            printf("Enter the name of the directory you want to enter: ");
            scanf(" %s", name);

            Dir_position temp = change_dir(current, name);
            push_on_stack(current, head);
            current = temp;
            break;

        case 3:
            Dir_position temp1 = cd_back(current, head);
            current = temp1;
            printf("You are currently in the directory %s \n", current->name);
            break;

        case 4:
            print_dir(current);
            break;

        case 5:
            if (free_stack(head) == 0) {
                printf("Successful memory cleaning stack!");
                printf("\n");
            }

            if (free_dir(root) == 0) {
                printf("Successful memory cleaning dir!");
                printf("\n");
            }

            return EXIT_SUCCESS;
            break;

        default:
            printf("Error, Try Again!");
            break;
        }

    }


    return EXIT_SUCCESS;
}


Dir_position make_new_dir(char* dir_name) {
    Dir_position new_dir = NULL;
    new_dir = (Dir_position)malloc(sizeof(Dir));

    if (new_dir == NULL) {
        return MEMORY_NOT_ALLOCATED_CORRECT;
    }

    strcpy(new_dir->name, dir_name);
    new_dir->sibling = NULL;
    new_dir->child = NULL;

    return new_dir;
}


Dir_position insert_sorted(Dir_position current, Dir_position new_dir) {

    if (NULL == current) {
        return new_dir;
    }

    if (strcmp(current->name, new_dir->name) > 0) {
        new_dir->sibling = current;
        return new_dir;
    }

    current->sibling = insert_sorted(current->sibling, new_dir);

    return current;

}


int print_dir(Dir_position current) {

    Dir_position temp = current->child;

    if (temp == NULL) {
        printf("%s has no children\n", current->name);
        return EXIT_SUCCESS;
    }

    printf("In directory: %s\n", current->name);

    while (temp != NULL) {
        printf("%s\n", temp->name);
        temp = temp->sibling;
    }

    return EXIT_SUCCESS;
}


Dir_position change_dir(Dir_position current, char* name) {

    Dir_position temp = current->child;

    if (current == NULL) {
        printf("This directory is empty\n");
    }

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        temp = temp->sibling;
    }

    return temp;
}


int push_on_stack(Dir_position current, Stack_position head) {

    Stack_position new = NULL;
    new = (Stack_position)malloc(sizeof(stack));

    if (new == NULL) {
        printf("Memory not allocated correctly!");
        return MEMORY_NOT_ALLOCATED_CORRECT;
    }

    new->directory = current;

    new->next = head->next;
    head->next = new;


    return EXIT_SUCCESS;
}


Dir_position cd_back(Dir_position current, Stack_position head) {
    Stack_position temp = head->next;
    Dir_position parent = NULL;

    if (head == NULL) {
        printf("This directory doesn't have a parent!");
        return current;
    }

    parent = temp->directory;

    head->next = temp->next;
    free(temp);


    return parent;
}



Dir_position free_dir(Dir_position current) {
    if (current == NULL) {
        return NULL;
    }

    free_dir(current->sibling);
    free_dir(current->child);
    free(current);

    return EXIT_SUCCESS;
}


int free_stack(Stack_position top) {

    Stack_position head = top->next;
    Stack_position temp;

    if (top->next == NULL) {
        free(top);
        return EXIT_SUCCESS;
    }

    while (head != NULL) {
        temp = head->next;
        free(head);
        head = temp;
    }

    return EXIT_SUCCESS;
}