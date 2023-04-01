#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();

	Serial_SendByte('A');

	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};
	Serial_SendArray(MyArray, 4);

	Serial_SendString("HelloWorld!\r\n");

	Serial_SendNumber(12345, 5);

	printf("Num1 = %d\r\n", 111);

	char String[100];
	sprintf(String, "Num2 = %d\r\n", 222);
	Serial_SendString(String);

	Serial_Printf("Num3 = %d\r\n", 333);

	while (1)
	{
	}
}
