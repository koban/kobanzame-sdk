#include "t_services.h"
#include "kernel_id.h"
#include "talkthrough.h"
#include "ad1836a.h"


void processData(struct CODEC_BUFFER *rx_buf, struct CODEC_BUFFER *tx_buf)
{
	int sample, slot;
	

	for ( sample = 0; sample < SAMPLE_PER_INTR; sample++ )
		for ( slot =0; slot < 8; slot ++ )
			tx_buf->data[sample][slot] = rx_buf->data[sample][slot];
	
}
