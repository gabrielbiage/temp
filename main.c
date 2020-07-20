#include "gd32vf103_startup.c"
#include "system_gd32vf103.h"
#include "gd32vf103_rcu.h"
#include "gd32vf103_eclic.h"
#include "gd32103_gpio.h"
#include "gd32103_i2c.h"
#include "sfm3019.h"

#include <stdio.h>
#include <stdint.h>

void clk_config(void){
    rcu_system_clock_source_config(RCU_CKSYSSRC_HXTAL); //Set external crystal oscilator as clock
    rcu_ahb_clock_config(RCU_AHB-CKSYS_DIV2);           //AHB clock prescalar  (SYS_CLK/2)
    rcu__apb1_clock_config(RCU_APB1_CKAHB_DIV2);        //APB1 clock prescalar (AHB_CLK/2)

    rcu_periph_clock_enable(RCU_AF);             //alternate function clock enable
    rcu_periph_clock_enable(RCU_I2C0);           //i2c0 clock enable
    rcu_periph_clock_enable(RCU_GPIOB);          //GPIOB clock enable
    
    return;
}

void gpio_config(void){    
    gpio_pin_remap_config(GPIO_I2C0_REMAP, DISABLE);                      //remap i2c0 to pins 6 and 7 in PORT B
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_ 6);    //configure alternate function open drain for pin6
    gpio_init(GPIOB, GPIO_MODE_AF_OD, GPIO_OSPEED_10MHZ, GPIO_PIN_ 7);    //configure alternate function open drain for pin7
    
    return;
}

void i2c_config(void){
    slave_addr_7BIT = 0x82U;
    
    i2c_clock_config(I2C0, 100000U, I2C_DTCY_2); //configure clock (standart mode)
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7bits, slave_addr_7BIT);
    
    i2c_enable(I2C0);                           //enable i2c
    
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);       //enable ack

    i2c_stretch_scl_low_config(I2C0, I2C_SCLSTRETCH_ENABLE) //Enable clock stretching
    
    return;
};

void nvic_config(void){
    i2c_interrupt_enable(I2C0, I2C_INT_EV);      //enable event interruts
    i2c_interrupt_enable(I2C0, I2C_INT_BUF);     //enable Tx and Rx interrupts

    eclic_global_interrupt_enable();            //dont know what it does;
    // nvic_irq_enable();                          //nvic interrupt enable
     
    return;
};

uint16_t data;
uint8_t crc_value;

uint8_t count = 0;

int main(){
    clk_config();
    gpio_config();
    i2c_config();
    
    //send continuous measurement command to the sensor
    sfm3019_start_continuous_measurement(uint32_t I2C0, uint16_t SFM3019_COMMAND_GAS_O2);

    //enable interrupts
    nvic_config();

    //set start bit
    i2c_start_on_bus(I2C0);
    while(1);

    return 0;
}


void I2C0_EV_IRQHandler(void){

    if (i2c_flag_get(I2C0, I2C_FLAG_SBSEND)){
       i2c_master_addressing(I2C0, SFM3019_ADDR, I2C_RECEIVER);
    }

    if (i2c_flag_get(I2C0, I2C_FLAG_ADDSEND)){
        i2c_flag_clear(I2C0, I2C_FLAG_ADDSEND);
    }

    if ( i2c_flag_get(I2C0, I2C_FLAG_RBNE) && i2c_flag_get(I2C0, I2C_FLAG_BTC)){
        //Read data register twice because both the register and the    
        for (int i = 0; i < 2; i++){
            if (count == 0){
                data = (i2c_data_receive(I2C0) << 8);
            }
            else if (count == 1){
                data = ( data | i2c_data_receive(I2C0) );
            }
            else if (count == 2){
                crc_value = i2c_data_receive(I2C0);
            }

            count = (count + 1) % 3;
        }

        uint8_t temp;

        temp = I2C_STAT0(I2C0); //"BTC flag cleared by reading I2C_STAT0 followed by reading or writing."  (?????????? não entendi)
        temp = I2C_STAT0(I2C0);
    }

    else if (i2c_flag_get(I2C0, I2C_FLAG_RBNE)){
        //clear RBNE FLAG by reading data register
        if (count == 0){
            data = (i2c_data_receive(I2C0) << 8);
        }
        else if (count == 1){
            data = ( data | i2c_data_receive(I2C0) );
        }
        else if (count == 2){
            CRC = i2c_data_receive(I2C0);
        }

        count = (count + 1) % 3;
    }

    return;

}
