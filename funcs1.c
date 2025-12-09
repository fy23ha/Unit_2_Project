#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs1.h"

//--------- Signal Generator Functions Defination----------//

void signal_generator_menu(void){
    print_signal_generator_menu();
    {
        int input = get_user_input();
        select_signal_generator_menu_item(input);
    }
}

void print_signal_generator_menu(void)
{
    printf("\n----------- Signal Generator Menu -----------\n");
    printf("\n"
           "\t1. Sine Waveform\t\t\n"
           "\t2. Square Waveform\t\t\n"
           "\t3. Triangular Waveform\t\t\n"
           "\t4. Sawtooth Waveform\t\t\n"
           "\t5. Exit\n\n");
    printf("---------------------------------------------\n");
}

void select_signal_generator_menu_item(int input)
{
    switch (input) {
        case 1:
            printf("\nSelected Type: Sine Waveform \n");
            handle_signal_generator(1);
            break;
        case 2:
            printf("\nSelected Type: Square Waveform \n");
            handle_signal_generator(2);
            break;
        case 3:
            printf("\nSelected Type: Traingular Waveform \n");
            handle_signal_generator(3);
            break;
        case 4:
            printf(" Selected Type: Sawtooth Waveform \n");
            handle_signal_generator(4);
            break;
        default:
            printf("Bye!\n");
            exit(0);
    }
}

void handle_signal_generator(int case_n){
    Waveform selected_waveform;
    selected_waveform.type = case_n;
    get_waveform_parameters(&selected_waveform);
    switch (case_n) {
        case 1:
            generate_sine(&selected_waveform);
            break;
        case 2:
            generate_square(&selected_waveform);
            break;
        case 3:
            generate_triangle(&selected_waveform);
            break;
        case 4:
            generate_sawtooth(&selected_waveform);
            break;
    }
    free_waveform_memory(&selected_waveform);
}

void get_waveform_parameters(Waveform* w){
    char buf[64];                                         // Temporary buffer to hold text input
    printf("Enter Frequency (Hz): ");
    w->frequency = get_valid_input();
    printf("Enter Amplitude (V): ");
    w->amplitude = get_valid_input();
    printf("Enter DC Offset (V): ");
    w->offset = get_valid_input();
    printf("Enter Sample rate (samples/sec): ");
    w->sample_rate = get_valid_input();
    printf("Enter Duration (sec): ");
    w->duration = get_valid_input();
    w->sample_count = (int)(w->sample_rate * w->duration);                      // number of samples = sample_rate * total time 
    w->samples = (double*) malloc(sizeof(double) * w->sample_count);            // Allocating memory for total number of samples
    w->timestamps = (double*) malloc(sizeof(double) * w->sample_count);         // Allocating memory for total number of samples
    //Check if memory allocation succeeded
    if (w->samples == NULL || w->timestamps == NULL) {
        printf("Critical Error: Memory allocation failed!\n");
        exit(1);
    }
}

void generate_sine(Waveform* w){
    for (int i=0; i< w->sample_count; i++){
        double t =  i / (w->sample_rate);                                           // Time in seconds for each sample
        w->timestamps[i] = t;
        w->samples[i] = (w->amplitude * sin(2*PI*t*w->frequency)) + w->offset;      // sample = A*sin(2πtf) + offset
    }
    save_waveform(w);
    visualize_waveform(w);
}

