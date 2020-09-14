
/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "diag/Trace.h"
#include "gd32f10x_libopt.h"

//freeRTOS includes
#include "FreeRTOS.h"
#include "task.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void rcu_config(void);
void gpio_config(void);
void Toggle_led(void);
void delay(uint32_t count);

TaskHandle_t task1Handle = NULL;

void task1(void *p){

	for(uint8_t i = 0; i < 15; i++){
		Toggle_led();
		vTaskDelay(8000);
	}

	vTaskDelete(task1Handle);

	while(1);
	return;
}

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	rcu_config();
	gpio_config();

  //RTOS task create
	xTaskCreate(task1, "task 1", 200, (void*) 0, tskIDLE_PRIORITY + 1, &task1Handle);
  //RTOS start scheduler
	vTaskStartScheduler();

	// Infinite loop
  while (1)
    {

    }

  return 0;
}

void rcu_config(void){
	rcu_system_clock_source_config(RCU_CKSYSSRC_HXTAL);
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1);
	rcu_apb1_clock_config(RCU_APB1_CKAHB_DIV1);
	rcu_apb2_clock_config(RCU_APB2_CKAHB_DIV1);

	rcu_periph_clock_enable(RCU_GPIOC);
	return;
}

void gpio_config(void){
	gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
	gpio_bit_set(GPIOC, GPIO_PIN_6);

	return;
}

void Toggle_led(void){
	if(gpio_output_bit_get(GPIOC, GPIO_PIN_6)){
		gpio_bit_reset(GPIOC, GPIO_PIN_6);
		return;
	}

	gpio_bit_set(GPIOC, GPIO_PIN_6);
	return;
}

void delay(uint32_t count){
	for (uint32_t i = 0; i < count; i++){
		asm("nop");
	}

	return;
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
