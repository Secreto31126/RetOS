#include "ethernet.h"

extern char tr_buffer[3][TR_BUFFER_SIZE];
extern char rx_buffer[RE_BUFFER_SIZE];

EthernetFrame messages[MESSAGE_BUFFER_SIZE] = {{{0}}};
int nextToWrite = 0;
int nextToRead = 0;

void transmit(EthernetFrame *frame);
void printFrame(EthernetFrame *frame);

// void send_msg(EthernetMessage msg)
// {
//     EthernetFrame frame;

//     memcpy(frame.dest, msg.mac, MAC_SIZE);

//     get_MAC_address(frame.src);

//     frame.length = msg.length + 1;

//     frame.payload[0] = msg.type;
//     memcpy(frame.payload + 1, msg.message, msg.length);

//     frame.check = msg.type;

//     transmit(&frame);
// }

// int get_msg(EthernetMessage *msg)
// {
//     if (nextToRead == nextToWrite)
//         return 0;

//     memcpy(msg->mac, messages[nextToRead].src, MAC_SIZE);

//     msg->length = messages[nextToRead].length - 1;

//     msg->type = messages[nextToRead].payload[0];
//     memcpy(msg->message, messages[nextToRead].payload + 1, msg->length);

//     nextToRead++;
//     nextToRead %= MESSAGE_BUFFER_SIZE;

//     return msg->length + MAC_SIZE + sizeof(msg->length);
// }

void ethernet_manager()
{
    uint16_t status = input_word(IO_ADDRESS + ISR);
    output_word(IO_ADDRESS + ISR, 0x5);

    // Recive OK
    if (status & CHECK_ROK)
    {
        ncPrint("-----Message recieved-----\n");

        EthernetFrame *frame = (EthernetFrame *)(rx_buffer + 4);

        ncClear();
        printFrame(frame);
        if (!messages[nextToWrite].dest[0] && isPacketDestinedForThisDevice(frame, true))
        {
            ncClear();

            memcpy(messages + (nextToWrite++), frame, sizeof(EthernetFrame));
            printFrame(messages + nextToRead);

            uint8_t mac[MAC_SIZE];
            get_MAC_address(mac);

            EthernetFrame response = {
                .length = 3,
                .payload = "Hi",
                .check = 0x00,
            };

            memcpy(response.dest, frame->src, sizeof(response.dest));
            memcpy(response.src, mac, sizeof(response.src));

            transmit(&response);
        }
        else
        {
            // buffer lleno o no es para mi
            ncPrint("BUFFER LLENO o no es para mi\n");
        }

        // verifico si llegue al final del buffer de mensajes
        nextToWrite %= MESSAGE_BUFFER_SIZE;
        output_word(IO_ADDRESS + ISR, status & CLEAR_ROK);
    }

    // Transmit OK
    else if (status & CHECK_TOK)
    {
        ncClear();
        ncPrint("-----Message sent-----\n");

        // pongo el TOK en 0
        output_word(IO_ADDRESS + ISR, status & CLEAR_TOK);
    }

    init_ethernet();
}

void transmit(EthernetFrame *frame)
{
    if (frame == NULL)
        return;

    int length = 0;

    /* Copy ethernet frame to transmit buffer */
    memcpy(tr_buffer[0] + length, frame->dest, MAC_SIZE);
    length += MAC_SIZE;

    memcpy(tr_buffer[0] + length, frame->src, MAC_SIZE);
    length += MAC_SIZE;

    tr_buffer[0][length] = frame->length;
    length += 2;

    memcpy(tr_buffer[0] + length, frame->payload, frame->length);
    length += frame->length;

    tr_buffer[0][length] = frame->check;
    length += 2;

    output_dword(IO_ADDRESS + TSD0, length & CLEAR_OWN); // clear own bit
}

void printFrame(EthernetFrame *frame)
{
    ncPrint("Mac destination:");
    for (int i = 0; i < MAC_SIZE; i++)
    {
        ncPrint(" ");
        ncPrintHex(frame->dest[i]);
    }

    ncPrint("\nMac Source:");
    for (int i = 0; i < MAC_SIZE; i++)
    {
        ncPrint(" ");
        ncPrintHex(frame->src[i]);
    }

    ncPrint("\nCheck: ");
    ncPrintDec(frame->check);

    ncPrint("\nLength: ");
    ncPrintDec(frame->length);

    ncPrint("\n-----Start of message-----\n");
    for (int i = 0; i < frame->length && i < 256; i++)
        ncPrintChar(frame->payload[i], 0x0F);

    ncPrint("\n-----End of message-----\n");
}
