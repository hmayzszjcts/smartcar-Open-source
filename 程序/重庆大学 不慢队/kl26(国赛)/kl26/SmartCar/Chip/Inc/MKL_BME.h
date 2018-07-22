/*
 * Copyright (c) 2012 Freescale Semiconductor, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY Freescale Semiconductor ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Freescale Semiconductor BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*  BME.h
    C Implemetation of the BME API based on Bit Manipulation engine Block Guide v0.3
*/

#ifndef __MKL_BME_H__
#define __MKL_BME_H__

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__)   //GNU compiler
#define _BME_INLINE __attribute__((always_inline))
#elif defined(__CWCC__) //Codewarrior compiler
#define _BME_INLINE __inline
#elif defined(__ICCARM__)   //IAR compiler
#define _BME_INLINE    static inline
#elif defined(__CC_ARM)     //Keil compiler
#define _BME_INLINE     __attribute__((always_inline))
#endif

#define BME_AND_MASK    (1<<26)
#define BME_OR_MASK     (1<<27)
#define BME_XOR_MASK    (3<<26)
#define BME_BFI_MASK(BIT,WIDTH)     (1<<28) | (BIT<<23) | ((WIDTH-1)<<19)
#define BME_LAC1_MASK(BIT)          (1<<27) | (BIT<<21)
#define BME_LAS1_MASK(BIT)          (3<<26) | (BIT<<21)
#define BME_UBFX_MASK(BIT,WIDTH)    (1<<28) | (BIT<<23) | ((WIDTH-1)<<19)


/* Decorated Store: 逻辑 与 AND */
void BME_AND_B(volatile uint8_t *addr, register uint8_t wdata);
void BME_AND_H(volatile uint16_t *addr, register uint16_t wdata);
void BME_AND_W(volatile uint32_t *addr, register uint32_t wdata);

/* Decorated Store: 逻辑 或 OR */
void BME_OR_B(volatile uint8_t *addr, register uint8_t wdata);
void BME_OR_H(volatile uint16_t *addr, register uint16_t wdata);
void BME_OR_W(volatile uint32_t *addr, register uint32_t wdata);

/* Decorated Store: 逻辑 异或 XOR */
void BME_XOR_B(volatile uint8_t *addr, register uint8_t wdata);
void BME_XOR_H(volatile uint16_t *addr, register uint16_t wdata);
void BME_XOR_W(volatile uint32_t *addr, register uint32_t wdata);

/* Decorated Store: 位插入 */
void BME_BFI_B(volatile uint8_t *addr, register uint8_t wdata, uint8_t bit, uint8_t width);
void BME_BFI_H(volatile uint16_t *addr, register uint16_t wdata, uint8_t bit, uint8_t width);
void BME_BFI_W(volatile uint32_t *addr, register uint32_t wdata, uint8_t bit, uint8_t width);

/* Decorated Load: 加载并清1位 */
uint8_t BME_LAC1_B(volatile uint8_t *addr, uint8_t bit);
uint16_t BME_LAC1_H(volatile uint16_t *addr, uint8_t bit);
uint32_t BME_LAC1_W(volatile uint32_t *addr, uint8_t bit);

/* Decorated Load: 加载并置1位 */
uint8_t BME_LAS1_B(volatile uint8_t *addr, uint8_t bit);
uint16_t BME_LAS1_H(volatile uint16_t *addr, uint8_t bit);
uint32_t BME_LAS1_W(volatile uint32_t *addr, uint8_t bit);

/* Decorated Load: 无符号提前位段 */
uint8_t BME_UBFX_B(volatile uint8_t *addr, uint8_t bit, uint8_t width);
uint16_t BME_UBFX_H(volatile uint16_t *addr, uint8_t bit, uint8_t width);
uint32_t BME_UBFX_W(volatile uint32_t *addr, uint8_t bit, uint8_t width);

/*********************   逻辑 与  ***********************/
_BME_INLINE void BME_AND_W (volatile uint32_t *addr, register uint32_t wdata)
{
    *(volatile uint32_t*)((uint32_t)addr | BME_AND_MASK) = wdata;
}

_BME_INLINE void BME_AND_H (volatile uint16_t *addr, register uint16_t wdata)
{
    *(volatile uint16_t*)((uint32_t)addr | BME_AND_MASK) = wdata;
}

_BME_INLINE void BME_AND_B (volatile uint8_t *addr, register uint8_t wdata)
{
    *(volatile uint8_t*)((uint32_t)addr | BME_AND_MASK) = wdata;
}

