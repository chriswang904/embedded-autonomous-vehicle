#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>

#include <drivers/drv_hrt.h>
#include <uORB/Publication.hpp>
#include <uORB/topics/test_motor.h>
#include <uORB/topics/rc_channels.h>


#define DC_MOTOR    0
#define SERVO_MOTOR 1

extern "C" __EXPORT int hello_world_main(int argc, char *argv[]);
int hello_world_main(int argc, char *argv[])
{
	test_motor_s test_motor;
	test_motor_s test_servo;
	double motor_value = 0; // a number between 0 to 1
	double servo_value = 0;
	double rc_channel_handle;
	rc_channels_s rc_channel_data;
	rc_channel_handle = orb_subscribe(ORB_ID(rc_channels));
	orb_set_interval(rc_channel_handle, 200);
	
	uORB::Publication<test_motor_s> test_motor_pub(ORB_ID(test_motor));

	while(1)
	{
		orb_copy(ORB_ID(rc_channels), rc_channel_handle, &rc_channel_data);
	        motor_value = ((double)rc_channel_data.channels[2] + 1.0) / 2.0;
		if (motor_value > 1.0  or motor_value < 0)
            		break;

	        servo_value = ((double)rc_channel_data.channels[0] + 1.0) / 2.0;
        	if (servo_value > 1.0  or servo_value < 0)
            		break;

        	PX4_INFO("Motor speed is %f", motor_value);
		test_motor.timestamp = hrt_absolute_time();
        	test_motor.motor_number = DC_MOTOR;
        	test_motor.value = (float)motor_value;
        	test_motor.action = test_motor_s::ACTION_RUN;
        	test_motor.driver_instance = 0;
        	test_motor.timeout_ms = 0;
        	test_motor_pub.publish(test_motor);

        	PX4_INFO("Servo angle is %f", servo_value);
        	test_servo.timestamp = hrt_absolute_time();
        	test_servo.motor_number = SERVO_MOTOR;
        	test_servo.value = (float)servo_value;
        	test_servo.action = test_motor_s::ACTION_RUN;
        	test_servo.driver_instance = 0;
        	test_servo.timeout_ms = 0;
        	test_motor_pub.publish(test_servo);
		
	}
	PX4_INFO("The motor will be stopped");
	test_motor.timestamp = hrt_absolute_time();
	test_motor.motor_number = DC_MOTOR;
	test_motor.value = 0.5;
	test_motor.driver_instance = 0;
	test_motor.timeout_ms = 0;
	test_motor_pub.publish(test_motor);
	
	PX4_INFO("The motor will be stopped");
	test_servo.timestamp = hrt_absolute_time();
	test_servo.motor_number = SERVO_MOTOR;
	test_servo.value = 0.5;
	test_servo.driver_instance = 0;
	test_servo.timeout_ms = 0;
	test_motor_pub.publish(test_servo);
	return 0;
}
