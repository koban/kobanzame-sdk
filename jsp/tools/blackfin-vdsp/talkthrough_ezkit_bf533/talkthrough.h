/*
 *	talkthrough.h
 */
#ifndef _TSKDEF_H_
#define _TSKDEF_H_
#include "ad1836a.h"

#ifdef __cplusplus
  extern "C" {
#endif

extern void processData( struct CODEC_BUFFER *rxBuf, struct CODEC_BUFFER *txBuf );
extern void app_init(VP_INT vp_int);

#ifdef __cplusplus
  };
#endif

#endif

