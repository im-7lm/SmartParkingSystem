#include "MCAL/DIO/DIO_interface.h"
#include "HAL/LED/LED_interface.h"
#include "HAL/SEVSEG/SSD_interface.h"
#include "HAL/IR/IR_interface.h"
#include "HAL/DcMotor/DcMotor_Interface.h"
#include "avr/delay.h"

int main(void)
{
	// IR Initialize
	IR_Init(DIO_u8PORTD, DIO_u8PIN0);
	IR_Init(DIO_u8PORTD, DIO_u8PIN1);

	// Motor initialize
	DCMOTOR_Init(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);

	// SevenSegment initialize
	SEVSEG_Init(DIO_u8PORTC);

	// Set LED as an output
	DIO_u8SetPinDirection(DIO_u8PORTD, DIO_u8PIN4, DIO_u8PIN_OUTPUT);

	u8 counter = 0;
	u8 carDetected = 0;

	u8 enterIrValue;
	u8 exitIrValue;

	while (1)
	{
		IR_GetState(DIO_u8PORTD, DIO_u8PIN0, &enterIrValue);
		IR_GetState(DIO_u8PORTD, DIO_u8PIN1, &exitIrValue);

		if (counter < 3)
		{
			LED_u8TurnOff(DIO_u8PORTD, DIO_u8PIN4);

			// Entry
			if (enterIrValue == IR_DETECTED && carDetected == 0)
			{
				carDetected = 1;

				DCMOTOR_RotateDirection1(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
				_delay_ms(1000);
				DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);

				for (u8 i = 0; i < 50; i++)
				{
					IR_GetState(DIO_u8PORTD, DIO_u8PIN1, &exitIrValue);

					if (exitIrValue == IR_DETECTED)
					{
						counter++;
						break;
					}

					_delay_ms(100);
				}

				SEVSEG_DisplayNumber(DIO_u8PORTC, counter, SEVSEG_u8COMMON_CATHODE, DIO_u8PIN0, DIO_u8PORTA);

				_delay_ms(2000);

				DCMOTOR_RotateDirection2(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
				_delay_ms(1000);
				DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			}

			// Exit
			else if (exitIrValue == IR_DETECTED && counter > 0 && carDetected == 0)
			{
				carDetected = 1;

				DCMOTOR_RotateDirection1(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
				_delay_ms(1000);
				DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);

				for (u8 i = 0; i < 50; i++)
				{
					IR_GetState(DIO_u8PORTD, DIO_u8PIN0, &enterIrValue);

					if (enterIrValue == IR_DETECTED)
					{
						counter--;
						break;
					}

					_delay_ms(100);
				}

				SEVSEG_DisplayNumber(DIO_u8PORTC, counter, SEVSEG_u8COMMON_CATHODE, DIO_u8PIN0, DIO_u8PORTA);

				_delay_ms(2000);

				DCMOTOR_RotateDirection2(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
				_delay_ms(1000);
				DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			}
		}
		else
		{
			LED_u8TurnOn(DIO_u8PORTD, DIO_u8PIN4);
		}

		IR_GetState(DIO_u8PORTD, DIO_u8PIN0, &enterIrValue);
		IR_GetState(DIO_u8PORTD, DIO_u8PIN1, &exitIrValue);

		if (enterIrValue == IR_NOT_DETECTED && exitIrValue == IR_NOT_DETECTED)
		{
			carDetected = 0;
		}
	}
}
