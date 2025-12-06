#ifndef FUNCS2_H
#define FUNCS2_H

#define MAX_BUFFER_SIZE 100000               // Don't know the file size in advance, so guess "big enough".
#define MAX_LINES_CHARACTER 256              // Maximum length of a line in the input file


//----------- Signal Analyser Functions Declaration ----------//
void signal_analyser_menu(void);
void print_signal_analyser_menu(void);
void select_signal_analyser_menu_item(int input);
void get_filename(char *, size_t);                              // To get a valid filename
void handle_signal_analyser(int , const char*);                 // run code based on user's choice for required analyser tool. Also, handles the waveform file reading.
void find_amplitude(const double *, int);
void find_frequency(const double *, const double *, int);
void find_offset(const double *, int);
void find_all(const double *, const double *, int);             // Finds amplitude, time-period, frequency, dc-offset 

#endif