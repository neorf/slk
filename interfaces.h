#define START_NAVIGATION_PACKETS 28

// NAVIGATION INTERFACES
typedef enum
{

	nav_packet_id_raw_sensors = START_NAVIGATION_PACKETS,
	nav_packet_id_raw_gnss

} nav_packet_ids;

void navigation(imu_nav_t *imu_nav_packet, uav_nav_t *uav_nav_packet, nav_cntr_t *nav_cntr_packet, nav_guid_t *nav_guid_packet);
void control(imu_nav_t *imu_nav_packet, nav_cntr_t *nav_cntr_packet, guid_cntr_t *guid_cntr_packet, cntr_act_t *cntr_act_packet);
void guide(nav_guid_t *nav_guid_packet, guid_cntr_t *guid_cntr_packet, gimbal_guid_t *gimbal_guid_packet);
