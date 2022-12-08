#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LENGTH 100
double avg;
int min, max;

void *calcMaximum(void *param)
{
}

void *calcMinimum(void *param)
{
}

void *calcAverage(void *param)
{
}

int main(int argc, char *argv[])
{
    int buffer[MAX_LENGTH];
    
    if (argc > 1)
    {
        // Copy the data from argv to the buffer array
    }
    else
    {
        return 1;
    }

    printf("The average value is %f\n", avg);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);

    return 0;
}