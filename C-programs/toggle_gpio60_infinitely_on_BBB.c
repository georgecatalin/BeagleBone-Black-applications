/*
===================================================================================================
Name: Program to toggle GPIO 60 written in C language
Author: George Calin (george.calin@gmail.com)
Version: 0.1
Description: This program toggles on/off an external LED on BeagleBone Black.
The GPIO number, time ON, time OFF are set upon execution of the program via command line arguments.
Date: September 3,2020
===================================================================================================
*/

/* **** List of error codes in C language given by the <errno.h> header
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


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h> // this header file embeds the stat() function which gets the properties of the file/directory

#define PATH_TO_GPIO_DIR "/sys/class/gpio/"

/* *** set the all valid numbers of GPIOs for BeagleBone Black *** */
const int valid_gpio[]={30,31,48,5,13,3,49,117,115,111,110,60,50,51,4,12,2,15,14,113,112,20,38,34,66,69,45,23,47,27,22,62,36,32,86,87,10,9,8,78,76,74,72,70,39,35,67,68,44,26,46,65,63,37,33,61,88,89,11,81,80,79,77,75,73,71};

int enter_gpio_set;
double seconds_led_on;
double seconds_led_off;
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

double convert_string_argument_to_double(char * myvalue, char * which_time)
{
	/* *** atof() is a function that converts a string to a floating point numerical representation (double). atof() is given by the <stdlib.h> header.
	 * If atof() can not return a double value, then it fails silently returning 0.0.
	 */
	double duration=atof(myvalue);
	printf(".........THE CONVERTED VALUE IS %f\n",duration);

	if (duration==0.0)
	{
		if (strcmp(which_time,"on")==0)
		{
			printf("Ooops..I can not convert the value you entered for LED Light ON to a double that I understand.\n");
			printf("Please make sure you are entering a valid value for DURATION(seconds) representation .....like 1 or 0.5 etc. \n\n");
		}
		else
		{
			printf("Ooops..I can not convert the value you entered for LED Light OFF to a double that I understand.\n");
			printf("Please make sure you are entering a valid value for DURATION(seconds) representation .....like 1 or 0.5 etc. \n\n");
		}

        exit(-1);
	}

	return duration;
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
		printf("The directory %s already exists.\n", buffer);
		printf("I do not need to construct the directory.\n\n");
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

	printf("I have opened the 'direction' file in gpio%d.\n",enter_gpio_set);
	printf("[..........................................]\n");

	return_from_write=write(fd,direction,strlen(direction));

	if(return_from_write<0)
	{
		printf("There was an error when writing to the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	printf("I have written in the file 'direction' of the gpio%d.\n",led_number);
	printf("[................................................]\n");

	if(close(fd)<0)
	{
		printf("There was an error when closing the file 'direction' in gpio%d. The error number is %d.\n",enter_gpio_set, errno);
		return (-1);
	}

	printf("I have closed the file 'direction' in gpio%d\n",enter_gpio_set);
	printf("[............................................................]\n");

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
		printf("There was an error when opening the file 'value' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	printf("I have opened the 'value' file in gpio%d.\n",enter_gpio_set);
	printf("[..........................................]\n");

	return_from_write=write(fd,value,strlen(value));

	if(return_from_write<0)
	{
		printf("There was an error when writing to the file 'value' in gpio%d. The error number is %d.\n",enter_gpio_set,errno);
		return (-1);
	}

	printf("I have written in the file 'value' of the gpio%d.\n",led_number);
	printf("[................................................]\n");

	if(close(fd)<0)
	{
		printf("There was an error when closing the file 'value' in gpio%d. The error number is %d.\n",enter_gpio_set, errno);
		return (-1);
	}

	printf("I have closed the file 'value' in gpio%d\n",enter_gpio_set);
	printf("[.......................................]\n");
	return (0);
}

int blink_external_led_on_bbb(double time_on,double time_off)
{
	write_to_direction_file(enter_gpio_set, "out");

	while(1==1)
	{
		write_to_value_file(enter_gpio_set, "1");
		sleep(time_on);
		write_to_value_file(enter_gpio_set, "0");
		sleep(time_off);
	}

	return (1);
}


/* *** only declaration of the functions. bodies are underneath main() *** */
int write_to_export_file(uint8_t led_no);


/* ********************************************************************************************
 *  user will specify the gpio number, property and value from command line upon executing the program
 *  I shall implement the following properties for the gpio: 'direction' and 'value'
 *  command will be like './program_name 60 direction out' or './program_name 60 value 1' >> this will fire up LED connected on GPIO60
 * *********************************************************************************************
 */

int main(int argc, char * argv[])
{
	/* *** set the global variable to the number of the gpio *** */
	enter_gpio_set=60;

	/* *** Verify if directory 'gpio?' exists under /sys/class/gpio.
	* If the directory does not exist, then one has to write the gpio number to the 'export' file to build the directory.
	* If the directory already exists from a previous iteration, then there is no need to do anything.
	* ***/
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

	/* ******** Wait for half a second to make sure the the 'export' file was properly closed, and the directory gpio? added to file system
	* If there is no time between the sequences, app returns error 13 * Permission Denied *
	* ********/

	sleep(0.5);

	/* *** Next, one wishes to achieve the flashing of the led which is direction='out' and alternating in an infinite loop value='1' ; value='0' *** */

		seconds_led_on=1;
		seconds_led_off=1;

		int activity_success=blink_external_led_on_bbb(seconds_led_on,seconds_led_off);



}

int write_to_export_file(uint8_t led_no)
{
	int fd; /* *** this is the file descriptor *** */
	int return_from_write;
	char path[]=PATH_TO_GPIO_DIR "export";
	char buffer_convert_int_to_string[3];

	snprintf(buffer_convert_int_to_string ,sizeof(buffer_convert_int_to_string),"%d",led_no);

	//printf("...................................%s\n",buffer_convert_int_to_string);

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


