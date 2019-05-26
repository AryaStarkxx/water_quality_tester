#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "ds18b20.h""




char TEMPARRAY[10];
char TURBARRAY[10];
char PhARRAY[10];
double d = 0;




unsigned long rawPh = 0;
unsigned long Ph_val = 0;
	
unsigned long turbValue = 0;
unsigned long rawTurb = 0;

uint16_t readADC(uint8_t channel) 
{
  ADMUX = (0xf0 & ADMUX) | channel;
  ADCSRA |= (1 << ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return (ADC);
}




int main (void)
{
	lcd_init(); //Initialize LCD Port
	lcd_clear();//Clear LCD Contents
	
															//Initialize ADC
	ADMUX |= (1 << REFS0);                   	//reference voltage on AVCC 
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);      //ADC clock prescaler /8 
	ADCSRA |= (1 << ADEN);  
	
	while(1)
	{
		
		lcd_set_cursor(0,0);
		lcd_puts("Water Quality Tester");
		lcd_set_cursor(0,1);
		lcd_puts("Temp: ");
		d = ds18b20_gettemp();
		_delay_ms(100);
		dtostrf(d, 3, 2, TEMPARRAY);
		lcd_set_cursor(15,1);
		lcd_puts(TEMPARRAY);
		_delay_ms(1000);
		
		
		
		
		lcd_set_cursor(0,2);
		lcd_puts("Turbidity:");
		for(int x=0; x<1000; x++){
			rawTurb += readADC(1);
		}
		rawTurb /= 1000;
		
		double turbidity = (rawTurb/1023.0)*10000;
		
		if(turbidity <=303.050)
		{
			lcd_set_cursor(15,2);
			lcd_puts("clean");
							
		}
		else{
		
			lcd_set_cursor(15,2);
			lcd_puts("dirty");	
		
		}
	
		for(int y=0; y<10; y++)
		{
			rawPh += readADC(0);
		}
		rawPh /= 10;
		
		lcd_set_cursor(0,3);
		lcd_puts("PH level:");
	
		double Ph_val = -5.72
		*((rawPh/1023.0)*5.0)+24.40;
		dtostrf(Ph_val,1,0,PhARRAY);
		lcd_set_cursor(19,3);
		lcd_puts(PhARRAY);

		
		
	}
	return 0 ;
}

