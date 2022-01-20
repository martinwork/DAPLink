/**
 * @file    storage.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright 2021 Micro:bit Educational Foundation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "storage.h"

#include "nrf_nvmc.h"

storage_status_t storage_program_flash(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    uint32_t *buf_32 = (uint32_t *)buf;
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_WRITE);
    for (uint32_t i = 0; i < sz / 4; i++) {
        ((volatile uint32_t *)adr)[i] = buf_32[i];
        while (!nrf_nvmc_ready_check(NRF_NVMC)) {
            // Wait for controller to be ready
        }
    }
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);

    return STORAGE_SUCCESS;
}


// Set to 1 to enable debugging
#define STORAGE_DEBUG     0

#if STORAGE_DEBUG
#include "daplink_debug.h"
#define storage_debug_msg    debug_msg
#else
#define storage_debug_msg(...)
#endif


#ifndef STORAGE_AVOID_ERASE
#define STORAGE_AVOID_ERASE 1
#endif


storage_status_t storage_erase_flash_page(uint32_t adr)
{
#if STORAGE_AVOID_ERASE
    bool empty = true;
    uint32_t *next = (uint32_t *) adr + 1024; 
    for ( uint32_t *p = (uint32_t *) adr; p < next; p++) {
        if ( *p != 0xFFFFFFFFu) {
            empty = false;
            break;
        }
    }
    if ( empty) {
        storage_debug_msg("[%x empty]\n", adr);
        return STORAGE_SUCCESS;
    }

    storage_debug_msg("[%x ERASE]\n", adr)
#endif

    // This operation can take up to 87.5ms
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_ERASE);
    nrf_nvmc_page_erase_start(NRF_NVMC, adr);
    while (!nrf_nvmc_ready_check(NRF_NVMC)) {
        // Wait for controller to be ready
    }
    nrf_nvmc_mode_set(NRF_NVMC, NRF_NVMC_MODE_READONLY);

    return STORAGE_SUCCESS;
}
