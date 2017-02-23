#ifndef PACKET_H
#define PACKET_H

const size_t PACKET_SIZE            = 4*4*1024;
const char   HEADER                 = 'X';
const char   TAIL                   = 'Y';
const size_t PACKET_TEST_LIMIT      = 1024u*1024u;

//#pragma pack(push, 1)
struct Packet
{
    char   marker[4] = {HEADER, HEADER, HEADER, HEADER};
    qint64 timestamp;
    char   data[PACKET_SIZE - sizeof(timestamp) - sizeof(marker)];
} __attribute__((packed));
//#pragma pack(pop)

#endif // PACKET_H
