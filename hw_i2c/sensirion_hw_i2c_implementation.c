/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_arch_config.h"
#include "sensirion_i2c.h"
#include "sensirion_common.h"

#include "gd32f10x.h"

uint32_t tick_counter = 0;
/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_select_bus(uint8_t bus_idx) {
    // IMPLEMENT or leave empty if all sensors are located on one single bus
    return STATUS_FAIL;
}

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init(void) {
    //gpio init
    gpio_pin_remap_config(GPIO_I2C0_REMAP, DISABLE);                                         //remap i2c0 to pins 6 and 7 in PORT B
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_ 6);                       //configure alternate function open drain for pin6
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_ 7);                       //configure alternate function open drain for pin7
    
    //i2c init
    slave_addr_7BIT = 0x82U;                                                                 //Own address
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7bits, slave_addr_7BIT);    //configure own address, address size and i2c mode
    i2c_enable(I2C0);                                                                        //enable i2c
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);                                                    //enable ack
    i2c_stretch_scl_low_config(I2C0, I2C_SCLSTRETCH_ENABLE);                                 //Enable clock stretching
 
    //timer init
    timer_parameter_struct TIMERx_config;
    
    TIMERx_config->prescalar = 1;
    TIMERx_config->allignedmode = TIMER_COUNTER_EDGE;
    TIMERx_config->counterdirection = TIMER_COUNTER_UP;
    if (rcu_clock_freq_get(CK_APB1) == rcu_clock_freq_get(CK_AHB)){
        TIMERx_config->period = (rcu_clock_freq_get(CK_APB1)/TIMERx_config->prescalar)/1000000;
    }
    else{
        TIMERx_config->period = (rcu_clock_freq_get(CK_APB1)/TIMERx_config->prescalar)*2/1000000;
    }
    TIMERx_config->clockdivision = TIMER_CKDIV_DIV1;
    TIMERx_config->repetitioncounter = 0;
    
    timer_init(TIMER5, TIMERx_config);
    timer_auto_reload_shadow_enable(TIMER5);
    timer_update_source_config(TIMER5, TIMER_UPDATE_SRC_REGULAR);
    timer_update_event_enable(TIMER5);
    timer_enable(TIMER5);
    
    
    return;
}

/**
 * Release all resources initialized by sensirion_i2c_init().
 */
void sensirion_i2c_release(void) {
    i2c_deinit(I2C0);
    gpio_deinit(GPIOB);
    
    return;
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t *data, uint16_t count) {
    /* send a start condition to I2C bus */
    i2c_start_on_bus(i2c_periph);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(i2c_periph, SFM3019_ADDR, I2C_RECEIVER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND));

    /* clear ADDSEND flag and read data */
    if (N_reads == 1){
    	/* disable acking */
    	i2c_ack_config(i2c_periph, I2C_ACK_DISABLE);

    	/* clear ADDSEND flag */
    	i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    	/* wait for RXNE flag */
    	while(!i2c_flag_get(i2c_periph, I2C_FLAG_RXNE));

    	/* generate stop condition */
    	i2c_stop_on_bus(i2c_periph);

    	/* read data */
    	*(Rxbuffer) = i2c_data_receive(i2c_periph);
    }
    else if (N_reads > 1){
    	/* clear ADDSEND flag */
    	i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    	for (int i = N_reads; i > 0; i--){
    		/* wait for RXNE flag */
    		while(!i2c_flag_get(i2c_periph, I2C_FLAG_RXNE));

    		if (i == 2){
    			/* disable acking */
    			i2c_ack_config(i2c_periph, I2C_ACK_DISABLE);

    			/* generate stop condition */
    			i2c_stop_on_bus(i2c_periph);
    		}

    		/* read data */
    		*(Rxbuffer) = i2c_data_receive(i2c_periph);

    		Rxbuffer++;
    	}
    }

    /* enable acking */
    i2c_ack_config(i2c_periph, I2C_ACK_ENABLE);   
    
    return 0;
    return STATUS_FAIL;
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t *data,
                           uint16_t count) {
    /* send a start condition to I2C bus */
    i2c_start_on_bus(i2c_periph);

    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_SBSEND));

    /* send slave address to I2C bus */
    i2c_master_addressing(i2c_periph, SFM3019_ADDR, I2C_TRANSMITTER);

    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND));

    /* clear ADDSEND bit */
    i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    /* send data */
    for (int i = 0; i < N_bytes; i++){
    	/* wait until the transmit data buffer is empty */
		while(!i2c_flag_get(i2c_periph, I2C_FLAG_TBE));

		/* send command */
		i2c_data_transmit(i2c_periph, *buff);
        
        /* incremment buffer */
		buff++;

    }

    /* wait until all data is sent */
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_BTC));
    
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(i2c_periph);

    /* wait until stop condition generate */
    while(I2C_CTL0(i2c_periph)&0x0200);
        
    return 0;
    return STATUS_FAIL;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
    /* reset tick counter */
    tick_counter = 0;
    
    /* enable interrupts for the timer */
    timer_interrupt_enable(uint32_t timer_periph, TIMER_INT_UP);
    
    /* delay */
    while (tick_counter <= limit ){
    }
    
    /* disable interrupts for the timer */
    timer_interrupt_disable(uint32_t timer_periph, TIMER_INT_UP);
    
    return;
}

void TIMER5_IRQHandler(void){
    tick_counter++;
    
    return;
}
