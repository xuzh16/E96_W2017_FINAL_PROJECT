#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fann.h"

int main(int argc, char **argv)
{
    int i;
    int location;           // location
    int n_input, n_output;  // number of inputs, number of outputs
    float max;
    fann_type *calc_out;    // fann output
    fann_type input[3];     // fann input
    struct fann *ann;       // fann nueral network
    char* input_file_name;
    char* buf = NULL;
    size_t len = 0;
    FILE *f;


    // set input_file_name to "test.txt", which is the file contains rssi inputs, if there is no argumnet
    if (argc != 2) {
        input_file_name = (char *) malloc(sizeof(char)*256);
        sprintf(input_file_name, "test.txt");
    } else {
        input_file_name = argv[1];
    }
    
    // load the trained neural network named "TEST.net"
    ann = fann_create_from_file("TEST.net");

    // open the input_file which has rssi inputs
    f = fopen(input_file_name,"r");
    if (f == NULL) {
        fprintf(stderr, "FAILED TO OPEN FILE \'%s\'. Exiting.\n", input_file_name);
        exit(EXIT_FAILURE);
    }
    // check the format of input_file
    if (getline(&buf, &len, f) == -1) {
        printf("FILE FORMAT IS WRONG FOR \'%s\'.\n", input_file_name);
        exit(EXIT_FAILURE);
    }
    // set n_input and n_output according to the first line of input_file
    sscanf(buf, "%d\t%d\n", &n_input, &n_output);
    len = 0;
    free(buf);
    buf = NULL;

    while (getline(&buf, &len, f) != -1) {  // read the next line of input_file 
        max = -100;
        // set inputs according to the second line of input_file
        sscanf(buf, "%f\t%f\t%f\n", &input[0], &input[1], &input[2]);

        /*************************** assignment (B) ***************************
         *       1) CODE to run the neural network using fann_run()           *
         *       2) CODE to determine the locations according to the calc_out *
         **********************************************************************/

        printf("RSSI:%f %f %f, Location is %d\n", input[0], input[1], input[2], location);
        len = 0;
        free(buf);
        buf = NULL;
    }
    
    fclose(f);
    fann_destroy(ann);
    return 0;
}
