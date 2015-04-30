 #include <stdint.h>
 #include <string.h>
 #include "an_packet_protocol.h"
 #include "spatial_packets.h"

/*
 * This file contains functions to decode and encode packets
 *
 * Decode functions take an an_packet_t and turn it into a type specific
 * to that packet so that the fields can be conveniently accessed. Decode
 * functions return 0 for success and 1 for failure. Decode functions are
 * used when receiving packets.
 *
 * Example decode
 *
 * an_packet_t an_packet
 * acknowledge_packet_t acknowledge_packet
 * ...
 * decode_acknowledge_packet(&acknowledge_packet, &an_packet);
 * printf("acknowledge id %d with result %d\n", acknowledge_packet.packet_id, acknowledge_packet.acknowledge_result);
 *
 * Encode functions take a type specific structure and turn it into an
 * an_packet_t. Encode functions are used when sending packets. Don't
 * forget to free the returned packet with an_packet_free().
 *
 * Example encode
 *
 * an_packet_t *an_packet;
 * boot_mode_packet_t boot_mode_packet;
 * ...
 * boot_mode_packet.boot_mode = boot_mode_bootloader;
 * an_packet = encode_boot_mode_packet(&boot_mode_packet);
 * serial_port_transmit(an_packet_pointer(an_packet), an_packet_size(an_packet));
 * an_packet_free(&an_packet);
 *
 */

an_packet_t *encode_request_packet(uint8_t requested_packet_id)
{
	an_packet_t *an_packet = an_packet_allocate(1, packet_id_request);
	an_packet->data[0] = requested_packet_id;
	return an_packet;
}


int decode_running_time_packet(running_time_packet_t *running_time_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_running_time && an_packet->length == 8)
	{
		memcpy(&running_time_packet->seconds, &an_packet->data[0], sizeof(uint32_t));
		memcpy(&running_time_packet->microseconds, &an_packet->data[4], sizeof(uint32_t));
		return 0;
	}
	else return 1;
}

int decode_system_state_packet(system_state_packet_t *system_state_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_system_state && an_packet->length == 100)
	{
		memcpy(&system_state_packet->system_status, &an_packet->data[0], sizeof(uint16_t));
		memcpy(&system_state_packet->filter_status, &an_packet->data[2], sizeof(uint16_t));
		memcpy(&system_state_packet->unix_time_seconds, &an_packet->data[4], sizeof(uint32_t));
		memcpy(&system_state_packet->microseconds, &an_packet->data[8], sizeof(uint32_t));
		memcpy(&system_state_packet->latitude, &an_packet->data[12], sizeof(double));
		memcpy(&system_state_packet->longitude, &an_packet->data[20], sizeof(double));
		memcpy(&system_state_packet->height, &an_packet->data[28], sizeof(double));
		memcpy(&system_state_packet->velocity[0], &an_packet->data[36], 3 * sizeof(float));
		memcpy(&system_state_packet->body_acceleration[0], &an_packet->data[48], 3 * sizeof(float));
		memcpy(&system_state_packet->g_force, &an_packet->data[60], sizeof(float));
		memcpy(&system_state_packet->orientation[0], &an_packet->data[64], 3 * sizeof(float));
		memcpy(&system_state_packet->angular_velocity[0], &an_packet->data[76], 3 * sizeof(float));
		memcpy(&system_state_packet->standard_deviation[0], &an_packet->data[88], 3 * sizeof(float));
		return 0;
	}
	else return 1;
}

int decode_raw_sensors_packet(raw_sensors_packet_t *raw_sensors_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_raw_sensors && an_packet->length == 48)
	{
		memcpy(&raw_sensors_packet->accelerometers[0], &an_packet->data[0], 3 * sizeof(float));
		memcpy(&raw_sensors_packet->gyroscopes[0], &an_packet->data[12], 3 * sizeof(float));
		memcpy(&raw_sensors_packet->magnetometers[0], &an_packet->data[24], 3 * sizeof(float));
		memcpy(&raw_sensors_packet->imu_temperature, &an_packet->data[36], sizeof(float));
		memcpy(&raw_sensors_packet->pressure, &an_packet->data[40], sizeof(float));
		memcpy(&raw_sensors_packet->pressure_temperature, &an_packet->data[44], sizeof(float));
		return 0;
	}
	else return 1;
}

