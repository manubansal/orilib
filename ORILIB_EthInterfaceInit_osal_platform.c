#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ti/platform/platform.h>
#include <osl/inc/swpform.h>

/* OSAL functions for Platform Library */
uint8_t *Osal_platformMalloc (uint32_t num_bytes, uint32_t alignment)
{
	printf("osal_platformMalloc called!\n");
	exit(1);
	return malloc(num_bytes);
}

void Osal_platformFree (uint8_t *dataPtr, uint32_t num_bytes)
{
    printf("osal_platformFree called!\n");
    exit(1);
    /* Free up the memory */
    if (dataPtr)
    {
        free(dataPtr);
    }
}