void generate_square(Waveform* w){
    for (int i=0; i< w->sample_count; i++){
        double t =  i / (w->sample_rate);                                          // Time in seconds for each sample
        w->timestamps[i] = t;
        if (sin(2 * PI * w->frequency * t) >= 0) {                                 // Output is +amplitude (HIGH) when sin(2πtf) >= 0
            w->samples[i] = w->amplitude + w->offset;                              // sample = Amplitude + dc_offset
        } else {                                                                   // Output is -amplitude (LOW) when sin(2πtf) < 0
            w->samples[i] = -w->amplitude + w->offset;                             // sample = -Amplitude + dc_offset
        }
    }
    save_waveform(w);
    visualize_waveform(w);
}
void generate_triangle(Waveform* w) {
    for (int i = 0; i < w->sample_count; i++) {
        double t = i / w->sample_rate;                                             // Time in seconds for each sample
        w->timestamps[i] = t;
        double slope =  asin(sin(2 * PI * w->frequency * t));                      // Arcsine output stored as variable slope. It simply 
        double base = (2 * w->amplitude / PI) * slope;                             // Scaling the waveform to the desired amplitude
        w->samples[i] = base + w->offset;
    }
    save_waveform(w);
    visualize_waveform(w);
}
void generate_sawtooth(Waveform* w) {
    for (int i = 0; i < w->sample_count; i++) {
        double t = i / w->sample_rate;
        w->timestamps[i] = t;                                                   // Time in seconds for each sample
        double cycle = fmod(t, 1 / w->frequency) * w->frequency;                // Computes where we are in the current wave cycle as a number between 0 and 1
        double base = (2 * w->amplitude) * (cycle - 0.5);                       // Scaling the waveform to the desired amplitude
        w->samples[i] = base + w->offset;
    }
    save_waveform(w);
    visualize_waveform(w);
}

// -------------- SAVE TO FILE --------------

void save_waveform(Waveform* w) {
    FILE *f = fopen("waveform.txt", "w");
    if (!f) {
        printf("Error writing file.\n");
        return;
    }
    fprintf(f, "timestamp,datapoint\n");                                        // Header format is :   timestamp,datapoint
    for (int i = 0; i < w->sample_count; i++){
        fprintf(f, "%f,%f\n",w->timestamps[i], w->samples[i]);}                 // e.g.  0.54,6.0
    fclose(f);
    printf("Saved as waveform.txt\n");
}

// ------------- VISUALIZATION -------------

void visualize_waveform(Waveform* w) {
    FILE *gp = popen("gnuplot", "w");           
    if (!gp) {
        printf("Failed to open gnuplot.\n");
        return;
    }
    printf("Waveform plot saved as plot.png\n");
    // Output to a PNG file
    fprintf(gp, "set terminal pngcairo size 800,600\n");
    fprintf(gp, "set output 'plot.png'\n");
    // --- Gnuplot settings ---
    fprintf(gp, "set title 'Waveform Plot'\n");
    fprintf(gp, "set xlabel 'Time (s)'\n");
    fprintf(gp, "set ylabel 'Amplitude (V)'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set key off\n");                                                // Hides the legend
    fprintf(gp, "plot '-' with lines linewidth 2 lc rgb 'blue'\n");              // 'with lines' connects the points by line; 'lc rgb 'blue' sets the color 
    for (int i = 0; i < w->sample_count; i++) {
        fprintf(gp, "%f %f\n", w->timestamps[i], w->samples[i]);                 // time & sample
    }
    fprintf(gp, "e\n");                                                          // Tells gnuplot to stop reading points.
    pclose(gp);                                                                  // Close gnu_pipe. Similar to closing a file
}

void free_waveform_memory(Waveform* w) {
    if (w->samples != NULL) {
        free(w->samples);
        w->samples = NULL; 
    }
    if (w->timestamps != NULL) {
        free(w->timestamps);
        w->timestamps = NULL; 
    }
}
/* The below validity function are inspired from the input validity functions given in main.c*/
double get_valid_input(void)
{
    char buf[128];                                                            // Temporary buffer to hold text input
    int valid_input = 0;
    double value = 0.0;

    do {
        if (!fgets(buf, sizeof(buf), stdin)) {
            /* EOF or error; bail out gracefully */
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0';                                    // strip trailing newline
        if (!is_valid_number(buf)) {
            printf("Invalid input. Only number are allowed!\nPlease enter a valid value : ");
            valid_input = 0;
        } else {
            value = atof(buf);
            valid_input = 1;
        }
    } while (!valid_input);

    return value;
}

int is_valid_number(const char *s)
{
    if (!s || !*s) return 0;                            // Empty string

    /* optional sign */
    if (*s == '+' || *s == '-') s++;

    /* must have at least one digit */
    if (!isdigit((unsigned char)*s)) return 0;

    int dot_count = 0;
    while (*s) {
        /* check number of dots in input number */
        if (*s == '.') {                            
            dot_count++;
            if (dot_count > 1) return 0; 
        }
        else if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
