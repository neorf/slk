/****************************************************************/
/*  SLKW LIBRARY 1 */
/****************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>



#include "rs232/rs232.h"
#include "an_packet_protocol.h"
#include "spatial_packets.h"

#include "interfaces.h"

// TIME FUNCTION START

#include <math.h>

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;



    return (diff<0);
}


void timeval_print(struct timeval *tv)
{
    char buffer[30];
    time_t curtime;

    printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
    curtime = tv->tv_sec;
    strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
    printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

// TIME FUNCTION END


int an_packet_transmit(an_packet_t *an_packet)
{
	an_packet_encode(an_packet);
	return SendBuf(an_packet_pointer(an_packet), an_packet_size(an_packet));
}

#define RADIANS_TO_DEGREES (180.0/M_PI)

void set_request_packet(int packet_id_request)
{
	an_packet_t *an_packet;

	an_packet = encode_request_packet(packet_id_request);

	an_packet_transmit(an_packet);

	an_packet_free(&an_packet);
}

void set_packet_timer_period(int permanent,int timer_period)
{

	an_packet_t *an_packet;

	packet_timer_period_packet_t packet_timer_period_packet;

	packet_timer_period_packet.permanent = permanent;
	packet_timer_period_packet.utc_synchronisation = 0;
	packet_timer_period_packet.packet_timer_period =  timer_period;

	an_packet = encode_packet_timer_period_packet(&packet_timer_period_packet);

	an_packet_transmit(an_packet);

	an_packet_free(&an_packet);
}


void set_encode_packet_periods_packet(int permanent,int period, int packet_ids[], int num_ids)
{
	int i;
	an_packet_t *an_packet2;

	packet_periods_packet_t packet_periods_packet2;

	packet_periods_packet2.permanent = permanent;
	packet_periods_packet2.clear_existing_packets = 1;

	for(i = 0; i < num_ids; i++)
	{

		packet_periods_packet2.packet_periods[i].packet_id = packet_ids[i];
		packet_periods_packet2.packet_periods[i].period = period;

	}

	an_packet2 = encode_packet_periods_packet(&packet_periods_packet2, num_ids);

	an_packet_transmit(an_packet2);

	an_packet_free(&an_packet2);
}

void set_encode_sensor_ranges_packet(int set_permanent, int set_acc_ranges, int set_gyr_range, int set_magn_range)
{

	an_packet_t *an_packet;

	sensor_ranges_packet_t sensor_ranges_packet;

	sensor_ranges_packet.permanent = 1; // permanent = 0 unable to set
	sensor_ranges_packet.accelerometers_range = set_acc_ranges;
	sensor_ranges_packet.gyroscopes_range = set_gyr_range;
	sensor_ranges_packet.magnetometers_range = set_magn_range;


	an_packet = encode_sensor_ranges_packet(&sensor_ranges_packet);

	an_packet_transmit(an_packet);

	an_packet_free(&an_packet);
}



int main(int argc, char *argv[])
{

	an_decoder_t an_decoder;
	an_packet_t *an_packet;

	//NAVIGATION MODULE
	imu_nav_t imu_nav_packet;
	uav_nav_t uav_nav_packet;
	nav_cntr_t nav_cntr_packet;
	nav_guid_t nav_guid_packet;

	//GUIDE MODULE
	guid_cntr_t guid_cntr_packet;
	gimbal_guid_t gimbal_guid_packet;

	//CONTROL MODULE
	cntr_act_t cntr_act_packet;

	system_state_packet_t system_state_packet;
	raw_sensors_packet_t raw_sensors_packet;
	running_time_packet_t running_time_packet;
	device_information_packet_t device_information_packet;
	raw_gnss_packet_t raw_gnss_packet;

	/* configuration */
	packet_timer_period_packet_t packet_timer_period_packet;
	packet_period_t packet_period;
	packet_periods_packet_t packet_periods_packet;
	sensor_ranges_packet_t sensor_ranges_packet;

	clock_t t;

	struct timeval tvBegin, tvEnd, tvDiff;



	int bytes_received;
	int first_packet_found=0;
    int memoria200=0,memoria100=0,memoria50=0,nopacket=0,step1_error=0,step2_error=0,step3_error=0;

    int conteggio_raw_sensors=0,conteggio_raw_gnss=0,conteggio_packet_timer_period=0,conteggio_sensor_ranges=0;
    int step1=0,step2=0,step3=0,step=0,elaborate=0,memoriastep=0;
    int r=0,conta200=0,conta100=0,conta50=0,conteggio_system_state=0,count_exit=0;

    //Configure elements in array to configure in packet period. Doesn't work
    int packet_id_period[]={20,28,29};
    int set_packet_period=2; // Packet Period

    //Array value navigation
    //double uav_nav[9] = {1000.0, 2.0, 3.4, 7.0, 50.0, 30.0, 70.0,90.0,70.0};

    //configure Navigation UAV/NAV
    uav_nav_packet.position_Lat_init=222.0;
    uav_nav_packet.position_Long_init=333.0;
    uav_nav_packet.position_Alt_init=444.0;

    uav_nav_packet.velocity_N_init=777.0;
    uav_nav_packet.velocity_E_init=888.0;
    uav_nav_packet.velocity_D_init=222.0;

    uav_nav_packet.roll_init=999.0;
    uav_nav_packet.pitch_init=001.0;
    uav_nav_packet.yaw_init=123.0;




	if (argc != 4)
	{
		printf("Usage - program com_port baud_rate cycles_to_break\nExample - ./run /dev/ttyUSB0 115200 200000\n");
		exit(EXIT_FAILURE);
	}

	/* open the com port */
	if (OpenComport(argv[1], atoi(argv[2])))
	{
		printf("Could not open serial/USB port!!!\n");
		exit(EXIT_FAILURE);
	}

	an_decoder_initialise(&an_decoder);
	printf("* * * START * * *\n");

	// FIRST - SET period for packets

	set_encode_packet_periods_packet(1,set_packet_period,packet_id_period,NumElements(packet_id_period)); //set permanent and period
	set_packet_timer_period(1,1250); //set period for packet //min 2000, max 65536

	// begin
	gettimeofday(&tvBegin, NULL);
	timeval_print(&tvBegin);

    t = clock();

	while (1)
	{




		//Base Freq. 400 Hz (IMU)
		if ((bytes_received = PollComport(an_decoder_pointer(&an_decoder), an_decoder_size(&an_decoder))) > 0)
		{


			/* increment the decode buffer length by the number of bytes received */
			an_decoder_increment(&an_decoder, bytes_received);

			/* decode all the packets in the buffer */
			while ((an_packet = an_packet_decode(&an_decoder)) != NULL && first_packet_found==0)
			{
				//printf("* ID: %u\n",an_packet->id);

				if(an_packet->id == packet_id_system_state) {
					conteggio_system_state++;
					step1++;
					elaborate=0;
									if(decode_system_state_packet(&system_state_packet, an_packet) == 0) { }
									else step1_error++;
				}

				else if(an_packet->id == packet_id_raw_sensors) {
					conteggio_raw_sensors++;
					step2++;
					elaborate=0;
									if(decode_raw_imu_nav_packet(&imu_nav_packet, an_packet) == 0) { }
									else step2_error++;
								}

				else if(an_packet->id == packet_id_raw_gnss) {
					conteggio_raw_gnss++;
					step3++;
					elaborate=1;
									if(decode_raw_imu_nav_packet(&imu_nav_packet, an_packet) == 0) { }
									else step3_error++;
								}
				else
					nopacket++;


				// START MODULE FREQUENCY
				if((conteggio_system_state>0 && conteggio_raw_sensors>0 && conteggio_raw_gnss>0) && elaborate==1) {
					step++;
					memoriastep++;


					if((step % FREQ_200)==0) {
									conta200++;
									memoria200++;

									navigation(&imu_nav_packet, &uav_nav_packet, &nav_cntr_packet, &nav_guid_packet);
								}
					if(step % FREQ_100==0) {
									conta100++;
									memoria100++;
									control(&imu_nav_packet, &nav_cntr_packet, &guid_cntr_packet, &cntr_act_packet);
								}

					if(step % FREQ_50==0) {
									conta50++;
									memoria50++;
									guide(&nav_guid_packet, &guid_cntr_packet, &gimbal_guid_packet);
					}



				}


				/* Ensure that you free the an_packet when your done with it or you will leak memory */
				an_packet_free(&an_packet);
				//first_packet_found++;

			}
			r=r+bytes_received;

		}
	//Reset counters
    if(step == BASE_FREQ_400-1) {
    	step=0;
    	conta200=0;
    	conta100=0;
    	conta50=0;

    	conteggio_system_state=1;
    	conteggio_raw_sensors=1;
    	conteggio_raw_gnss=1;
    }



    count_exit++;
    //if(count_exit==50000000) break;
    if(count_exit==atoi(argv[3])) break;

	}
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds



    printf("|---------------------------------------|\n");
	printf("conteggio_system_state: %u\n",conteggio_system_state);
	printf("conteggio_raw_sensors: %u\n",conteggio_raw_sensors);
	printf("conteggio_raw_gnss: %u\n",conteggio_raw_gnss);
	printf("conteggio_packet_timer_period: %u\n",conteggio_packet_timer_period);
	printf(" conteggio_sensor_ranges: %u\n",conteggio_sensor_ranges);
	printf("|---------------------------------------|\n");
	printf("Total step1: %u\n", step1);
	printf("Total step2: %u\n", step2);
	printf("Total step3: %u\n", step3);
	printf("|---------------------------------------|\n");
	printf("step: %u\n",step);
	printf("conta200: %u\n",conta200);
	printf("conta100: %u\n",conta100);
	printf("conta50: %u\n",conta50);
	printf("|---------------------------------------|\n");
	printf("Bytes Ricevuti Totali: %u\n",r);
	printf("Cicli Totali Impostati %u - Cicli effettivi: %u\n",atoi(argv[3]),count_exit);
	fflush(stdout);
	//end
	    gettimeofday(&tvEnd, NULL);
	    timeval_print(&tvEnd);

	     //diff
	    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);



