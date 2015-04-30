 #include <stdint.h>
 #include <string.h>
 #include "an_packet_protocol.h"
 #include "spatial_packets.h"
 #include "interfaces.h"



void navigation(imu_nav_t *imu_nav_packet, uav_nav_t *uav_nav_packet, nav_cntr_t *nav_cntr_packet, nav_guid_t *nav_guid_packet)
{
//	printf("A acceleration: x %f, y %f, z %f\n", imu_nav_packet->acc_x, imu_nav_packet->acc_y, imu_nav_packet->acc_z);
//	printf("A gyr: x %f, y %f, z %f\n", imu_nav_packet->gyr_x, imu_nav_packet->gyr_y, imu_nav_packet->gyr_z);
//	printf("A navigation: vel_north %f, vel_east %f, vel_down %f\n", imu_nav_packet->vel_north, imu_nav_packet->vel_east, imu_nav_packet->vel_down);
//
//	printf("A uav_nav_t: position_Lat_init %f, position_Long_init %f, position_Alt_init %f\n", uav_nav_packet->position_Lat_init, uav_nav_packet->position_Long_init, uav_nav_packet->position_Alt_init);
//
//	//OUT
//	nav_cntr_packet->velocity_N=uav_nav_packet->position_Lat_init+31;
//	nav_guid_packet->velocity_N=uav_nav_packet->position_Lat_init+31;
//
//	printf("A nav_cntr_packet: %f\n",nav_cntr_packet->velocity_N);

	//printf("NAVIGATION MODULE\n");
	sqrt(50.0);
}


void control(imu_nav_t *imu_nav_packet, nav_cntr_t *nav_cntr_packet, guid_cntr_t *guid_cntr_packet, cntr_act_t *cntr_act_packet)
{
//	printf("B acceleration: x %f, y %f, z %f\n", imu_nav_packet->acc_x, imu_nav_packet->acc_y, imu_nav_packet->acc_z);
//	printf("B gyr: x %f, y %f, z %f\n", imu_nav_packet->gyr_x, imu_nav_packet->gyr_y, imu_nav_packet->gyr_z);
//	printf("B navigation: vel_north %f, vel_east %f, vel_down %f\n", imu_nav_packet->vel_north, imu_nav_packet->vel_east, imu_nav_packet->vel_down);
//
//	nav_cntr_packet->velocity_N=22+31;
//
//	printf("B nav_cntr_packet: %f\n",nav_cntr_packet->velocity_N);
	//printf("CONTROL MODULE\n");
	sqrt(50.0);
}

void guide(nav_guid_t *nav_guid_packet, guid_cntr_t *guid_cntr_packet, gimbal_guid_t *gimbal_guid_packet)
{
//	printf("GUIDE nav_guid_packet: pitch %f\n", nav_guid_packet->pitch);
//	printf("GUIDE guid_cntr_packet: x %f, y %f, z %f\n", guid_cntr_packet->acc_x, guid_cntr_packet->acc_y, guid_cntr_packet->acc_z);
//	printf("GUIDE gimbal_guid_packet: azimuth %f\n", gimbal_guid_packet->azimuth);
	//printf("GUIDE MODULE\n");
	sqrt(50.0);
}