int decode_device_information_packet(device_information_packet_t *device_information_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_device_information && an_packet->length == 24)
	{
		memcpy(&device_information_packet->software_version, &an_packet->data[0], sizeof(uint32_t));
		memcpy(&device_information_packet->device_id, &an_packet->data[4], sizeof(uint32_t));
		memcpy(&device_information_packet->hardware_revision, &an_packet->data[8], sizeof(uint32_t));
		memcpy(&device_information_packet->serial_number[0], &an_packet->data[12], 3*sizeof(uint32_t));
		return 0;
	}
	else return 1;
}

int decode_raw_gnss_packet(raw_gnss_packet_t *raw_gnss_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_raw_gnss && an_packet->length == 74)
	{
		memcpy(&raw_gnss_packet->unix_time_seconds, &an_packet->data[0], sizeof(uint32_t));
		memcpy(&raw_gnss_packet->microseconds, &an_packet->data[4], sizeof(uint32_t));
		memcpy(&raw_gnss_packet->position[0], &an_packet->data[8], 3*sizeof(double));
		memcpy(&raw_gnss_packet->velocity[0], &an_packet->data[32], 3*sizeof(float));
		memcpy(&raw_gnss_packet->position_standard_deviation[0], &an_packet->data[44], 3*sizeof(float));
		memcpy(&raw_gnss_packet->tilt, &an_packet->data[56], sizeof(float));
		memcpy(&raw_gnss_packet->heading, &an_packet->data[60], sizeof(float));
		memcpy(&raw_gnss_packet->tilt_standard_deviation, &an_packet->data[64], sizeof(float));
		memcpy(&raw_gnss_packet->heading_standard_deviation, &an_packet->data[68], sizeof(float));
		memcpy(&raw_gnss_packet->flags.r, &an_packet->data[72], sizeof(uint16_t));
		return 0;
	}
	else return 1;
}

/* PACKET CONFIGURATION FUNCTIONS */

int decode_packet_timer_period_packet(packet_timer_period_packet_t *packet_timer_period_packet, an_packet_t *an_packet)
{
    if(an_packet->id == packet_id_packet_timer_period && an_packet->length == 4)
    {
        packet_timer_period_packet->permanent = an_packet->data[0];
        packet_timer_period_packet->utc_synchronisation = an_packet->data[1];
        memcpy(&packet_timer_period_packet->packet_timer_period, &an_packet->data[2], sizeof(uint16_t));
        return 0;
    }
    else return 1;
}

an_packet_t *encode_packet_timer_period_packet(packet_timer_period_packet_t *packet_timer_period_packet)
{
	an_packet_t *an_packet = an_packet_allocate(4, packet_id_packet_timer_period);
	if(an_packet != NULL)
	{
		an_packet->data[0] = packet_timer_period_packet->permanent > 0;
		an_packet->data[1] = packet_timer_period_packet->utc_synchronisation > 0;
		memcpy(&an_packet->data[2], &packet_timer_period_packet->packet_timer_period, sizeof(uint16_t));
	}
    return an_packet;
}

int decode_packet_periods_packet(packet_periods_packet_t *packet_periods_packet, an_packet_t *an_packet)
{
    if(an_packet->id == packet_id_packet_periods && (an_packet->length - 2) % 5 == 0)
    {
        int i;
        int packet_periods_count = (an_packet->length - 2) / 5;
        packet_periods_packet->permanent = an_packet->data[0];
        packet_periods_packet->clear_existing_packets = an_packet->data[1];
        for(i = 0; i < MAXIMUM_PACKET_PERIODS; i++)
        {
            if(i < packet_periods_count)
            {
                packet_periods_packet->packet_periods[i].packet_id = an_packet->data[2 + 5*i];
                memcpy(&packet_periods_packet->packet_periods[i].period, &an_packet->data[2 + 5*i + 1], sizeof(uint32_t));
            }
            else memset(&packet_periods_packet->packet_periods[i], 0, sizeof(packet_period_t));
        }
        return 0;
    }
    else return 1;
}

