#include "t_services.h"
#include "ad1836a.h"
#include <cdefBF533.h>
#include <ccblkfn.h>

void sport0_handler(void)
{
	*pDMA1_IRQ_STATUS = DMA_DONE;		// SPORT0 RX DMA割り込みをクリア
	ssync();
	isig_sem( sport0_sem );
}

