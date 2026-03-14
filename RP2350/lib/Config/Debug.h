/*****************************************************************************
 * | File        :   Debug.h
 * | Author      :   Waveshare team (ported to RP2350)
 * | Function    :   Debug output
 * | Info        :   Uses Pico SDK printf (routed via USB)
 ******************************************************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_


#define USE_DEBUG 1

#if USE_DEBUG
#define Debug(__info, ...) printf("Debug: " __info, ##__VA_ARGS__)
#else
#define Debug(__info, ...)
#endif

#endif
