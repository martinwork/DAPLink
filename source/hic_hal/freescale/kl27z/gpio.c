/**
 * @file    gpio.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "gpio.h"

static void busy_wait(uint32_t cycles)
{
    volatile uint32_t i;
    i = cycles;

    while (i > 0) {
        i--;
    }
}

void gpio_init(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
    // configure pin as GPIO
    PIN_HID_LED_PORT->PCR[PIN_HID_LED_BIT] = PORT_PCR_MUX(1);
    PIN_MSC_LED_PORT->PCR[PIN_MSC_LED_BIT] = PORT_PCR_MUX(1);
    PIN_CDC_LED_PORT->PCR[PIN_CDC_LED_BIT] = PORT_PCR_MUX(1);
    PIN_KILLME_PORT->PCR[PIN_KILLME_BIT]   = PORT_PCR_MUX(1);
    PIN_LOADDUMP_PORT->PCR[PIN_LOADDUMP_BIT] = PORT_PCR_MUX(1);
    PIN_SW_RESET_PORT->PCR[PIN_SW_RESET_BIT] = PORT_PCR_MUX(1);
    PIN_RED_LED_PORT->PCR[PIN_RED_LED_BIT] = PORT_PCR_MUX(1);
    PIN_RUN_VMON_BAT_PORT->PCR[PIN_RUN_VMON_BAT_BIT] = PORT_PCR_MUX(1);
    // led off
    gpio_set_hid_led(GPIO_LED_OFF);
    gpio_set_cdc_led(GPIO_LED_OFF);
    gpio_set_msc_led(GPIO_LED_OFF);
//    gpio_set_red_led(GPIO_LED_OFF);
    // Power regulator signals off
    gpio_set_killme(GPIO_OFF);
    gpio_set_loaddump(GPIO_OFF);
//    // Voltage monitoring enable off
//    gpio_set_run_vbat_sense(GPIO_OFF);
    // Set as output
    PIN_HID_LED_GPIO->PDDR  |= PIN_HID_LED;
    PIN_MSC_LED_GPIO->PDDR  |= PIN_MSC_LED;
    PIN_CDC_LED_GPIO->PDDR  |= PIN_CDC_LED;
    PIN_KILLME_GPIO->PDDR   |= PIN_KILLME;
    PIN_LOADDUMP_GPIO->PDDR |= PIN_LOADDUMP;
    PIN_RED_LED_GPIO->PDDR |= PIN_RED_LED;
    PIN_RUN_VMON_BAT_GPIO->PDDR |= PIN_RUN_VMON_BAT;
    // set as input
    PIN_SW_RESET_GPIO->PDDR &= ~PIN_SW_RESET;

    // Let the voltage rails stabilize.  This is especailly important
    // during software resets, since the target's 3.3v rail can take
    // 20-50ms to drain.  During this time the target could be driving
    // the reset pin low, causing the bootloader to think the reset
    // button is pressed.
    // Note: With optimization set to -O2 the value 1000000 delays for ~85ms
    busy_wait(1000000);
}

void gpio_set_hid_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (PIN_HID_LED_GPIO->PSOR = PIN_HID_LED) : (PIN_HID_LED_GPIO->PCOR = PIN_HID_LED);
}

void gpio_set_cdc_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (PIN_CDC_LED_GPIO->PSOR = PIN_CDC_LED) : (PIN_CDC_LED_GPIO->PCOR = PIN_CDC_LED);
}

void gpio_set_msc_led(gpio_led_state_t state)
{
    (GPIO_LED_ON == state) ? (PIN_MSC_LED_GPIO->PSOR = PIN_MSC_LED) : (PIN_MSC_LED_GPIO->PCOR = PIN_MSC_LED);
}

//void gpio_set_red_led(gpio_led_state_t state)
//{
//    (GPIO_LED_ON == state) ? (PIN_RED_LED_GPIO->PSOR = PIN_RED_LED) : (PIN_RED_LED_GPIO->PCOR = PIN_RED_LED);
//}

void gpio_set_killme(gpio_state_t state)
{
    (GPIO_ON == state) ? (PIN_KILLME_GPIO->PSOR = PIN_KILLME) : (PIN_KILLME_GPIO->PCOR = PIN_KILLME);
}

void gpio_set_loaddump(gpio_state_t state)
{
    (GPIO_ON == state) ? (PIN_LOADDUMP_GPIO->PSOR = PIN_LOADDUMP) : (PIN_LOADDUMP_GPIO->PCOR = PIN_LOADDUMP);
}

//void gpio_set_run_vbat_sense(gpio_state_t state)
//{
//    (GPIO_ON == state) ? (PIN_RUN_VMON_BAT_GPIO->PSOR = PIN_RUN_VMON_BAT) : (PIN_RUN_VMON_BAT_GPIO->PCOR = PIN_RUN_VMON_BAT);
//}

uint8_t gpio_get_reset_btn_no_fwrd(void)
{
    return 0;
}

uint8_t gpio_get_reset_btn_fwrd(void)
{
    return (PIN_SW_RESET_GPIO->PDIR & PIN_SW_RESET) ? 0 : 1;
}

void gpio_set_board_power(bool powerEnabled)
{
}
