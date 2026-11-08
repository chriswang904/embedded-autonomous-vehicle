#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <uORB/topics/sensor_combined.h>
#include <uORB/topics/rc_channels.h>
extern "C" __EXPORT int hello_world_main(int argc, char *argv[]);
/*
int hello_world_main(int argc, char *argv[])
{
	int sensor_combined_handle;
	sensor_combined_s sensor_data;
	sensor_combined_handle = orb_subscribe(ORB_ID(sensor_combined));
	orb_set_interval(sensor_combined_handle, 200);
	while(1)
	{
		orb_copy(ORB_ID(sensor_combined), sensor_combined_handle, &sensor_data);
		PX4_INFO("X = %f, Y = %f, Z = %f", (double)sensor_data.accelerometer_m_s2[0],
					            (double)sensor_data.accelerometer_m_s2[1],
					            (double)sensor_data.accelerometer_m_s2[2]);
		px4_usleep(200000);
	}
	return 0;
}
*/

int hello_world_main(int argc, char *argv[])
{
    double rc_channel_handle;
    rc_channels_s rc_channel_data;
    rc_channel_handle = orb_subscribe(ORB_ID(rc_channels));
    orb_set_interval(rc_channel_handle, 200);

    while (1)
    {
        orb_copy(ORB_ID(rc_channels), rc_channel_handle, &rc_channel_data);

        PX4_INFO("header = %f, ch1 = %f, ch2 = %f, ch3 = %f, ch4 = %f",
                (double)rc_channel_data.channels[0],
                (double)rc_channel_data.channels[1],
                (double)rc_channel_data.channels[2],
                (double)rc_channel_data.channels[3],
                (double)rc_channel_data.channels[4]);
                
        PX4_INFO("header = %f, ch5 = %f, ch6 = %f, ch7 = %f, ch8 = %f",
        	 (double)rc_channel_data.channels[0],               
                (double)rc_channel_data.channels[5],
                (double)rc_channel_data.channels[6],
                (double)rc_channel_data.channels[7],
                (double)rc_channel_data.channels[8]);

        px4_usleep(200000);
    }

    return 0;
}
