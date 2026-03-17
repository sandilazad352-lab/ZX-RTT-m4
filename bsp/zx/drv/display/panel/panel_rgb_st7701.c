/*
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "panel_com.h"

static unsigned int __gpio_sck_g, __gpio_sck_p;
static unsigned int __gpio_cs_g, __gpio_cs_p;
static unsigned int __gpio_sda_g, __gpio_sda_p;

static int __st7701_write(unsigned int value, unsigned char u8Bits)
{
    unsigned int i;

    for(i = 0; i < u8Bits; i++)
    {
        hal_gpio_clr_output(__gpio_sck_g, __gpio_sck_p);
        aic_delay_us(5);
        if(value & (0x01<<(u8Bits - 1)))
            hal_gpio_set_output(__gpio_sda_g, __gpio_sda_p);
        else
            hal_gpio_clr_output(__gpio_sda_g, __gpio_sda_p);
        value <<= 1;
        aic_delay_us(5);
        hal_gpio_set_output(__gpio_sck_g, __gpio_sck_p);
        aic_delay_us(5);
    }

    hal_gpio_clr_output(__gpio_sda_g, __gpio_sda_p);
    aic_delay_us(5);
    hal_gpio_clr_output(__gpio_sck_g, __gpio_sck_p);
    aic_delay_us(5);
    return 0;
}

static void __spi_send_cmd(uint8_t c)
{
    hal_gpio_clr_output(__gpio_cs_g, __gpio_cs_p);
    aic_delay_us(50);
    __st7701_write(0, 1);
    __st7701_write(c, 8);
    aic_delay_us(50);
    hal_gpio_set_output(__gpio_cs_g, __gpio_cs_p);
    aic_delay_us(30);
}

static void __spi_send_data(uint8_t c)
{
    hal_gpio_clr_output(__gpio_cs_g, __gpio_cs_p);
    aic_delay_us(50);
    __st7701_write(1, 1);
    __st7701_write(c, 8);
    aic_delay_us(50);
    hal_gpio_set_output(__gpio_cs_g, __gpio_cs_p);
    aic_delay_us(30);
}

void __st7701_init(void)
{ 
    long pin;

    pin = hal_gpio_name2pin(PANEL_ST7701_CS_GPIO);
    __gpio_cs_g = GPIO_GROUP(pin);
    __gpio_cs_p = GPIO_GROUP_PIN(pin);

    hal_gpio_direction_output(__gpio_cs_g, __gpio_cs_p);
    hal_gpio_set_output(__gpio_cs_g, __gpio_cs_p);  // cs high

    pin = hal_gpio_name2pin(PANEL_ST7701_SDA_GPIO);
    __gpio_sda_g = GPIO_GROUP(pin);
    __gpio_sda_p = GPIO_GROUP_PIN(pin);

    hal_gpio_direction_output(__gpio_sda_g, __gpio_sda_p);
    hal_gpio_clr_output(__gpio_sda_g, __gpio_sda_p);    // sda low

    pin = hal_gpio_name2pin(PANEL_ST7701_SCK_GPIO);
    __gpio_sck_g = GPIO_GROUP(pin);
    __gpio_sck_p = GPIO_GROUP_PIN(pin);

    hal_gpio_direction_output(__gpio_sck_g, __gpio_sck_p);
    hal_gpio_set_output(__gpio_sck_g, __gpio_sck_g);  // sck high

    hal_gpio_set_output(__gpio_cs_g, __gpio_cs_p);
    hal_gpio_clr_output(__gpio_sda_g, __gpio_sda_p);
    hal_gpio_clr_output(__gpio_sck_g, __gpio_sck_p);

#if 0
    pin = hal_gpio_name2pin(PANEL_ST7701_RESET_GPIO);
    g = GPIO_GROUP(pin);
    p = GPIO_GROUP_PIN(pin);
    hal_gpio_direction_output(g, p);
    hal_gpio_set_output(g, p);
    aic_delay_ms(80);
    hal_gpio_clr_output(g, p);
    aic_delay_ms(80);
    hal_gpio_set_output(g, p);
    aic_delay_ms(120);
#endif

    __spi_send_cmd (0xF0);
    __spi_send_data (0x55);
    __spi_send_data (0xAA);
    __spi_send_data (0x52);
    __spi_send_data (0x08);
    __spi_send_data (0x00);

    __spi_send_cmd (0xF6);
    __spi_send_data (0x5A);
    __spi_send_data (0x87);

    __spi_send_cmd (0xC1);
    __spi_send_data (0x3F);

    __spi_send_cmd (0xC2);
    __spi_send_data (0x0E);

    __spi_send_cmd (0xC6);
    __spi_send_data (0xF8);

    __spi_send_cmd (0xC9);
    __spi_send_data (0x10);

    __spi_send_cmd (0xCD);
    __spi_send_data (0x25);

    __spi_send_cmd (0xF8);
    __spi_send_data (0x8A);

    __spi_send_cmd (0xAC);
    __spi_send_data (0x45);

    __spi_send_cmd (0xA0);
    __spi_send_data (0xDD);

    __spi_send_cmd (0xA7);
    __spi_send_data (0x47);

    __spi_send_cmd (0xFA);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);

    __spi_send_cmd (0x86);
    __spi_send_data (0x99);
    __spi_send_data (0xa3);
    __spi_send_data (0xa3);
    __spi_send_data (0x51);

    __spi_send_cmd (0xA3);
    __spi_send_data (0xEE);

    __spi_send_cmd (0xFD);
    __spi_send_data (0x3c);
    __spi_send_data (0x3c);
    __spi_send_data (0x00);

    __spi_send_cmd (0x71);
    __spi_send_data (0x48);

    __spi_send_cmd (0x72);
    __spi_send_data (0x48);

    __spi_send_cmd (0x73);
    __spi_send_data (0x00);
    __spi_send_data (0x44);

    __spi_send_cmd (0x97);
    __spi_send_data (0xEE);

    __spi_send_cmd (0x83);
    __spi_send_data (0x93);

    __spi_send_cmd (0x9A);
    __spi_send_data (0x72);

    __spi_send_cmd (0x9B);
    __spi_send_data (0x5a);

    __spi_send_cmd (0x82);
    __spi_send_data (0x2c);
    __spi_send_data (0x2c);

    __spi_send_cmd (0xB1);
    __spi_send_data (0x10);

    __spi_send_cmd (0x6D);
    __spi_send_data (0x00);
    __spi_send_data (0x1F);
    __spi_send_data (0x19);
    __spi_send_data (0x1A);
    __spi_send_data (0x10);
    __spi_send_data (0x0e);
    __spi_send_data (0x0c);
    __spi_send_data (0x0a);
    __spi_send_data (0x02);
    __spi_send_data (0x07);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x1E);
    __spi_send_data (0x08);
    __spi_send_data (0x01);
    __spi_send_data (0x09);
    __spi_send_data (0x0b);
    __spi_send_data (0x0D);
    __spi_send_data (0x0F);
    __spi_send_data (0x1a);
    __spi_send_data (0x19);
    __spi_send_data (0x1f);
    __spi_send_data (0x00);

    __spi_send_cmd (0x64);
    __spi_send_data (0x38);
    __spi_send_data (0x05);
    __spi_send_data (0x01);
    __spi_send_data (0xdb);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x38);
    __spi_send_data (0x04);
    __spi_send_data (0x01);
    __spi_send_data (0xdc);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x65);
    __spi_send_data (0x38);
    __spi_send_data (0x03);
    __spi_send_data (0x01);
    __spi_send_data (0xdd);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x38);
    __spi_send_data (0x02);
    __spi_send_data (0x01);
    __spi_send_data (0xde);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x66);
    __spi_send_data (0x38);
    __spi_send_data (0x01);
    __spi_send_data (0x01);
    __spi_send_data (0xdf);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x38);
    __spi_send_data (0x00);
    __spi_send_data (0x01);
    __spi_send_data (0xe0);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x67);
    __spi_send_data (0x30);
    __spi_send_data (0x01);
    __spi_send_data (0x01);
    __spi_send_data (0xe1);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x30);
    __spi_send_data (0x02);
    __spi_send_data (0x01);
    __spi_send_data (0xe2);
    __spi_send_data (0x03);
    __spi_send_data (0x03);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x68);
    __spi_send_data (0x00);
    __spi_send_data (0x08);
    __spi_send_data (0x15);
    __spi_send_data (0x08);
    __spi_send_data (0x15);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x08);
    __spi_send_data (0x15);
    __spi_send_data (0x08);
    __spi_send_data (0x15);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x60);
    __spi_send_data (0x38);
    __spi_send_data (0x08);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x38);
    __spi_send_data (0x09);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x63);
    __spi_send_data (0x31);
    __spi_send_data (0xe4);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);
    __spi_send_data (0x31);
    __spi_send_data (0xe5);
    __spi_send_data (0x7A);
    __spi_send_data (0x7A);

    __spi_send_cmd (0x69);
    __spi_send_data (0x04);
    __spi_send_data (0x22);
    __spi_send_data (0x14);
    __spi_send_data (0x22);
    __spi_send_data (0x14);
    __spi_send_data (0x22);
    __spi_send_data (0x08);

    __spi_send_cmd (0x6B);
    __spi_send_data (0x07);

    __spi_send_cmd (0x7A);
    __spi_send_data (0x08);
    __spi_send_data (0x13);

    __spi_send_cmd (0x7B);
    __spi_send_data (0x08);
    __spi_send_data (0x13);

    __spi_send_cmd (0xD1);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0xD2);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0xD3);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0xD4);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0xD5);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0xD6);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x00);
    __spi_send_data (0x04);
    __spi_send_data (0x00);
    __spi_send_data (0x12);
    __spi_send_data (0x00);
    __spi_send_data (0x18);
    __spi_send_data (0x00);
    __spi_send_data (0x21);
    __spi_send_data (0x00);
    __spi_send_data (0x2a);
    __spi_send_data (0x00);
    __spi_send_data (0x35);
    __spi_send_data (0x00);
    __spi_send_data (0x47);
    __spi_send_data (0x00);
    __spi_send_data (0x56);
    __spi_send_data (0x00);
    __spi_send_data (0x90);
    __spi_send_data (0x00);
    __spi_send_data (0xe5);
    __spi_send_data (0x01);
    __spi_send_data (0x68);
    __spi_send_data (0x01);
    __spi_send_data (0xd5);
    __spi_send_data (0x01);
    __spi_send_data (0xd7);
    __spi_send_data (0x02);
    __spi_send_data (0x36);
    __spi_send_data (0x02);
    __spi_send_data (0xa6);
    __spi_send_data (0x02);
    __spi_send_data (0xee);
    __spi_send_data (0x03);
    __spi_send_data (0x48);
    __spi_send_data (0x03);
    __spi_send_data (0xa0);
    __spi_send_data (0x03);
    __spi_send_data (0xba);
    __spi_send_data (0x03);
    __spi_send_data (0xc5);
    __spi_send_data (0x03);
    __spi_send_data (0xd0);
    __spi_send_data (0x03);
    __spi_send_data (0xE0);
    __spi_send_data (0x03);
    __spi_send_data (0xea);
    __spi_send_data (0x03);
    __spi_send_data (0xFa);
    __spi_send_data (0x03);
    __spi_send_data (0xFF);

    __spi_send_cmd (0x3a);
    __spi_send_data (0x66);

#if 0   //浅蓝 need pclk
    __spi_send_cmd(0xce);
    __spi_send_data(0x06);
    __spi_send_cmd(0xf2);
    __spi_send_data(0x70);
    __spi_send_data(0xFF);
    __spi_send_data(0x00);
    __spi_send_data(0x00);
#endif

    __spi_send_cmd (0x11);
    aic_delay_ms(120);
    __spi_send_cmd (0x29);
    aic_delay_ms(20);
}


static int panel_enable(struct aic_panel *panel)
{
    panel_di_enable(panel, 0);
    panel_de_timing_enable(panel, 0);
    __st7701_init();
    panel_backlight_enable(panel, 0);

    return 0;
}

static struct aic_panel_funcs panel_funcs = {
    .disable = panel_default_disable,
    .unprepare = panel_default_unprepare,
    .prepare = panel_default_prepare,
    .enable = panel_enable,
    .register_callback = panel_register_callback,
};

/* Init the videomode parameter, dts will override the initial value. */
static struct display_timing st7701_timing = {
    .pixelclock = 24000000,
    .hactive = 480,
    .hfront_porch = 60,
    .hback_porch = 80,
    .hsync_len = 10,
    .vactive = 480,
    .vfront_porch = 100,
    .vback_porch = 20,
    .vsync_len = 10,
};

static struct panel_rgb rgb = {
    .mode = AIC_RGB_MODE,
    .format = AIC_RGB_FORMAT,
    .clock_phase = AIC_RGB_CLK_CTL,
    .data_order = AIC_RGB_DATA_ORDER,
    .data_mirror = AIC_RGB_DATA_MIRROR,
};

struct aic_panel rgb_st7701 = {
    .name = "panel-st7701",
    .timings = &st7701_timing,
    .funcs = &panel_funcs,
    .rgb = &rgb,
    .connector_type = AIC_RGB_COM,
};
