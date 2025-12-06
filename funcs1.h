#ifndef FUNCS1_H
#define FUNCS1_H

#define PI 3.14159265358979323846

//Wave_Type
typedef enum {
    SINE = 1,
    SQUARE,
    TRIANGLE,
    SAWTOOTH 
} WaveType;
typedef struct{
    WaveType type;
    double frequency;
    double amplitude;
    double offset;        //DC offset
    double sample_rate;
    double duration;
    int sample_count;
    double *samples;     // dynamic allocated buffer for datapoints
    double *timestamps;  // dynamic allocated buffer for time associated with each sample
} Waveform;

//Below two functions will be used main.c, funcs1.c, funcs2.c//
int  get_user_input(void);           
int  is_integer(const char *);      

//----------Signal Generator Functions Declaration----------//
int is_valid_number(const char *);            
double get_valid_input(void);                      // To get valid number from user
void signal_generator_menu(void); 
void print_signal_generator_menu(void);            // output the main menu description 
void select_signal_generator_menu_item(int);
void generate_sine(Waveform*);
void generate_square(Waveform*);
void generate_triangle(Waveform*);
void generate_sawtooth(Waveform*);
void get_waveform_parameters(Waveform*);           // Get waveform parameters from user e.g. freq, amplitude etc.
void save_waveform(Waveform*);                     // Make a file of waveform datapoints
void visualize_waveform(Waveform*);          
void handle_signal_generator(int);                 // run code based on user's choice for a specific waveform type
void free_waveform_memory(Waveform*);              // free the allocated dynamic memory

#endif