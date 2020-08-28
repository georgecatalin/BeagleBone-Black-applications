	/*
==========================================================
Name: Program to control the GPIOs of BeagleBone Black written in C language
Author: George Calin (george.calin@gmail.com)
Version: 0.1
Description: This program controls the GPIOs of the BeagleBone Black allowing
 to experiment various layouts of LEDs (digital output devices) on the breadboard
Date: August 28,2020
==========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> // this is header file is mention because it includes the stat() function

#define PATH_TO_GPIO_DIR "/sys/class/gpio/"

/* *** set the all valid numbers of GPIOs for BeagleBone Black *** */
const int valid_gpio[]={30,31,48,5,13,3,49,117,115,111,110,60,50,51,4,12,2,15,14,113,112,20,38,34,66,69,45,23,47,27,22,62,36,32,86,87,10,9,8,78,76,74,72,70,39,35,67,68,44,26,46,65,63,37,33,61,88,89,11,81,80,79,77,75,73,71};

int enter_gpio_set;
extern int errno;


int get_size_array_valid_gpio()
{
	size_t size_array= sizeof(valid_gpio)/sizeof(int);
	return size_array;
}

/* ***  I am verifying if the input number correspond to a valid GPIO number as defined by BeagleBone Black specifications *** */
int verify_if_number_is_valid_gpio(int number)
{
	int size=get_size_array_valid_gpio();

	for (int i = 0; i < size; i++)
	{
		if(valid_gpio[i]==number)
		{
			printf("%d exists in the list with valid GPIOs for BeagleBone Black.\n", number);
			printf("*************************************************************\n");
			printf("Thank you.\n");
			printf("*************************************************************\n");
			return number;
		}
	}
	return (-1);
}

int verify_if_directory_gpionumber_exists(uint8_t led_number)
{
	/* *** Construct the name of the directory *** */
	char buffer[100];

	snprintf(buffer,sizeof(buffer),PATH_TO_GPIO_DIR "gpio%d",led_number);

	struct stat file_properties;
	/* *** with stat() from <sys.stat.h> one can get file properties like file permission, creation and modification date, size *** */

	stat(buffer,&file_properties);

	if(S_ISDIR(file_properties.st_mode))
	{
		printf("The directory %s exists.\n", buffer);
		return (1);
	}
	else
	{
		printf("The directory %s does not exist.\n",buffer);
		printf("I shall create this directory for you and make the necessary adjustments to handle the gpio%d.\n",led_number);
		return (0);
	}

}

