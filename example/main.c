/**
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "app_util_platform.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "transfer_handler.h"
#include "st7302.h"

unsigned char BMP1[4125] = { 
	  0xEA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,  
    0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xB0,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x55,0x55,0x55,0x55,0x40,  
    0x30,0x00,0x00,0xC0,0xE4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0xC2,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xAA,0xAA,0xAA,0xAA,0x80,0x30,0x00,0x00,0xC0,0xD5,0x75,  
    0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x15,  
    0x55,0x55,0x40,0x30,0x00,0x00,0xC0,0xC0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0xC6,0x00,0x2A,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xEA,0xBF,0xC0,0x30,0x00,0x00,  
    0xC0,0xA0,0x00,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x0A,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x2A,0xAA,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF5,0x7F,0xC0,0x3F,0xC0,  
    0x30,0x00,0x00,0xC0,0x00,0xAA,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xAA,0xAA,0x80,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x7F,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x05,0x55,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0xFF,0xFF,  
    0xF0,0x00,0x00,0x00,0x00,0x00,0x05,0x50,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x36,0xFF,0xE7,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0xAA,0xAF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,  
    0xC0,0x3F,0x6E,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x3F,0xF3,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,  
    0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x3F,0xF3,0xFF,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,  
    0x30,0x00,0x00,0xC0,0x3F,0xF3,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0x00,0x0F,0xF0,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x3F,0xF3,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x03,0xFF,0xFF,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x2A,0x80,0x30,0x00,0x00,0xC0,0x3F,0xE6,  
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xAA,0xAA,0xAA,0x00,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x3E,0x7F,0x6F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x00,0x00,0x00,0x15,0x40,0x30,0x00,0x00,0xC0,0x27,0xFF,0xF6,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x0F,0xD0,0x00,0x00,0x7F,0xC0,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xFF,0x40,0x1F,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0xAA,0x80,0x00,0x00,0x00,0x00,0x00,0x02,0xBF,0xF5,0xFF,0xFA,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x55,0x55,0x55,0x55,0x55,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x0B,0xFF,0xFE,0x80,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xD0,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x07,0xFF,0xEA,0xBF,0xFD,0x40,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFE,0x00,  
    0x0A,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x54,0x00,0x00,0x00,0x05,  
    0x50,0x00,0x00,0x00,0x00,0x00,0x0F,0x80,0x00,0x00,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x28,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,  
    0x05,0x50,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,  
    0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0xFF,0x00,0x0F,0xF0,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x03,0xFD,0x55,0xFF,0x55,0x5F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,  
    0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,  
    0xE0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x2A,0x80,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x05,0x55,0x55,0x45,0x55,0x40,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xCF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFA,0xBF,0xCA,0xBF,0xC0,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,  
    0xD5,0x7F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  
    0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,  
    0xFF,0xF0,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0A,0xA0,0x2A,0xAA,0xAA,0x80,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,  
    0x05,0x55,0x55,0x55,0x55,0x40,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0xFF,0xFF,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFA,0xAA,0xAA,0xBF,0xC0,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFE,0xAA,0xAA,0xA0,0x00,0xFF,0xFF,0xFE,0xAA,0xAA,0xAB,0xFF,0xFF,0xF0,  
    0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,  
    0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xF5,0x55,  
    0x55,0x7F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,  
    0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,  
    0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0A,0xAA,0xAA,0xAA,0xAA,0x80,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,  
    0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,  
    0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,  
    0x00,0x00,0x00,0x00,0x00,0x05,0x50,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,  
    0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,  
    0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFD,0x55,0x55,0x55,0x55,0xFF,0xFF,0xFC,0x00,0x00,0x03,  
    0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,  
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,  
    0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0x00,0x00,0x03,0xFF,0xFF,0xF0,  
    0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,  
    0xF0,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,  
    0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x55,0x55,0x54,0x00,  
    0x01,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x57,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,  
    0xFF,0xFC,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x05,0x55,0x40,0x05,0x55,0x40,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,  
    0x00,0x00,0x00,0x0F,0xFF,0xF4,0x0F,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFC,0x00,0x03,0xFF,0xFF,  
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x0F,0xFA,0xFD,0x0A,0xBF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0xBF,  
    0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF5,0x43,0xFD,0x7F,0xC0,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xFF,0xC0,0xBF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xAA,0x80,0x2A,0xAA,0x80,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x55,0x55,0x55,0x55,0x55,0x50,0x00,0x00,0x00,0x00,0x00,0x05,0x50,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,  
    0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,  
    0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xF0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0A,0xA0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x05,0x50,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x55,0x55,0x55,0x55,0x55,0x50,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x5F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,  
    0x5F,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x0F,  
    0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF1,0x7F,0xFF,0xE8,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xE8,0x00,0x00,0x30,0x00,0x00,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xFF,0xA0,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,  
    0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x0A,0xA0,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFC,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xF0,0x05,0x50,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x07,0xD5,0x55,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x2F,0xFF,  
    0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x55,0x55,0x55,0x55,0x55,0x50,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,  
    0xF0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x02,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xFF,0xFF,0xFF,  
    0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0xD0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x03,0xFA,0xAA,0xAA,0xAA,0xAB,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x0F,0xF0,0x3F,0xC0,0x30,0x00,0x00,  
    0xC0,0x3F,0xFF,0xFD,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x2A,0xAA,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF0,0x00,  
    0x00,0x00,0x03,0xF0,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x80,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFD,0x40,0x00,0x00,0x1F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFD,0x55,0x57,0xFF,0xC0,0x00,0x00,  
    0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x2F,0xFF,0xFF,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x00,0xEA,  
    0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xBF,0xFF,0xFF,0xA0,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,  
    0x00,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x3A,0xAA,0xAB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x33,0xFF,0xF3,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xC0,0x30,0x00,0x00,  
    0xC0,0x33,0xFF,0xF3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x31,0x55,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x33,0xFF,0xF3,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x32,0xAA,0xA3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x15,0x55,0x55,0x40,0x30,0x00,0x00,0xC0,0x33,0xFF,0xF3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xFF,0xFF,0xC0,0x30,0x00,0x00,0xC0,0x33,0xFF,  
    0xF3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,  
    0xEA,0xBF,0xC0,0x30,0x00,0x00,0xC0,0x31,0x55,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x33,0xFF,0xF3,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x3F,0xC0,0x3F,0xC0,0x30,0x00,0x00,  
    0xC0,0x32,0xAA,0xA3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x0F,0xF5,0x7F,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x2A,0xAA,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xC0,0x3F,0xC0,0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0xAA,0xAA,0x80,0x00,0x00,  
    0x30,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0xD5,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,  
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x70,0x00,0x00,
};

int main(void)
{
	
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
	NRF_LOG_INFO("ST7302 example started.");
	NRF_LOG_FLUSH();
    
    st7302_begin();
	
    st7302_addrSet(0,0,10,0x7c);
//    uint8_t sendd[125*11*3];
//    memset(sendd, 0xff, 125*11*3);
//    sendd[33] = 0xff;
    st7302_writeData(BMP1, 125*11*3);
    delay(1000);

    while (1)
    {
        NRF_LOG_FLUSH();
        __WFE();
        
//        sendd[0] <<= 1;
//    st7302_writeData(sendd, 1);
//        delay(1000);

    }
}