an_packet_t *encode_packet_periods_packet(packet_periods_packet_t *packet_periods_packet, int num_ids)
{

	int i;

    an_packet_t *an_packet = an_packet_allocate(252, packet_id_packet_periods);
	if(an_packet != NULL)
	{
		an_packet->data[0] = packet_periods_packet->permanent > 0;
		an_packet->data[1] = packet_periods_packet->clear_existing_packets;

//		for(i = 0; i < MAXIMUM_PACKET_PERIODS; i++)
		for(i = 0; i < num_ids; i++)
		{
			if(packet_periods_packet->packet_periods[i].packet_id)
			{

				an_packet->data[2 + 5*i] = packet_periods_packet->packet_periods[i].packet_id;
				memcpy(&an_packet->data[2 + 5*i + 1], &packet_periods_packet->packet_periods[i].period, sizeof(uint32_t));
			}
			else break;
		}
		an_packet->length = 2 + 5*i;
	}
    return an_packet;
}

int decode_sensor_ranges_packet(sensor_ranges_packet_t *sensor_ranges_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_sensor_ranges && an_packet->length == 4)
	{
		memcpy(sensor_ranges_packet, an_packet->data, 4*sizeof(uint8_t));
		return 0;
	}
	else return 1;
}

an_packet_t *encode_sensor_ranges_packet(sensor_ranges_packet_t *sensor_ranges_packet)
{
	an_packet_t *an_packet = an_packet_allocate(4, packet_id_sensor_ranges);
	if(an_packet != NULL)
	{
		memcpy(an_packet->data, sensor_ranges_packet, 4*sizeof(uint8_t));
	}
	return an_packet;
}


// NAVIGATION

int decode_raw_imu_nav_packet(imu_nav_t *imu_nav_packet, an_packet_t *an_packet)
{
	if(an_packet->id == packet_id_raw_sensors && an_packet->length == 48)
	{
		//memcpy(&raw_sensors_packet->accelerometers[0], &an_packet->data[0], 3 * sizeof(float));
		memcpy(&imu_nav_packet->acc_x, &an_packet->data[0], sizeof(float));
		memcpy(&imu_nav_packet->acc_y, &an_packet->data[4], sizeof(float));
		memcpy(&imu_nav_packet->acc_z, &an_packet->data[8], sizeof(float));
		memcpy(&imu_nav_packet->gyr_x, &an_packet->data[12], sizeof(float));
		memcpy(&imu_nav_packet->gyr_y, &an_packet->data[16], sizeof(float));
		memcpy(&imu_nav_packet->gyr_z, &an_packet->data[20], sizeof(float));

		return 0;
	}
	else if(an_packet->id == packet_id_raw_gnss && an_packet->length == 74)
	{

		//memcpy(&imu_nav_packet->position[0], &an_packet->data[8], 3*sizeof(double));
		memcpy(&imu_nav_packet->gps_lat, &an_packet->data[8], sizeof(double));
		memcpy(&imu_nav_packet->gps_long, &an_packet->data[16], sizeof(double));
		memcpy(&imu_nav_packet->gps_height, &an_packet->data[24], sizeof(double));

		//memcpy(&imu_nav_packet->velocity[0], &an_packet->data[32], 3*sizeof(float));
		memcpy(&imu_nav_packet->vel_north, &an_packet->data[32], sizeof(float));
		memcpy(&imu_nav_packet->vel_east, &an_packet->data[36], sizeof(float));
		memcpy(&imu_nav_packet->vel_down, &an_packet->data[40], sizeof(float));

		return 0;
	}
	else return 1;
}

//void get_uav_nav(uav_nav_t *uav_nav_packet, *uav_nav) {
//
//	memcpy(&uav_nav_packet->position_Lat_init, &uav_nav[0], sizeof(double));
//	memcpy(&uav_nav_packet->position_Lat_init, 1111, sizeof(double));
//	memcpy(&uav_nav_packet->position_Long_init, 2222, sizeof(double));
//	memcpy(&uav_nav_packet->position_Alt_init, 3333, sizeof(double));
//
//	memcpy(&uav_nav_packet->velocity_N_init, 4444, sizeof(double));
//	memcpy(&uav_nav_packet->velocity_E_init, 5555, sizeof(double));
//	memcpy(&uav_nav_packet->velocity_D_init, 6666, sizeof(double));
//
//	memcpy(&uav_nav_packet->roll_init, 7777, sizeof(double));
//	memcpy(&uav_nav_packet->pitch_init, 8888, sizeof(double));
//	memcpy(&uav_nav_packet->yaw_init, 9999, sizeof(double));

//}
