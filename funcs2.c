#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs2.h"
#include "funcs1.h"                //need get_user_input() & is_integer() functions declared in funcs1.h

//--------- Signal Analyser Functions Defination----------//

void signal_analyser_menu(void){
    print_signal_analyser_menu();
    {
        int input = get_user_input();
        select_signal_analyser_menu_item(input);
    }
}

void print_signal_analyser_menu(void)
{
    printf("\n----------- Signal Analyser Menu -----------\n");
    printf("\n"
           "\t1. Find Amplitutde\t\t\n"
           "\t2. Find Frequency\t\t\n"
           "\t3. Find DC Offset\t\t\n"
           "\t4. Find all\t\t\n"
           "\t5. Exit\n\n");
    printf("--------------------------------------------\n");
}

void select_signal_analyser_menu_item(int input)
{
    char filename[128];                                 // Variable to store filename
    if (input>=1 && input<=4){
        get_filename(filename, sizeof(filename));
    }
    switch (input) {
        case 1:
            printf("\nSelected Type: Finding Amplitude \n");
            handle_signal_analyser(1, filename);
            break;
        case 2:
            printf("\nSelected Type: Finding Frequency \n");
            handle_signal_analyser(2, filename);
            break;
        case 3:
            printf("\nSelected Type: Finding DC-Offset \n");
            handle_signal_analyser(3, filename);
            break;
        case 4:
            printf(" Selected Type: Finding all \n");
            handle_signal_analyser(4, filename);
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

void get_filename(char *filename, size_t size)
{
    char buf[256];                                 // Temporary buffer to hold text input
    while (1) {
        printf("Enter full filename: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            printf("Input error. Exiting.\n");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0';         // Removes \n or \r at the end of input so filenames donot contain hidden newline characters
        // Check if empty
        if (strlen(buf) == 0) {
            printf("Filename cannot be empty!\n");
            continue;
        }
        // Copy safely from buffer
        strncpy(filename, buf, size - 1);
        filename[size - 1] = '\0';
        break;
    }
}

void handle_signal_analyser(int case_n, const char* filename){

    /*"filename.txt" should contain a header line followed by two numbers per line separated by coma.   eg:  0.4,6.0  */ 

    double *time = (double*) malloc(MAX_BUFFER_SIZE * sizeof(double));           // Allocating memory equal to max buffer size given in header file
    double *sample = (double*) malloc(MAX_BUFFER_SIZE * sizeof(double));         // Allocating memory equal to max buffer size given in header fil
    if (time == NULL || sample == NULL) {
        printf("Critical Error: Memory Allocation failed!\n");
        return;
    }

    FILE *input = fopen(filename, "r");
    if (!input) {
        printf("Error opening file.\n");
        //free the memory
        free(time); 
        free(sample);
        return;
    }
    char line[MAX_LINES_CHARACTER];                                             // Temporary buffer to hold text input
    fgets(line, MAX_LINES_CHARACTER, input);                                    // Skips the header line (not gonna use it)

    int count = 0;
    while (fgets(line, MAX_LINES_CHARACTER, input)) {                           // Runs unless all lines are read 
        if (sscanf(line, "%lf,%lf", &time[count], &sample[count]) == 2) {       // Guarantees that each line contains exactly two float numbers
            count++;
        } else {
            printf("Error parsing values inside provided file at line number %d!\n", count);
        }
    }
    fclose(input);

    if (count > 0) {
        switch (case_n) {
            case 1:
                find_amplitude(sample, count);
                break;
            case 2:
                find_frequency(time, sample, count);
                break;
            case 3:
                find_offset(sample, count);
                break;
            case 4:
                find_all(time, sample, count);
                break;
        }
    } else {
        printf("No valid data found in file. Cannot analyse.\n");
    }

    //free the memory
    free(time); 
    free(sample);
}

void find_amplitude(const double *sample, int n)                            // n is number of total samples
{
     double max = sample[0], min = sample[0];
    for (int i = 1; i < n; i++) {
        if (sample[i] > max) max = sample[i];
        if (sample[i] < min) min = sample[i];
    }
    double amplitude = (max - min) / 2.0;
    printf("Amplitude = %.3f\n", amplitude);                  
    return;
}

void find_frequency(const double *time, const double *sample, int n)        // n is number of total samples
{
    // Calculates the average (DC Offset) to use as a crossing threshold to find frequency
    double max = sample[0], min = sample[0];
    for (int i = 1; i < n; i++) {
        if (sample[i] > max) max = sample[i];
        if (sample[i] < min) min = sample[i];
    }
    double offset = (max + min) / 2.0;
    double t1 = -1.0, t2 = -1.0;                // time set a random value as only initilization was needed here.
    int crossings_found = 0;

    // Looks for the signal crossing from 'below offset' to 'above offset'. (Rising Edge Crossing)
    for (int i = 1; i < n; i++) {
        // If previous sample is below offset, and current is above or equal
        if (sample[i-1] < offset && sample[i] >= offset) {
            if (crossings_found == 0) {                                 // If its the very first time when rising edge crossing occurs
                t1 = time[i];
                crossings_found++;
            }
            else if (crossings_found == 1) {                           // If rising edge crossing has occured once before
                t2 = time[i];
                crossings_found++;
                break;                                                // Found one full cycle
            }
        }
    }
    if (crossings_found < 2) {                                        // In case of invalid waveform which doesnot even have a full cycle datapoints
        printf("Not enough cycles to determine frequency.\n");      
        return;
    }

    double period = t2 - t1;
    double freq = 1.0 / period;

    printf("Time Period = %.5f\n", period);
    printf("Frequency = %.2f Hz\n", freq);
}

void find_offset(const double *sample, int n)
{
    double max = sample[0], min = sample[0];
    for (int i = 1; i < n; i++) {
        if (sample[i] > max) max = sample[i];
        if (sample[i] < min) min = sample[i];
    }
    double offset = (max + min) / 2.0;
    printf("DC Offset = %.2f\n", offset);
    return;
}

void find_all(const double *time, const double *sample, int n)
{
    find_amplitude(sample, n);
    find_frequency(time, sample, n);
    find_offset(sample, n);

}