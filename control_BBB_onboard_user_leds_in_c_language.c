/*
==========================================================
Name: Program to control the USER LEDs of BeagleBone Black written in C language
Author: George Calin (george.calin@gmail.com)
Version: 0.1
Description: This program controls the USER LEDs concerning trigger and brightness
==========================================================
*/

#include <stdio.h>
/* *** the stdio.h header defines three variable types, several macros and various functions for input and output */

#include <stdlib.h>
/* *** the stdlib.h header defines four variable types, several macros and various functions for performing general functions *** */

#include <stdint.h>
/* *** the stdint.h header allows programmers to write portable code by providing a fixed set of typedefs that specify exact width integer types *** */

#include <string.h>
/* *** the string.h header in this case allows various functions for manipulating arrays of characters *** */

#include <errno.h>
/* *** defines the external errno variable and all the values it can take on *** */

#include <unistd.h>
/* *** supplies EXIT_FAILURE, EXIT_SUCCESS *** */

#include <fcntl.h>

/* *** define the path to USER LEDs control directory in the 'sys' directory *** */
#define PATH_TO_CONFIG_USER_LEDS "/sys/class/leds"

/* *** define the path to USER LED 1 directory in the 'sys' directory *** */
#define PATH_TO_CONFIG_USER_LED1 "/sys/class/leds/beaglebone:green:usr1"

#define USER_LED_NUMBER 0
#define SIZE_IN_LETTERS 100

int enter_user_led;


int get_user_led_no_from_keyboard()
{
	printf("Hello, this is an application that controls USER LEDs on the BeagleBone Black.\n");
	printf("*********************************************************************************\n");
	printf("Please enter the number of the USER LED you wish to control (0-3):\t");
	int input_number;

	if (scanf("%d",&input_number)== 1) {
		printf("here");
		if(input_number<4 && input_number>=0)
		{
			printf("\n");
			printf(" You entered a valid USER LED number. Thanks. \n");
			printf("This application controls the properties of USER LED %d\n",input_number);
			return input_number;
		}
		else
		{
			printf("\n");
			printf(" You did not enter a valid USER LED number. Remember: There are only 4 USER LEDs on BeagleBone Black (0-3)");
			return (-1);
		}
	}
	else
	{
		printf("\n");
		printf("You did not enter a valid USER LED number.There are only 4 USER LEDs on BeagleBone Black (0,1,2,3)");\
		return (-2);
	}
}


int write_values_to_brightness_file(uint8_t led_no, char * value)
{
  printf("Write values to brightness file.\n");
  int fd;
  /* *** fd stands for file descriptor which is an integer that identifies an open file of the process. *** */
  int return_from_write=0;
  char buffer[SIZE_IN_LETTERS];

  /* *** snprintf() function is defined in the < stdio.h> header. snprintf() formats and stores a series of characters and values in the array buffer. *** */
  int numbers_of_characters=snprintf(buffer,sizeof(buffer),PATH_TO_CONFIG_USER_LEDS "/beaglebone:green:usr%d/brightness", led_no);

  /* *** Open the file for writing values of brightness in write mode *** */
  /* *** fd is the file descriptor of the file. This integer uniquely identifies the open file in the process *** */
  fd=open(buffer,O_WRONLY);

  printf("the file descriptor is: %d;\n",fd);

  if(fd<0)
  {
	  perror("There was an error when opening the 'brightness' file.\n");
	  return (-1);
  }

  /* *** Next, we shall write the integer value in the 'brightness' file *** */
  /* ***  write(): writes the cnt number of bytes to the file or socket associated with the fd file descriptor.cnt should not be larger than INT_MAX.
   * If cnt=0, write() only returns 0 without any other action.
   * write() returns how many bytes were actually written on success
   * write() returns 0 when reaching end of file
   * write() returns -1 on error
   * write() returns -1 on signal interrupt
   */

  return_from_write=write(fd, value, strlen(value));

  if (return_from_write<0)
  {
	printf("There was an error when writing to 'brightness' file.\n");
	return (-1);
  }

  return (0);
}

