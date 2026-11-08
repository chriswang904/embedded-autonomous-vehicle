#include "RC_Receiver_Component.h"

SemaphoreHandle_t rc_hold_semaphore;
TaskHandle_t rc_task_handle;

typedef struct
{
    uint16_t header;
    uint16_t ch1;
    uint16_t ch2;
    uint16_t ch3;
    uint16_t ch4;
    uint16_t ch5;
    uint16_t ch6;
    uint16_t ch7;
    uint16_t ch8;
} RC_Values;


void setupRCReceiverComponent()
{
	setupRCPins();

	setupUART_RC();

    /*************** RC Task ***************/
	//Create RC Semaphore
	rc_hold_semaphore = xSemaphoreCreateBinary();
	if (rc_hold_semaphore == NULL)
	{
		PRINTF("RC Semaphore creation failed!.\r\n");
		while (1)
			;
	}

	//Create RC Task
	BaseType_t status;
	status = xTaskCreate(rcTask, "RC Task", 200, NULL, 2, &rc_task_handle);
	if (status != pdPASS)
	{
		PRINTF("RC Task creation failed!.\r\n");
		while (1)
			;
	}
}

void setupRCPins()
{
	//Configure RC pins
	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);
}

void setupUART_RC()
{
	//setup UART for RC receiver
	uart_config_t config;

	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx = false;
	config.enableRx = true;

	UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

void rcTask(void* pvParameters)
{
	//RC task implementation
	(void)pvParameters;

	BaseType_t status;

	RC_Values rc_values;
	uint8_t *ptr = (uint8_t *)&rc_values;

	int speed = 0;
	int angle = 0;

	uint8_t mode = 0;
	uint8_t modePress = 0;

	int direction = 1;
	uint8_t switch_direction = 0;

	// Initial queue values
	xQueueSendToBack(led_queue, &mode, portMAX_DELAY);

	while (1)
	{
		UART_ReadBlocking(UART1, ptr, 1);

		if (*ptr != 0x20)
			continue;

		UART_ReadBlocking(UART1, &ptr[1], sizeof(rc_values) - 1);

		if (rc_values.header == 0x4020)
		{

			if (rc_values.ch5 == 2000)
			{
				if (!switch_direction)
				{
					switch_direction = 1;

					if (direction == -1)
					{
						direction = 1;
					}
					else
					{
						direction = -1;
					}
				}
			}
			else
			{
				switch_direction = 0;
			}

			speed = direction * (rc_values.ch3 - 1000) / 10;

			if (mode == 1)
			{
				speed *= 0.5;
			}
			else if (mode == 2)
			{
				speed *= 0.1;
			}

			status = xQueueSendToBack(motor_queue, &speed, portMAX_DELAY);
			if (status != pdPASS)
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1)
					;
			}

			// Servo Logic

			angle = -1 * (rc_values.ch1 - 1500) / 5;

			status = xQueueSendToBack(angle_queue, &angle, portMAX_DELAY);
			if (status != pdPASS)
			{
				PRINTF("Queue Send failed!.\r\n");
				while (1)
					;
			}

			// LED Logic

			if (rc_values.ch6 == 2000)
			{
				if (!modePress)
				{
					modePress = 1;

					if (mode == 2)
					{
						mode = 0;
					}
					else if (mode == 1)
					{
						mode = 2;
					}
					else
					{
						mode = 1;
					}

					status = xQueueSendToBack(led_queue, &mode, portMAX_DELAY);
					if (status != pdPASS)
					{
						PRINTF("Queue Send failed!.\r\n");
						while (1)
							;
					}
				}
			}
			else
			{
				modePress = 0;
			}
		}
	}
}
