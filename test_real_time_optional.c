#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/******************** assignment (C) **************************
 *	CODE to include header file to drive LED by mraa library  *
 **************************************************************/


static volatile int run_flag = 1;
// file pointers 
static FILE * fp1 = NULL;
static FILE * fp2 = NULL;
static FILE * fp3 = NULL;

void do_when_interrupted()
{
	printf("\nUser issued ^C sequence. Terminating.\n");
	run_flag = 0;
	if (fp1 != NULL)
		fclose(fp1);
	if (fp2 != NULL)
		fclose(fp2);
	if (fp3 != NULL)
		fclose(fp3);
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{		
	/********************** assignment (C) ************************
 	 * 	 	 CODE to declare variables for driving LEDs (gpio)    *
 	 **************************************************************/
	int flag;

	// number of input neurons, number of output neurons
	int num_input_neurons, num_output_neurons;

	// variables that will store mac address in access_point.conf
	char *mac1, *mac2, *mac3;

	// variables that will store mac_address,rssi,signal in relevant_rssi_info.csv
	char *mac1_str, *mac2_str, *mac3_str;

	// variables that will store rssi values of access points
	char *rssi1, *rssi2, *rssi3;

	size_t len;	
	ssize_t read;

	// pattern "," which seperates mac address, rssi, signal in relevant_rssi_info.csv
	const char s[2] = ",";

	signal(SIGINT, do_when_interrupted);

	// set up the number of inputs (features) and outputs (location area) 
	// according to your system
	num_input_neurons = 3;

	/*********************************** assignment (E) ***************************************
	 * 	  	Change num_output_neurons according to your number of region to be detected	      *
	 ******************************************************************************************/
	num_output_neurons = 4;

	while (run_flag == 1) {
		read = 0;
		len = 0;
		// flag for if rssi can be detected 
		flag = 0;
		printf("Press enter to continue\n");	

		// waiting for the user to press [Enter]
		while (getchar() != '\n' && run_flag);
		
		printf("Running ./get_rssi\n");
		// system call to run get_rssi and get rssi_agg.csv
		system("./get_rssi");

		printf("Collecting relevant information from rssi_agg.csv\n");
		// system call to run filter_address to filter access point rssi data from rssi_agg.csv according to access_point.conf 
		// the filtered data will be stored at relevant_rssi_info.csv
		system("./filter_address rssi_agg.csv relevant_rssi_info.csv");

		// *** important ***
		// parse relevant_rssi_info.csv into test.txt which will be used by test_neural_net the output file will be test.txt

		fp1 = fopen("access_point.conf","r");
		fp2 = fopen("relevant_rssi_info.csv","r");

		// read mac address in access_point.conf
		getline(&mac1, &len, fp1); // read line1 from fp1, and store it in mac1
		mac1[strlen(mac1)-1] = '\0'; // remove the newline from the string
		len = 0;
		getline(&mac2, &len, fp1); // read line2 from fp1, and store it in mac2
		mac2[strlen(mac2)-1] = '\0';
		len = 0;
		getline(&mac3, &len, fp1); // read line3 from fp1, and store it in mac3
		mac3[strlen(mac3)-1] = '\0';
		printf("MAC address 1 from access_point.conf:%s\n", mac1);
		printf("MAC address 2 from access_point.conf: %s\n", mac2);
		printf("MAC address 3 from access_point.conf: %s\n", mac3);

		fclose(fp1);
		fp1 = NULL;


		// read mac_address,rssi,signal in relevant_rssi_info.csv
		len = 0;
		getline(&mac1_str, &len, fp2); // read line1 from fp2, and store it in mac1_str
		len = 0;
		getline(&mac2_str, &len, fp2);
		len = 0;
		getline(&mac3_str, &len, fp2);
		printf("Data1 from relevant_rssi_info.csv: %s\n", mac1_str);
		printf("Data2 from relevant_rssi_info.csv: %s\n", mac2_str);
		printf("Data3 from relevant_rssi_info.csv: %s\n", mac3_str);

		fclose(fp2);
		fp2 = NULL; 

		// get rssi of ap1
		if (strstr(mac1_str, mac1) != NULL) { // check if mac1_str contains mac1 
			rssi1 = strtok(mac1_str, s);	  // assign rssi1 with string before the first "," (which is mac address) in mac1_str
			rssi1 = strtok(NULL, s);		  // assign rssi1 with string between first "," and second "," (which is rssi value) in mac1_str
			flag = 1;						  // rssi value is detected, flag set to 1
		}
		if (strstr(mac2_str, mac1) != NULL) {
			rssi1 = strtok(mac2_str, s);
			rssi1 = strtok(NULL, s);
			flag = 1;
		}

		if (strstr(mac3_str, mac1) != NULL) {
			rssi1 = strtok(mac3_str, s);
			rssi1 = strtok(NULL, s);
			flag = 1;
		}
		if (flag == 0) {				// if rssi1 value is not detected in relevant_rssi_info.csv 
			rssi1 = "-80";				// set rssi1 to -80 assuming access point 1 has minimum rssi value because it can't be detected
			printf("Access point : %s not detected\n", mac1); 
			/********************* assignment (C) *************************
 			 * 	  	CODE to turn on LED when RSSI is not detected 	      *
 			 **************************************************************/
		}
		flag = 0;						// reset the flag for rssi2 and rssi3
		printf("RSSI1: %s\n", rssi1);

		// get rssi of ap2
		if (strstr(mac1_str, mac2) != NULL) {
			rssi2 = strtok(mac1_str, s);
			rssi2 = strtok(NULL, s);
			flag = 1;
		}
		if (strstr(mac2_str, mac2) != NULL) {
			rssi2 = strtok(mac2_str, s);
			rssi2 = strtok(NULL, s);
			flag = 1;
		}
		if (strstr(mac3_str, mac2) != NULL) {
			rssi2 = strtok(mac3_str, s);
			rssi2 = strtok(NULL, s);
			flag = 1;
		}
		if (flag == 0)
		{
			rssi2 = "-80";
			printf("Access point : %s not detected\n", mac2);
			/************************ assignment (C) **********************
 			 * 	  	CODE to turn on LED when RSSI is not detected 	      *
 			 **************************************************************/
		}

		printf("RSSI2: %s\n", rssi2);
		flag = 0;
		// get rssi of ap3
		if (strstr(mac1_str, mac3) != NULL) {
			rssi3 = strtok(mac1_str, s);
			rssi3 = strtok(NULL, s);
			flag = 1;
		}

		if (strstr(mac2_str, mac3) != NULL) {
			rssi3 = strtok(mac2_str, s);
			rssi3 = strtok(NULL, s);
			flag = 1;
		}
		if (strstr(mac3_str, mac3) != NULL) {
			rssi3 = strtok(mac3_str, s);
			rssi3 = strtok(NULL, s);
			flag = 1;
		}
		if (flag == 0) {		
			rssi3= "-80 ";
			printf("Access point : %s not detected\n", mac3);
			/********************* assignment (C) *************************
 			 * 	  	CODE to turn on LED when RSSI is not detected 	      *
 			 **************************************************************/
		}
		printf("RSSI3: %s\n", rssi3);	

		printf("Creating the file to test the neural network\n");

		// write the parsed rssi value to test.txt 
		fp3 = fopen("test.txt", "w");
		// first line of test.txt indicating number of inputs, and number of outputs, use \t to seperate inputs & outputs
		fprintf(fp3, "%d\t%d\n", num_input_neurons, num_output_neurons); 
		// seond line of test.txt including rssi1, rssi2, and rssi3, use \t to seperate rssi values
		fprintf(fp3, "%s\t%s\t%s", rssi1, rssi2, rssi3);
		fclose(fp3);
		fp3 = NULL;

		printf("Testing the neural network\n");
		// system call to run test_neural_net
		system("./test_neural_net");
	}

	return 0;
}
