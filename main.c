// ELEC2645 Unit 2 Project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs1.h"
#include "funcs2.h"

//-------Functions Declaration----------//
static void main_menu(void);                                    /* runs in the main loop */
static void print_main_menu(void);                              /* output the main menu description */
static void select_menu_item(int input);                        /* run code based on user's choice */
static void go_back_to_main(void);

//---------Functions Definition----------//
static void main_menu(void)
{
    print_main_menu();
    {
        int input = get_user_input();
        select_menu_item(input);
    }
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            printf("\nSelected Type: Signal Generator \n");
            signal_generator_menu();
            go_back_to_main();
            break;
        case 2:
            printf("\nSelected Type: Signal Analyser \n");
            signal_analyser_menu();
            go_back_to_main();
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n---------------- Main Menu ------------------\n");
    printf("\n"
           "\t1. Signal Generator\t\t\n"
           "\t2. Signal Analyser \t\t\n"
           "\t3. Exit\n\n");
    printf("---------------------------------------------\n");
}

int get_user_input(void)
{
    enum { MENU_ITEMS = 5 };   /* 1..4 = items, 5 = Exit */
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect item: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }

        // strip trailing newline
        buf[strcspn(buf, "\r\n")] = '\0';

        if (!is_integer(buf)) {
            printf("Enter an integer!\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid menu item!\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to go back to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* strip newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}

int is_integer(const char *s)
{
    if (!s || !*s) return 0;

    /* optional sign */
    if (*s == '+' || *s == '-') s++;

    /* must have at least one digit */
    if (!isdigit((unsigned char)*s)) return 0;

    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

//----------------------------MAIN---------------------------//

int main(void)
{
    /* this will run forever until we call exit(0) in select_menu_item() */
    while(1) {
        main_menu();
    }
    return 0;
}
