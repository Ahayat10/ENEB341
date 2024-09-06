#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

int main ()
{
    int handle;
    int dataAvailable;
    int read;
    char buf[1000];

    /* TODO: In the if statement, initialize the GPIO library and check if it has properly initialized*/
    if ()
    {
        printf("initialization failed\n");
    }
    else
    { 
        printf("initialization ok\n");
    }

    /* TODO: handle should open the serial device using the correct serial device address
     * HINT: use ls -l /dev/serial* in the terminal to find the device and make sure to set the baud rate correctly*/
    handle = ; 
  
    if (handle >= 0)
    {
        printf("opened ok\n");
    }
    else 
    {
        printf("failed to open\n");
    } 

    while (1)
    {
        /* TODO: check if there is data available */
        dataAvailable = ;

        while (dataAvailable > 0)
        {
            /* TODO: Read the data */
            read = ;
            
            if (read >= 0)
            {
                /* TODO: Print the data */
                
            }
        }
    }

    return 0;
}
