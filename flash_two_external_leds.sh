#!/bin/bash
# A script to flash a LED connected to a GPIO of the BeagleBone Black using Bash shell scripting
# By Arnold the Crocodile

# Verify if the needed gpio directory already exists, and if not, create it with the export file
if [ -d "/sys/class/gpio/gpio60" ]
then
        echo "'gpio60' directory already exists.there is no need to create it by writing to 'export'."
else
        echo "'gpio60' directory does not exist.there is need to create it by writing in  the 'export' file"
        echo 60 > /sys/class/gpio/export
fi

if [ -d "/sys/class/gpio/gpio48" ]
then
        echo "'gpio48' directory already exists.there is no need to create it by writing to 'export'."
else
        echo "'gpio48' directory does not exist.there is need to create it by writing in  the 'export' file"
        echo 48 > /sys/class/gpio/export
fi


echo "out" > /sys/class/gpio/gpio60/direction
echo "out" > /sys/class/gpio/gpio48/direction

while  true
 do
        sleep .5
        echo  "entering the while.set the GPIO60 value to 1"
        echo 1 > /sys/class/gpio/gpio60/value
        echo "done"
        echo "set the GPIO48 value to 0"
        echo 0 > /sys/class/gpio/gpio48/value
        echo  "done"
        sleep .5

        echo "set the GPIO60  vslue to 0"
        echo 0 > /sys/class/gpio/gpio60/value
        echo "done"
        echo "set the GPIO48 value to 1"
        echo 1 >  /sys/class/gpio/gpio48/value
        echo "done"
done
