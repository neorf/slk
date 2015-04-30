#define MAXIMUM_PACKET_PERIODS 50

#define START_CONFIGURATION_PACKETS 180
#define START_STATE_PACKETS 20

typedef struct
{
	union
	{
		uint16_t r;
		struct
		{
			unsigned int system_failure :1;
			unsigned int accelerometer_sensor_failure :1;
			unsigned int gyroscope_sensor_failure :1;
			unsigned int magnetometer_sensor_failure :1;
			unsigned int pressure_sensor_failure :1;
			unsigned int gnss_failure :1;
			unsigned int accelerometer_over_range :1;
			unsigned int gyroscope_over_range :1;
			unsigned int magnetometer_over_range :1;
			unsigned int pressure_over_range :1;
			unsigned int minimum_temperature_alarm :1;
			unsigned int maximum_temperature_alarm :1;
			unsigned int low_voltage_alarm :1;
			unsigned int high_voltage_alarm :1;
			unsigned int gnss_antenna_disconnected :1;
			unsigned int serial_port_overflow_alarm :1;
		} b;
	} system_status;
	union
	{
		uint16_t r;
		struct
		{
			unsigned int orientation_filter_initialised :1;
			unsigned int ins_filter_initialised :1;
			unsigned int heading_initialised :1;
			unsigned int utc_time_initialised :1;
			unsigned int gnss_fix_type :3;
			unsigned int event1_flag :1;
			unsigned int event2_flag :1;
			unsigned int internal_gnss_enabled :1;
			unsigned int magnetic_heading_enabled :1;
			unsigned int velocity_heading_enabled :1;
			unsigned int atmospheric_altitude_enabled :1;
			unsigned int external_position_active :1;
			unsigned int external_velocity_active :1;
			unsigned int external_heading_active :1;
		} b;
	} filter_status;
	uint32_t unix_time_seconds;
	uint32_t microseconds;
	double latitude;
	double longitude;
	double height;
	float velocity[3];
	float body_acceleration[3];
	float g_force;
	float orientation[3];
	float angular_velocity[3];
	float standard_deviation[3];
} system_state_packet_t;


typedef struct
{
	float accelerometers[3];
	float gyroscopes[3];
	float magnetometers[3];
	float imu_temperature;
	float pressure;
	float pressure_temperature;
} raw_sensors_packet_t;

typedef enum
{
	packet_id_acknowledge,
	packet_id_request,
	packet_id_boot_mode,
	packet_id_device_information,
	packet_id_restore_factory_settings,
	packet_id_reset,
	packet_id_print,
	packet_id_file_transfer_request,
	packet_id_file_transfer_acknowledge,
	packet_id_file_transfer,
	packet_id_serial_port_passthrough,
	end_system_packets,

	packet_id_system_state = START_STATE_PACKETS,
	packet_id_unix_time,
	packet_id_formatted_time,
	packet_id_status,
	packet_id_position_standard_deviation,
	packet_id_velocity_standard_deviation,
	packet_id_euler_orientation_standard_deviation,
	packet_id_quaternion_orientation_standard_deviation,
	packet_id_raw_sensors,
	packet_id_raw_gnss,
	packet_id_satellites,
	packet_id_satellites_detailed,
	packet_id_geodetic_position,
	packet_id_ecef_position,
	packet_id_utm_position,
	packet_id_ned_velocity,
	packet_id_body_velocity,
	packet_id_acceleration,
	packet_id_body_acceleration,
	packet_id_euler_orientation,
	packet_id_quaternion_orientation,
	packet_id_dcm_orientation,
	packet_id_angular_velocity,
	packet_id_angular_acceleration,
	packet_id_external_position_velocity,
	packet_id_external_position,
	packet_id_external_velocity,
	packet_id_external_body_velocity,
	packet_id_external_heading,
	packet_id_running_time,
	packet_id_local_magnetics,
	packet_id_odometer_state,
	packet_id_external_time,
	packet_id_external_depth,
	packet_id_geoid_height,
	packet_id_rtcm_corrections,
	packet_id_external_pitot_pressure,
	packet_id_wind,
	packet_id_heave,
	packet_id_post_processing,
	packet_id_raw_satellite_data,
	packet_id_raw_satellite_ephemeris,
	packet_id_depth,
	packet_id_water_profiling,
	packet_id_external_usbl,
	packet_id_speed_of_sound,
	packet_id_lockheed,
	packet_id_external_odometer,
	packet_id_external_air_data,
	packet_id_gnss_receiver_information,
	end_state_packets,

	packet_id_packet_timer_period = START_CONFIGURATION_PACKETS,
	packet_id_packet_periods,
	packet_id_baud_rates,
	packet_id_bus_configuration,
	packet_id_sensor_ranges,
	packet_id_installation_alignment,
	packet_id_filter_options,
	packet_id_filter_trust,
	packet_id_gpio_configuration,
	packet_id_magnetic_calibration_values,
	packet_id_magnetic_calibration_configuration,
	packet_id_magnetic_calibration_status,
	packet_id_odometer_configuration,
	packet_id_zero_alignment,
	packet_id_heave_offset,
	packet_id_gpio_output_configuration,
	packet_id_dual_antenna_configuration,
	end_configuration_packets
} packet_id_e;

