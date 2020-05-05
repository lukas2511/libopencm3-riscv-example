/*
 * Copyright (c) 2020, Martin Lund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>

extern uint32_t  _data_start;
extern uint32_t  _data_end;
extern uint32_t  _data_load;
extern uint32_t  _bss_start;
extern uint32_t  _bss_end;
extern uint32_t  _stack_end;
extern uint32_t  _global_pointer;
extern uint32_t  _start_vectors;

extern void main(void);

__attribute__((section(".init"), naked))
void reset(void)
{
    register uint32_t *src, *dst;

    /* Setup global pointer and stack */
    asm volatile("la gp, _global_pointer");
    asm volatile("la sp, _stack_end");

    /* Set up Enhanced CLIC for vectored interrupt table */
    asm volatile("csrw mtvec, %0":: "r"((uint8_t *)(&_start_vectors) + 0x1));

    /* Copy the .data section (initialized data) from flash to RAM */
    src = (uint32_t *) &_data_load;
    dst = (uint32_t *) &_data_start;
    while (dst < (uint32_t *)&_data_end)
    {
        *dst++ = *src++;
    }

    /* Zero initialize the BSS section (zero initialized data) */
    dst = &_bss_start;
    while (dst < (uint32_t *)&_bss_end)
    {
        *dst++ = 0U;
    }

    /* Call main() */
    main();
}