/*********************   逻辑 或  ***********************/
_BME_INLINE void BME_OR_W (volatile uint32_t *addr, register uint32_t wdata)
{
    *(volatile uint32_t*)((uint32_t)addr | BME_OR_MASK) = wdata;
}

_BME_INLINE void BME_OR_H (volatile uint16_t *addr, register uint16_t wdata)
{
    *(volatile uint16_t*)((uint32_t)addr | BME_OR_MASK) = wdata;
}

_BME_INLINE void BME_OR_B (volatile uint8_t *addr, register uint8_t wdata)
{
    *(volatile uint8_t*)((uint32_t)addr | BME_OR_MASK) = wdata;
}

/*********************   逻辑 异或  ***********************/
_BME_INLINE void BME_XOR_W (volatile uint32_t *addr, register uint32_t wdata)
{
    *(volatile uint32_t*)((uint32_t)addr | BME_XOR_MASK) = wdata;
}

_BME_INLINE void BME_XOR_H (volatile uint16_t *addr, register uint16_t wdata)
{
    *(volatile uint16_t*)((uint32_t)addr | BME_XOR_MASK) = wdata;
}

_BME_INLINE void BME_XOR_B (volatile uint8_t *addr, register uint8_t wdata)
{
    *(volatile uint8_t*)((uint32_t)addr | BME_XOR_MASK) = wdata;
}

/*********************   位插入  ***********************/
/* The width field is encoded as (width-1) */
_BME_INLINE void BME_BFI_W (volatile uint32_t *addr, register uint32_t wdata, uint8_t bit, uint8_t width)
{
    *(volatile uint32_t*)((uint32_t)addr | BME_BFI_MASK(bit,width)) = wdata;
}

_BME_INLINE void BME_BFI_H (volatile uint16_t *addr, register uint16_t wdata, uint8_t bit, uint8_t width)
{
    *(volatile uint16_t*)((uint32_t)addr | BME_BFI_MASK(bit,width)) = wdata;
}

_BME_INLINE void BME_BFI_B (volatile uint8_t *addr, register uint8_t wdata, uint8_t bit, uint8_t width)
{
    *(volatile uint8_t*)((uint32_t)addr | BME_BFI_MASK(bit,width)) = wdata;
}

_BME_INLINE uint32_t BME_LAC1_W (volatile uint32_t *addr, uint8_t bit)
{
    return *(volatile uint32_t*)((uint32_t)addr | BME_LAC1_MASK(bit));
}

_BME_INLINE uint16_t BME_LAC1_H (volatile uint16_t *addr, uint8_t bit)
{
    return *(volatile uint16_t*)((uint32_t)addr | BME_LAC1_MASK(bit));
}

_BME_INLINE uint8_t BME_LAC1_B (volatile uint8_t *addr, uint8_t bit)
{
    return *(volatile uint8_t*)((uint32_t)addr | BME_LAC1_MASK(bit));
}

_BME_INLINE uint32_t BME_LAS1_W (volatile uint32_t *addr, uint8_t bit)
{
    return *(volatile uint32_t*)((uint32_t)addr | BME_LAS1_MASK(bit));
}

_BME_INLINE uint16_t BME_LAS1_H (volatile uint16_t *addr, uint8_t bit)
{
    return *(volatile uint16_t*)((uint32_t)addr | BME_LAS1_MASK(bit));
}

_BME_INLINE uint8_t BME_LAS1_B (volatile uint8_t *addr, uint8_t bit)
{
    return *(volatile uint8_t*)((uint32_t)addr | BME_LAS1_MASK(bit));
}

/* The width field is encoded as (width-1) */
_BME_INLINE uint32_t BME_UBFX_W (volatile uint32_t *addr, uint8_t bit, uint8_t width)
{
    return *(volatile uint32_t*)((uint32_t)addr | BME_UBFX_MASK(bit,width));
}

_BME_INLINE uint16_t BME_UBFX_H (volatile uint16_t *addr, uint8_t bit, uint8_t width)
{
    return *(volatile uint16_t*)((uint32_t)addr | BME_UBFX_MASK(bit,width));
}

_BME_INLINE uint8_t BME_UBFX_B (volatile uint8_t *addr, uint8_t bit, uint8_t width)
{
    return *(volatile uint8_t*)((uint32_t)addr | BME_UBFX_MASK(bit,width));
}

#endif /* __MKL_BME_H__ */
