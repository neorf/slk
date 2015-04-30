#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "an_packet_protocol.h"

/*
 * Function to calculate the CRC16 of data
 * CRC16-CCITT
 * Initial value = 0xFFFF
 * Polynomial = x^16 + x^12 + x^5 + x^0
 */
uint16_t calculate_crc16(const void *data, uint16_t length)
{
	uint8_t *bytes = (uint8_t *) data;
	uint16_t crc = 0xFFFF, i;
	for (i = 0; i < length; i++)
	{
		crc = (uint16_t)((crc << 8) ^ crc16_table[(crc >> 8) ^ bytes[i]]);
	}
	return crc;
}

/*
 * Initialise the decoder
 */
void an_decoder_initialise(an_decoder_t *an_decoder)
{
	an_decoder->buffer_length = 0;
	an_decoder->crc_errors = 0;
}

/*
 * Function to dynamically allocate an an_packet
 */
an_packet_t *an_packet_allocate(uint8_t length, uint8_t id)
{
	an_packet_t *an_packet = malloc(sizeof(an_packet_t) + length * sizeof(uint8_t));
	if (an_packet != NULL)
	{
		an_packet->id = id;
		an_packet->length = length;
	}
	return an_packet;
}

/*
 * Function to calculate a 4 byte LRC
 */
uint8_t calculate_header_lrc(uint8_t *data)
{
	//LRC = ((packet_id + packet_length + crc[0] + crc[1])^0xFF) + 1
	return ((data[0] + data[1] + data[2] + data[3]) ^ 0xFF) + 1;
}

/*
 * Function to decode an_packets from raw data
 * Returns a pointer to the packet decoded or NULL if no packet was decoded
 */
an_packet_t *an_packet_decode(an_decoder_t *an_decoder)
{
	uint16_t decode_iterator = 0;
	an_packet_t *an_packet = NULL;
	uint8_t header_lrc, id, length;
	uint16_t crc;

	while (decode_iterator + AN_PACKET_HEADER_SIZE <= an_decoder->buffer_length)
	{
		header_lrc = an_decoder->buffer[decode_iterator++];
		if (header_lrc == calculate_header_lrc(&an_decoder->buffer[decode_iterator]))
		{
			id = an_decoder->buffer[decode_iterator++];
			length = an_decoder->buffer[decode_iterator++];
			crc = an_decoder->buffer[decode_iterator++];
			crc |= an_decoder->buffer[decode_iterator++] << 8;

			if (decode_iterator + length > an_decoder->buffer_length)
			{
				decode_iterator -= AN_PACKET_HEADER_SIZE;
				break;
			}

			if (crc == calculate_crc16(&an_decoder->buffer[decode_iterator], length))
			{
				an_packet = an_packet_allocate(length, id);
				if (an_packet != NULL)
				{
					memcpy(an_packet->header, &an_decoder->buffer[decode_iterator - AN_PACKET_HEADER_SIZE], AN_PACKET_HEADER_SIZE * sizeof(uint8_t));
					memcpy(an_packet->data, &an_decoder->buffer[decode_iterator], length * sizeof(uint8_t));
				}
				decode_iterator += length;
				break;
			}
			else
			{
				decode_iterator -= (AN_PACKET_HEADER_SIZE - 1);
				an_decoder->crc_errors++;
			}
		}
	}

	if (decode_iterator < an_decoder->buffer_length)
	{
		if (decode_iterator > 0)
		{
			memmove(&an_decoder->buffer[0], &an_decoder->buffer[decode_iterator], (an_decoder->buffer_length - decode_iterator) * sizeof(uint8_t));
			an_decoder->buffer_length -= decode_iterator;
		}
	}
	else an_decoder->buffer_length = 0;

	return an_packet;
}

/*
 * Function to encode an an_packet
 */
void an_packet_encode(an_packet_t *an_packet)
{
	uint16_t crc;
	an_packet->header[1] = an_packet->id;
	an_packet->header[2] = an_packet->length;
	crc = calculate_crc16(an_packet->data, an_packet->length);
	memcpy(&an_packet->header[3], &crc, sizeof(uint16_t));
	an_packet->header[0] = calculate_header_lrc(&an_packet->header[1]);
}

/*
 * Function to free an an_packet
 */
void an_packet_free(an_packet_t **an_packet)
{
	free(*an_packet);
	*an_packet = NULL;
}
