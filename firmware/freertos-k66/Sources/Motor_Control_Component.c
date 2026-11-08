#include "Motor_Control_Component.h"

QueueHandle_t motor_queue;
QueueHandle_t angle_queue;

void setupMotorComponent()
{
	setupMotorPins();

	setupDCMotor();
	setupServo();

    /*************** Motor Task ***************/
	//Create Motor Queue
	//Create Motor Task
	BaseType_t status;

	motor_queue = xQueueCreate(1, sizeof(int));
	if (motor_queue == NULL)
	{
		PRINTF("Motor queue creation failed!.\r\n");
		while (1) {;}
	}

	status = xTaskCreate(motorTask, "Motor_Task", 200, NULL, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1) {;}
	}

    /*************** Position Task ***************/
	//Create Angle Queue
	//Create Position Task
	angle_queue = xQueueCreate(1, sizeof(int));
	if (angle_queue == NULL)
	{
		PRINTF("Angle queue creation failed!.\r\n");
		while (1) {;}
	}

	status = xTaskCreate(positionTask, "Position_Task", 200, NULL, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Motor setup creation failed!.\r\n");
		while (1) {;}
	}
}

void setupMotorPins()
{
	//Configure PWM pins for DC and Servo motors
	CLOCK_EnableClock(kCLOCK_PortA);
	PORT_SetPinMux(PORTA, 6U, kPORT_MuxAlt3);

	CLOCK_EnableClock(kCLOCK_PortC);
	PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt4);
}

void setupDCMotor()
{
	//Initialize PWM for DC motor
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;

	ftmParam.chnlNumber            = FTM_CHANNEL_DC_MOTOR;
	ftmParam.level                 = pwmLevel;
	ftmParam.dutyCyclePercent      = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary   = false;
	ftmParam.enableDeadtime        = false;

	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;

	FTM_Init(FTM_MOTORS, &ftmInfo);
	FTM_SetupPwm(FTM_MOTORS,&ftmParam,1U,kFTM_EdgeAlignedPwm,50U,CLOCK_GetFreq(kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTORS, kFTM_SystemClock);
}

void setupServo()
{
	//Initialize PWM for Servo motor
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;

	ftmParam.chnlNumber            = FTM_CHANNEL_SERVO;
	ftmParam.level                 = pwmLevel;
	ftmParam.dutyCyclePercent      = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary   = false;
	ftmParam.enableDeadtime        = false;

	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;

	FTM_Init(FTM_MOTORS, &ftmInfo);
	FTM_SetupPwm(FTM_MOTORS,&ftmParam,1U,kFTM_EdgeAlignedPwm,50U,CLOCK_GetFreq(kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTORS, kFTM_SystemClock);
}

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle)
{
	uint32_t cnv, cnvFirstEdge = 0, mod;

	/* The CHANNEL_COUNT macro returns -1 if it cannot match the FTM instance */
	assert(-1 != FSL_FEATURE_FTM_CHANNEL_COUNTn(FTM_MOTORS));

	mod = FTM_MOTORS->MOD;
	if(dutyCycle == 0U)
	{
		/* Signal stays low */
		cnv = 0;
	}
	else
	{
		cnv = mod * dutyCycle;
		/* For 100% duty cycle */
		if (cnv >= mod)
		{
			cnv = mod + 1U;
		}
	}

	FTM_MOTORS->CONTROLS[channel].CnV = cnv;
}

void motorTask(void* pvParameters)
{
	//Motor task implementation
	(void)pvParameters;

	BaseType_t status;
	int input_motor = 0;
	float dutyCycle;

	while (1)
	{
		status = xQueueReceive(motor_queue, (void *)&input_motor, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Queue Receive failed!.\r\n");
			while (1) {;}
		}

		dutyCycle = (float)input_motor * 0.025f / 100.0f + 0.0615f;
		updatePWM_dutyCycle(FTM_CHANNEL_DC_MOTOR, dutyCycle);
		FTM_SetSoftwareTrigger(FTM_MOTORS, true);
	}
}

void positionTask(void* pvParameters)
{
	//Position task implementation
	(void)pvParameters;

	BaseType_t status;
	int input_servo = 0;
	float dutyCycle;

	while (1)
	{
		status = xQueueReceive(angle_queue, (void *)&input_servo, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Queue Receive failed!.\r\n");
			while (1) {;}
		}
		dutyCycle = (float)input_servo * 0.025f / 100.0f + 0.075f;
		updatePWM_dutyCycle(FTM_CHANNEL_SERVO, dutyCycle);
		FTM_SetSoftwareTrigger(FTM_MOTORS, true);
	}
}
