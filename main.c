#define F_CPU 8000000
#define MOTOR_DELAY 120

#include "MCAL/DIO/DIO_interface.h"
#include "HAL/LED/LED_interface.h"
#include "HAL/SEVSEG/SSD_interface.h"
#include "HAL/IR/IR_interface.h"
#include "HAL/DcMotor/DcMotor_Interface.h"
#include "avr/delay.h"

int main(void)
{
	// IR Initialize
	IR_Init(DIO_u8PORTD, DIO_u8PIN0); // outside
	IR_Init(DIO_u8PORTD, DIO_u8PIN1); // inside

	// Motor initalize
	DCMOTOR_Init(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);

	// SevenSegment initialize
	SEVSEG_Init(DIO_u8PORTC);

	// set led as output
	DIO_u8SetPinDirection(DIO_u8PORTD, DIO_u8PIN6, DIO_u8PIN_OUTPUT);

	u8 counter = 0;
	u8 carDetected = 0;

	u8 enterIrValue;
	u8 exitIrValue;

	SEVSEG_DisplayNumber(DIO_u8PORTC, counter, SEVSEG_u8COMMON_CATHODE, DIO_u8PIN0, DIO_u8PORTA);

	while (1)
	{
		IR_GetState(DIO_u8PORTD, DIO_u8PIN1, &enterIrValue);
		IR_GetState(DIO_u8PORTD, DIO_u8PIN0, &exitIrValue);

		if (counter >= 3)
		{
			LED_u8TurnOn(DIO_u8PORTD, DIO_u8PIN6);
		}
		else
		{
			LED_u8TurnOff(DIO_u8PORTD, DIO_u8PIN6);
		}

		// entry
		if (enterIrValue == IR_DETECTED && counter < 3 && carDetected == 0)
		{
			carDetected = 1;

			// open gate
			DCMOTOR_RotateDirection1(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(MOTOR_DELAY);
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

			// close
			_delay_ms(2000);
			DCMOTOR_RotateDirection2(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(MOTOR_DELAY);
			DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
		}

		// exit
		else if (exitIrValue == IR_DETECTED && counter > 0 && carDetected == 0)
		{
			carDetected = 1;

			// open
			DCMOTOR_RotateDirection1(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(MOTOR_DELAY);
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

			// close
			_delay_ms(2000);
			DCMOTOR_RotateDirection2(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
			_delay_ms(MOTOR_DELAY);
			DCMOTOR_Stop(DIO_u8PORTD, DIO_u8PIN2, DIO_u8PIN3);
		}

		IR_GetState(DIO_u8PORTD, DIO_u8PIN0, &enterIrValue);
		IR_GetState(DIO_u8PORTD, DIO_u8PIN1, &exitIrValue);

		if (enterIrValue == IR_NOT_DETECTED && exitIrValue == IR_NOT_DETECTED)
		{
			carDetected = 0;
		}
	}
}
