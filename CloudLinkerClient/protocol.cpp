#include "protocol.h"


PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen=sizeof(PDU)+uiMsgLen;

    PDU *pdu=(PDU*)malloc(uiPDULen);
    if(pdu==nullptr)
    {
        exit(EXIT_FAILURE);
    }

    memset(pdu,0,uiPDULen);

    pdu->uiPDULen=uiPDULen;
    pdu->uiMsgLen=uiMsgLen;

    return pdu;
}