int write_values_to_trigger_file(uint8_t led_no,char * value)
{
	/* *** Declare the file descriptor variable: a unique integer that characterizes the open file in the process *** */
	int fd;
	/* *** Declare the variable which will be returned from the write() command *** */
	int number_bytes_written;
	/* *** Declare the array of type char that will be the buffer used in snprintf(),open(),write() *** */
	char buffer[SIZE_IN_LETTERS];

	/* *** Concatenate the strings into the variable representing the path to the 'trigger' file *** */
	snprintf(buffer,sizeof(buffer),PATH_TO_CONFIG_USER_LEDS "/beaglebone:green:usr%d/trigger",led_no);

	/* *** Open the 'trigger' file in write mode *** */
	fd=open(buffer,O_WRONLY);

	/* *** Exit if opening the file was not successful *** */
	if(fd==-1)
	{
		perror("Error when opening the 'trigger' file");
		return (-1);
	}

	/* *** Write the string 'value' in the file 'trigger' *** */
	number_bytes_written=write(fd, value,strlen(value));

	/* *** Exit if write was not successful *** */
	if(number_bytes_written<=0)
	{
		perror("There was an error while writing to the file 'trigger'\n");
		return (-1);
	}

	return (0);
}

void process_values_fed_to_trigger_file(char * string_value)
{
	/* **** Logical operators in C:
	 *
	 * && (AND) if both operands are non-zero, then the condition is true ex. A=1, B=0 => A && B = false
	 * || (OR) if any of the two operands is non-zero, then the condition is true. A=1, B=0 => A || B = true
	 * ! (Logical Not) it is used to reverse the logical state of the operand. A=1,B=0 => ! (A && B) = true
     * ***/
 if(!(strcmp(string_value,"heartbeat") && strcmp(string_value,"timer") && strcmp(string_value,"none") && strcmp(string_value,"default-on") && strcmp(string_value,"mmc0") && strcmp(string_value,"mmc1") && strcmp(string_value,"oneshot")))
 {
	 write_values_to_trigger_file(enter_user_led,string_value);
 }
 else
 {
	 printf("You did not submitted valid values for 'trigger'.\n");
	 printf("These are the valid values for 'trigger': heartbeat,\t timer,\t none,\t default-on,\t mmc0,\t mmc1,\t oneshot\t ");
 }
}

void process_values_fed_to_brightness_file(char * value)
{
  /* ***  convert the string value to int *** */
  /* *** The int atoi(const char * string ) function resides in stdlib.h header and converts the string argument str to an int *** */

  int myvalue=atoi(value);
  switch(myvalue)
  {
  case 0:
	  write_values_to_brightness_file(enter_user_led, value);
	  break;
  case 1:
	  write_values_to_brightness_file(enter_user_led,value);
	  break;
  default:
	  printf("You entered an invalid value for 'brightness' \n");
	  printf("Make sure you are always using only one of the following values: 0;\t1.\n");

  }
}

/* *** This is the function main(int argc, char * argv[]) which is required by all C programs *** */
/* *************************************************************************************************
 * It is possible to pass some values from the command line to your C programs when they are executed.
 * These values are called command line arguments and are important for your program when it is executed.
 * You might want to control your program from outside instead of hard coding those values inside the code.
 * ****************************
 * The command line arguments are handled using main() function arguments
 * where argc refers to the number of arguments passed,
 * and argv[] is a pointer array which points to each argument passed to the program.
 * ****************************
 * It should be noted that argv[0] holds the name of the program itself and
 * argv[1] is a pointer to the first command line argument supplied
 *,while argv[n] is the last argument.
 *  ***************************
 *  So if no argument is passed, argc=1. If only one argument is passed argc=2.
 *  ***************************
 *  You pass all the arguments separated by space, but if you have an argument that contains space
 *  then you include this argument in single '' or double quotation marks.

*/
int main(int argc, char * argv[])
{
    enter_user_led=get_user_led_no_from_keyboard();



  if(argc!=3)
  {
	  printf("The complete usage of this application: %s <name_of_led_property> <value>\n", argv[0]);
	  printf("Valid names for the properties: brightness, trigger. \n");
	  printf("Valid values for 'brightness': 0,1\n ");
	  printf("Valid values for 'trigger' : heartbeat,timer,none,default-on,mmc0,mmc1,oneshot.\n");
  }
  else
  {
	  /* *** in this case the number of arguments submitted from command line is right, so to say: command_name command_parameter * ***/
	  /* *** string function strcmp(s1,s2) : returns 0 if s1 and s2 are the same, <0 if s1<s2 and >0 if s1>s2 *** */

	  if(strcmp(argv[1],"trigger")==0)
	  {

		  process_values_fed_to_trigger_file(argv[2]);
	  }
	  else if (strcmp(argv[1],"brightness")==0)
	  {
		  process_values_fed_to_brightness_file(argv[2]);
	  }
	  else
	  {
		  printf("You have not supplied correct properties.\n****************\n");
		  printf("********************************************");
		  printf("The valid control properties are: brightness,trigger.\n");
	  }

  }

  return 0;
}
