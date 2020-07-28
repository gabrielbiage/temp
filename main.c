#include "sfm3019.h"
#include <stdint.h>

int16_t error;

void rcu_config(void){
	/* select clock source */
	rcu_system_clock_source_config(RCU_CKSYSSRC_HXTAL); //Set external crystal oscilator as clock
	/* set AHB clock prescalar */
    rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV2)            //AHB = (SYS_CLK/2)
    /* set APB1 clock prescalar */
    rcu__apb1_clock_config(RCU_APB1_CKAHB_DIV2);        //APB1 = (AHB_CLK/2)
	/* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable I2C0 clock */
    rcu_periph_clock_enable(RCU_I2C0);

    return;
}

void gpio_config(void){
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
}

/* blink led every measurement */
void blink(void){
	gpio_bit_write(GPIOB, GPIO_PIN_5, SET);
	/* delay */
	gpio_bit_write(GPIOB, GPIO_PIN_5, RESET);

	return;
}

void led_on(void){
	return gpio_bit_write(GPIOB, GPIO_PIN_5, SET);
}

int main(){
	/* Clock config */
	rcu_config;
	
	/* Gpio config */
	gpio_config;

	/* Initialize I2C bus */
    sensirion_i2c_init();

    /* Reset all I2C devices */
    error = sensirion_i2c_general_call_reset();
    if (error) {
        /* turn led on */
    	led_on();
        while(1);
    }

    /* Wait for the SFM3019 to initialize */
    sensirion_sleep_usec(SFM3019_SOFT_RESET_TIME_US);

    /* wait until sensor responds */
    while (sfm3019_probe());

    SfmConfig sfm3019 = sfm3019_create();

    /* initialize continuous measurement */
    error = sfm_common_start_continuous_measurement(&sfm3019, SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR);
    if (error) {
        /* turn led on */
    	led_on();
        while(1);
    }

    /* Wait for the first measurement to be available. Wait for
     * SFM3019_MEASUREMENT_WARM_UP_TIME_US instead for more reliable results */
    sensirion_sleep_usec(SFM3019_MEASUREMENT_INITIALIZATION_TIME_US);

    /* continously read from sensor */
    for (;;) {
        int16_t flow_raw;
        int16_t temperature_raw;
        uint16_t status;
        error = sfm_common_read_measurement_raw(&sfm3019, &flow_raw, &temperature_raw, &status);
        if (error) {
            /* do nothing */
            ;
        } else {
        	blink();
            float flow;
            float temperature;
            error = sfm_common_convert_flow_float(&sfm3019, flow_raw, &flow);
            if (error) {
                /* do nothing */
                ;
            }
            temperature = sfm_common_convert_temperature_float(temperature_raw);
        }
    }

    sensirion_i2c_release();
    return 0;

	return 0;
}