typedef struct
{
	uint32_t seconds;
	uint32_t microseconds;
} running_time_packet_t;

typedef struct
{
	uint32_t software_version;
	uint32_t device_id;
	uint32_t hardware_revision;
	uint32_t serial_number[3];
} device_information_packet_t;

typedef struct
{
	uint32_t unix_time_seconds;
	uint32_t microseconds;
	double position[3];
	float velocity[3];
	float position_standard_deviation[3];
	float tilt; /* This field will only be valid if an external dual antenna GNSS system is connected */
	float heading; /* This field will only be valid if an external dual antenna GNSS system is connected */
	float tilt_standard_deviation; /* This field will only be valid if an external dual antenna GNSS system is connected */
	float heading_standard_deviation; /* This field will only be valid if an external dual antenna GNSS system is connected */
	union
	{
		uint16_t r;
		struct
		{
			unsigned int fix_type:3;
			unsigned int velocity_valid:1;
			unsigned int time_valid:1;
			unsigned int external_gnss:1;
			unsigned int tilt_valid:1; /* This field will only be valid if an external dual antenna GNSS system is connected */
			unsigned int heading_valid:1; /* This field will only be valid if an external dual antenna GNSS system is connected */
		} b;
	} flags;
} raw_gnss_packet_t;


/* CONFIGURATION PACKETS */

typedef struct
{
	uint8_t permanent;
	uint8_t utc_synchronisation;
	uint16_t packet_timer_period;
} packet_timer_period_packet_t;

typedef struct
{
	uint8_t packet_id;
	uint32_t period;
} packet_period_t;

typedef struct
{
	uint8_t permanent;
	uint8_t clear_existing_packets;
	packet_period_t packet_periods[MAXIMUM_PACKET_PERIODS];
} packet_periods_packet_t;

typedef struct
{
	uint8_t permanent;
	uint8_t accelerometers_range;
	uint8_t gyroscopes_range;
	uint8_t magnetometers_range;
} sensor_ranges_packet_t;

// IMU/NAV
typedef struct
{
	//packet id 28 - Raw Sensors Packet
	float acc_x;
	float acc_y;
	float acc_z;
	float gyr_x;
	float gyr_y;
	float gyr_z;

	//packet id 29 - Raw GNSS Packet
	double gps_lat;
	double gps_long;
	double gps_height;
	float vel_north;
	float vel_east;
	float vel_down;

} imu_nav_t;

// UAV/NAV
typedef struct
{
	double position_Lat_init;
	double position_Long_init;
	double position_Alt_init;

	double velocity_N_init;
	double velocity_E_init;
	double velocity_D_init;

	double roll_init;
	double pitch_init;
	double yaw_init;

} uav_nav_t;


// NAV/CNTR
typedef struct
{
	double velocity_N;
	double velocity_E;
	double velocity_D;

	double roll;
	double pitch;
	double yaw;


} nav_cntr_t;

// NAV/GUID
typedef struct
{
	double position_Lat;
	double position_Long;
	double position_Alt;

	double velocity_N;
	double velocity_E;
	double velocity_D;

	double roll;
	double pitch;
	double yaw;

} nav_guid_t;

// GUID/CNTR
typedef struct
{
	double acc_x;
	double acc_y;
	double acc_z;


} guid_cntr_t;

// GIMBAL/GUID
typedef struct

{
	int16_t azimuth;
	int32_t elevation;

	int8_t x_pos;
	int8_t y_pos;


} gimbal_guid_t;

// CNTR/ACT
typedef struct

{
	double def_1;
	double def_2;

} cntr_act_t;

int decode_system_state_packet(system_state_packet_t *system_state_packet, an_packet_t *an_packet);
int decode_device_information_packet(device_information_packet_t *device_information_packet, an_packet_t *an_packet);
int decode_raw_gnss_packet(raw_gnss_packet_t *raw_gnss_packet, an_packet_t *an_packet);
int decode_packet_timer_period_packet(packet_timer_period_packet_t *packet_timer_period_packet, an_packet_t *an_packet);
an_packet_t *encode_packet_timer_period_packet(packet_timer_period_packet_t *packet_timer_period_packet);
int decode_packet_periods_packet(packet_periods_packet_t *packet_periods_packet, an_packet_t *an_packet);
an_packet_t *encode_packet_periods_packet(packet_periods_packet_t *packet_periods_packet, int num_ids);
int decode_sensor_ranges_packet(sensor_ranges_packet_t *sensor_ranges_packet, an_packet_t *an_packet);
an_packet_t *encode_sensor_ranges_packet(sensor_ranges_packet_t *sensor_ranges_packet);
an_packet_t *encode_request_packet(uint8_t requested_packet_id);
int decode_raw_imu_nav_packet(imu_nav_t *imu_nav_packet, an_packet_t *an_packet);

//void get_uav_nav(uav_nav_t *uav_nav_packet, *uav_nav);