int write_to_direction_file(uint8_t led_number, char * direction)
{
	int fd; //file descriptor
	int return_from_write; //this integer will be returned as result from write() function. if -1 it means unsuccessful and error
	char buffer[100]; //this will be used in the snprintf() function

	snprintf(buffer,sizeof(buffer),PATH_TO_GPIO_DIR "gpio%d/direction",enter_gpio_set);

	/* *** open file 'direction' *** */
	fd=open(buffer,O_WRONLY);

	if(fd==-1)
	{
		printf("There was an error when opening the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	return_from_write=write(fd,direction,strlen(direction));

	if(return_from_write<0)
	{
		printf("There was an error when writing to the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	printf("I have written in the file 'direction' of the gpio%d",led_number);
	return (0);
}

int write_to_value_file(uint8_t led_number, char * value)
{
	int fd; //file descriptor
	int return_from_write; //this integer will be returned as result from write() function. if -1 it means unsuccessful and error
	char buffer[100]; //this will be used in the snprintf() function

	snprintf(buffer,sizeof(buffer),PATH_TO_GPIO_DIR "gpio%d/value",enter_gpio_set);

	/* *** open file 'direction' *** */
	fd=open(buffer,O_WRONLY);

	if(fd==-1)
	{
		printf("There was an error when opening the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	return_from_write=write(fd,value,strlen(value));

		if(return_from_write<0)
		{
			printf("There was an error when writing to the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
			return (-1);
		}

		printf("I have written in the file 'value' of the gpio%d",led_number);
		return (0);
}

void process_parameter_of_direction(char * direction)
{
	if ((strcmp(direction,"out") && strcmp(direction,"in")))
	{
		/* *** this option happens when direction is not 'out' and direction is not 'in' *** */
		printf("You did not submit a valid option for direction.\n");
		printf("The valid options for direction are: out, in.\n");
	}
	else
	{
		/* *** this option happens when direction is either 'out' or direction is  'in' *** */
		write_to_direction_file(enter_gpio_set,direction);
	}
}

void process_parameter_of_value(char * value)
{
	if ((strcmp(value,"0") && strcmp(value,"1")))
		{
			/* *** this option happens when direction is not '0' and direction is not '1' *** */
			printf("You did not submit a valid option for value.\n");
			printf("The valid options for direction are: 0, 1.\n");
		}
		else
		{
			/* *** this option happens when direction is either '0' or direction is  '1' *** */
			write_to_value_file(enter_gpio_set,value);
		}
}

int write_to_export_file(uint8_t led_no);

/* ********************************************************************************************
 *  user will specify the gpio number, property and value from command line upon executing the program
 *  I shall implement the following properties for the gpio: 'direction' and 'value'
 *  command will be like './program_name 60 direction out' or './program_name 60 value 1' >> this will fire up LED connected on GPIO60
 * *********************************************************************************************
 */

int main(int argc, char * argv[])
{
	if(argc!=4)
	{
		printf("Ooops...it seems you did not specify the complete command for opening the application.\n");
		printf("The completed usage of this application in command line is: %s <number_of_gpio> <property_of_the_gpio> <value_for_the_property>.\n",argv[0]);
		printf("Valid uses for <property_of_the_gpio>: direction, value.\n");
		printf("Valid uses for <value_for_the_property>: out, in, 0, 1.\n");
	}
	else
	{

		int converted_to_int=atoi(argv[1]);

			int valid_gpio=verify_if_number_is_valid_gpio(converted_to_int);

			if(valid_gpio==-1)
			{
				printf("Ooops...there is an error with the value you entered for the gpio number: %s\n",argv[1]);
				printf("Make sure you enter a number value which corresponds to a valid GPIO on the BeagleBone Black\n");
				exit (-1);
			}

			enter_gpio_set=valid_gpio;

			int existsDirectory=verify_if_directory_gpionumber_exists(enter_gpio_set);

			if(existsDirectory)
			{
				printf("I do not need to write in the file 'export'.\n");
			}
			else
			{
				printf("I need to write '%d' in the file 'export'.\n",enter_gpio_set);
				write_to_export_file(enter_gpio_set);
			}

			/* *** wait for half a second to make sure the the 'export' file was properly closed, and the directory gpio? added to file system
			 * If there is no time between the sequences, app returns error 13 * Permission Denied *
			 * List of error codes in C language given by the <errno.h> header
			 * ***************************************************************
			 * errno value       Error
1            * Operation not permitted *
2            * No such file or directory *
3            * No such process *
4            * Interrupted system call *
5            * I/O error *
6            * No such device or address *
7            * Argument list too long *
8            * Exec format error *
9            * Bad file number *
10           * No child processes *
11           * Try again *
12           * Out of memory *
13           * Permission denied *
			 * ***************************************************************
			 */

			sleep(0.5);

			if (strcmp(argv[2],"direction")==0)
			{
				process_parameter_of_direction(argv[3]);
			}
			else if(strcmp(argv[2],"value")==0)
			{
				process_parameter_of_value(argv[3]);
			}
			else
			{
				printf("Ooops..you have entered incorrect properties.\n");
				printf("You should have entered one of these properties: direction, value.\n");
				printf("see two examples below: \n");
				printf("%s 60 direction out or \n %s 88 value 0.\n",argv[0],argv[0]);
			}
	}

}

int write_to_export_file(uint8_t led_no)
{
	int fd; /* *** this is the file descriptor *** */
	int return_from_write;
	char path[]=PATH_TO_GPIO_DIR "export";
	char buffer_convert_int_to_string[3];

	snprintf(buffer_convert_int_to_string ,sizeof(buffer_convert_int_to_string),"%d",led_no);

	printf("...................................%s\n",buffer_convert_int_to_string);

	fd=open(path,O_WRONLY);

	if(fd==-1)
		{
			printf("There was an error when opening the file 'export' in %s. The error number is %d.\n",path,errno);
			return (-1);
		}

	return_from_write=write(fd,buffer_convert_int_to_string,strlen(buffer_convert_int_to_string));


	if(return_from_write<0)
	{
		printf("There was an error when writing to the file 'export' in gpio. The error number is %d.\n",errno);
		return (-1);
	}

	printf("I have written in the file 'export' of the gpio.\n\n");
	return (0);
}

