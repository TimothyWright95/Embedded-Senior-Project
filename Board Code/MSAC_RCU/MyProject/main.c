#include <atmel_start.h>
#include "mytasks.h"
int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	Create_all_tasks();
	/* Replace with your application code */
	while (1) {

	}
}