//	    printf("RATE conteggio_system_state: %ld Hz\n", conteggio_system_state / (tvDiff.tv_sec));
//	    printf("RATE conteggio_raw_sensors: %ld Hz\n", conteggio_raw_sensors / (tvDiff.tv_sec));
//	    printf("RATE conteggio_raw_gnss: %ld Hz\n", conteggio_raw_gnss / (tvDiff.tv_sec));
//	    printf("RATE conteggio_packet_timer_period: %ld Hz\n", conteggio_packet_timer_period / (tvDiff.tv_sec));
//	    printf("RATE conteggio_sensor_ranges: %ld Hz\n", conteggio_sensor_ranges / (tvDiff.tv_sec));


		printf("|---------------------------------------|\n");
	    //printf("SECOND TIME took %f seconds to execute \n", time_taken);
		printf("EXECUTION TIME: %ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	    	    printf("RATE memoriastep: %ld Hz\n", memoriastep / (tvDiff.tv_sec));
	    	    printf("RATE memoria200: %ld Hz\n", memoria200 / (tvDiff.tv_sec));
	    	    printf("RATE memoria100: %ld Hz\n", memoria100 / (tvDiff.tv_sec));
	    	    printf("RATE memoria50: %ld Hz\n", memoria50 / (tvDiff.tv_sec));

//	    printf("RATE memoriastep: %f Hz\n", memoriastep / time_taken);
//	    printf("RATE memoria200: %f Hz\n", memoria200 / time_taken);
//	    printf("RATE memoria100: %f Hz\n", memoria100 / time_taken);
//	    printf("RATE memoria50: %f Hz\n", memoria50 / time_taken);
	    printf("-->memoria200: %u \n", memoria200);
	    printf("-->memoria100: %u \n", memoria100);
	    printf("-->memoria500: %u \n", memoria50);
	    printf("|---------------------------------------|\n");
	    printf("NoPacket: %u\n",nopacket);
	    printf("STEP1_ERROR: %u\n",step1_error);
	    printf("STEP2_ERROR: %u\n",step2_error);
	    printf("STEP3_ERROR: %u\n",step3_error);






	    fflush(stdout);

}


