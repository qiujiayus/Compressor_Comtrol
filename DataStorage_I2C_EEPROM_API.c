/*
 * DataStorage_I2C_EEPROM_API.c
 *
 *  Created on: 2024年2月1日
 *      Author: wangyz1
 */
#include "f28002x_device.h"         // f28002x Headerfile Include File
#include "f28002x.h"       // f28002x Include File

#define EEPROMWriteTest     0
#define EEPROMReadTest      0


#if StorageType==StorageType_EEPROM

uint16_t handleNACK(uint32_t base)
{
    if(I2C_getStatus(base) & I2C_STS_NO_ACK)
    {
        I2C_clearStatus(base, I2C_STS_NO_ACK);
//        I2C_sendStopCondition(base);

        return I2C_ERROR_NACK_RECEIVED;
    }
    return I2C_SUCCESS;
}

uint16_t checkBusStatus(uint32_t base)
{
    if(I2C_isBusBusy(base))
    {
        return I2C_ERROR_BUS_BUSY;
    }

    if(I2C_getStopConditionStatus(base))
    {
        return I2C_ERROR_STOP_NOT_READY;
    }

    return I2C_SUCCESS;
}
void EEPROM_Storage_DataPrepare()
{
    DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
    DataFlash.EEPROM.WritePtr           = 0;
    DataFlash.EEPROM.WritePtrAllData    = 0;
    DataFlash.EEPROM.state = I2C_WritePrepare;

//    Uint16 i;
//    for(i=0;i<dDataStorage_DataLength;i++)
//    {
//        DataFlash.data[i] = 666;
//    }
    Uint16 sizeof_data = sizeof(unBlackBoxStorageInfo.u16Data);
    if( sizeof_data > dDataStorage_DataLength - 2 )
    {
        sizeof_data = dDataStorage_DataLength - 2;
    }
    memcpy_fast((Uint16*) DataFlash.data, &unBlackBoxStorageInfo.u16Data, sizeof_data);
    DataFlash.data[dDataStorage_DataLength - 1] = 0x1AA1;
    DataFlash.data[dDataStorage_DataLength - 2] = DataFlash.EEPROM.ControlAddr;

//    Uint16 sizeof_data = sizeof(unBlackBoxStorageInfo.u16Data);
//    if( sizeof_data > dDataStorage_DataLength - 3 )
//    {
//        sizeof_data = dDataStorage_DataLength - 3;
//    }

//    DataFlash.data[dDataStorage_DataLength - 1] = 0x1AA1;

    DataFlash.state = EEP_Program;

}
void DataStorage_Init_EEPROM()
{
    construct_Timer(&DataFlash.EEPROM.timer);
    DataFlash.EEPROM.TargetAddr     = 0x50;
    DataFlash.EEPROM.base           = I2CA_BASE;
    DataFlash.EEPROM.pTX_MsgBuffer  = DataFlash.data;
    DataFlash.EEPROM.pRX_MsgBuffer  = DataFlash.data;

    Uint16 n = 0;
    for(DataFlash.EEPROM.n=0;DataFlash.EEPROM.n<dEEPROM_DataMaxNum;DataFlash.EEPROM.n++)
    {
        if(DataFlash.EEPROM.n == 0)
        {
            DataFlash.EEPROM.ControlAddr   = 0;
        }
        else
        {
            DataFlash.EEPROM.ControlAddr   = dEEPROM_DataBytes * DataFlash.EEPROM.n;
        }

        DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
        DataFlash.EEPROM.WritePtr           = 0;
        DataFlash.EEPROM.WritePtrAllData    = 0;
        DataFlash.EEPROM.state = I2C_ReadPrepare;
        DataFlash.EEPROM.E2state = EEPROM_WaitReadHead1;
        while(DataFlash.EEPROM.state != I2C_Wait)
        {
            I2C_readData(&DataFlash.EEPROM);
        }


        if( DataFlash.data[dDataStorage_DataLength - 1]==0x1AA1)
        {
            n = DataFlash.EEPROM.n;
            break;
        }
    }

    for(DataFlash.EEPROM.n=n;DataFlash.EEPROM.n<dEEPROM_DataMaxNum;DataFlash.EEPROM.n++)
    {
        if(DataFlash.EEPROM.n == 0)
        {
            DataFlash.EEPROM.ControlAddr   = 0;
        }
        else
        {
            DataFlash.EEPROM.ControlAddr   = dEEPROM_DataBytes * DataFlash.EEPROM.n;
        }

        DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
        DataFlash.EEPROM.WritePtr           = 0;
        DataFlash.EEPROM.WritePtrAllData    = 0;
        DataFlash.EEPROM.state = I2C_ReadPrepare;
        DataFlash.EEPROM.E2state = EEPROM_WaitReadHead1;
        while(DataFlash.EEPROM.state != I2C_Wait)
        {
            I2C_readData(&DataFlash.EEPROM);
        }


        if( DataFlash.data[dDataStorage_DataLength - 1]!=0x1AA1)
        {
            break;
        }
    }

    if( DataFlash.EEPROM.n == 0 )
    {
        DataFlash.EEPROM.ControlAddr   = dEEPROM_DataBytes * (dEEPROM_DataMaxNum - 1);
        DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
        DataFlash.EEPROM.WritePtr           = 0;
        DataFlash.EEPROM.WritePtrAllData    = 0;
        DataFlash.EEPROM.state = I2C_ReadPrepare;
        DataFlash.EEPROM.E2state = EEPROM_WaitReadHead1;
        while(DataFlash.EEPROM.state != I2C_Wait)
        {
            I2C_readData(&DataFlash.EEPROM);
        }
        if( DataFlash.data[dDataStorage_DataLength - 1]!=0x1AA1)
        {
            DataFlash.EEPROM.ControlAddr = 0;
        }else
        {
            DataFlash.EEPROM.ControlAddr = dEEPROM_DataBytes * (dEEPROM_DataMaxNum - 1);
        }
    }
    else
    {
        DataFlash.EEPROM.ControlAddr   = dEEPROM_DataBytes * (DataFlash.EEPROM.n - 1);
    }

    DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
    DataFlash.EEPROM.WritePtr           = 0;
    DataFlash.EEPROM.WritePtrAllData    = 0;
    DataFlash.EEPROM.state = I2C_ReadPrepare;
    DataFlash.EEPROM.E2state = EEPROM_WaitReadHead1;
    while(DataFlash.EEPROM.state != I2C_Wait)
    {
        I2C_readData(&DataFlash.EEPROM);
    }

    readFlash_StorageData(DataFlash.EEPROM.pRX_MsgBuffer);


    DataFlash.state = DF_Read;

#if EEPROMWriteTest
    //EEPROM Write Test
    DataFlash.EEPROM.ControlAddr    = 0;//dEEPROM_DataBytes*1 + 1;
    DataFlash.EEPROM.NumOfAllDataBytes = dEEPROM_DataMaxBytes;
    DataFlash.EEPROM.WritePtr = 0;
    DataFlash.EEPROM.WritePtrAllData = 0;
    DataFlash.EEPROM.state = I2C_WritePrepare;
//    memcpy((Uint16*)DataFlash.data,&unBlackBoxStorageInfo.u16Data,sizeof(unBlackBoxStorageInfo.u16Data));
    Uint16 i;
    for(i=0;i<dDataStorage_DataLength;i++)
    {
        DataFlash.data[i] = 0;
    }
//    DataFlash.data[dDataStorage_DataLength - 1] = 0x1AA1;

    while(DataFlash.EEPROM.state != I2C_Wait)
    {
        I2C_writeData(&DataFlash.EEPROM);
    }
#endif
#if EEPROMReadTest
    //EEPROM Read Test
    DataFlash.EEPROM.ControlAddr        = 0;//dEEPROM_DataBytes * 1 + 1;
    DataFlash.EEPROM.NumOfAllDataBytes  = dEEPROM_DataBytes;
    DataFlash.EEPROM.WritePtr           = 0;
    DataFlash.EEPROM.WritePtrAllData    = 0;
    DataFlash.EEPROM.state = I2C_ReadPrepare;
    DataFlash.EEPROM.E2state = EEPROM_WaitReadHead1;
    while(DataFlash.EEPROM.state != I2C_Wait)
    {
        I2C_readData(&DataFlash.EEPROM);
    }
    ESTOP0;
#endif
//    }
//    ESTOP0;



//    DataFlash.state = EEP_WaitErase;



}
void I2C_writeData(struct I2CHandle *I2C_Params)
{
    uint16_t status;
    uint16_t temp;
    uint16_t temp_1;
    uint16_t temp_PageByte;
    Uint16 u16EEPROMAddr;
    switch(I2C_Params->state)
    {
    case I2C_WritePrepare:
        DataFlash.EEPROM.NumOfDataBytes = DataFlash.EEPROM.NumOfAllDataBytes - DataFlash.EEPROM.WritePtrAllData;
        if( DataFlash.EEPROM.NumOfDataBytes > dEEPROM_PageDataBytesLength )
        {
            DataFlash.EEPROM.NumOfDataBytes = dEEPROM_PageDataBytesLength;
        }
        temp_PageByte = (I2C_Params->ControlAddr+I2C_Params->WritePtrAllData)%dEEPROM_PageDataBytesLength;
        if( temp_PageByte && DataFlash.EEPROM.NumOfDataBytes > (dEEPROM_PageDataBytesLength - temp_PageByte) )              //BL24C64A一个Page有dEEPROM_PageDataBytesLength个Byte
        {
            DataFlash.EEPROM.NumOfDataBytes = dEEPROM_PageDataBytesLength - temp_PageByte;
        }
        if( DataFlash.EEPROM.NumOfDataBytes == 0 )
        {

            I2C_Params->state = I2C_Wait;

            if(DataFlash.state == EEP_Erase)
            {
                if(DataFlash.EEPROM.Erase_EEPROM_Finished_Count < dEEPROM_DataMaxNum / 2)
                {
                    DataFlash.EEPROM.Erase_EEPROM_Finished_Count ++;
                    DataFlash.EEPROM.ControlAddr = DataFlash.EEPROM.ControlAddr + dEEPROM_DataBytes;
                    DataFlash.state = EEP_WaitErase;
                }
                else
                {
                    DataFlash.EEPROM.Storage_EEPROM_Finished_Flag = 1;
                    DataFlash.EEPROM.Storage_EEPROM_Finished_Count ++;
                    DataFlash.state = DF_Wait;
                    if( DataFlash.EEPROM.n == dEEPROM_DataMaxNum) //All of EEPROM has already been filled
                    {

                        DataFlash.EEPROM.ControlAddr = 0;
                    }
                    else
                    {
                        DataFlash.EEPROM.ControlAddr   = dEEPROM_DataBytes * DataFlash.EEPROM.n;

                    }
                }


            }


//            ESTOP0;
        }
        else
        {
            I2C_Params->state = I2C_StartWrite;
            I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        }
        break;
    case I2C_StartWrite:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
        }
        status = checkBusStatus(I2C_Params->base);
        if(status)
        {
//            ESTOP0;
            return;
        }
        if(I2C_getStatus(I2C_Params->base) & I2C_STS_STOP_CONDITION)
        {
            I2C_clearStatus(I2C_Params->base,I2C_STS_STOP_CONDITION);
        }
        I2C_disableFIFO(I2C_Params->base);
        I2C_enableFIFO(I2C_Params->base);
        I2C_setTargetAddress(I2C_Params->base, I2C_Params->TargetAddr + (I2C_Params->ControlAddr+I2C_Params->WritePtrAllData)/dEEPROM_AddrLength);
        I2C_setDataCount(I2C_Params->base, (I2C_Params->NumOfDataBytes + dEEPROM_AddrBytes));
        I2C_Params->WritePtr = 0;
        u16EEPROMAddr = (I2C_Params->ControlAddr + I2C_Params->WritePtrAllData) % dEEPROM_AddrLength;
        I2C_putData(I2C_Params->base, u16EEPROMAddr >> 8);
        I2C_putData(I2C_Params->base, u16EEPROMAddr & 0xff);
        I2C_setConfig(I2C_Params->base, I2C_CONTROLLER_SEND_MODE);
        I2C_sendStartCondition(I2C_Params->base);
        I2C_sendStopCondition(I2C_Params->base);
        I2C_Params->state = I2C_WriteData;
        I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        break;
    case I2C_WriteData:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        while( I2C_getTxFIFOStatus(I2C_Params->base) != I2C_FIFO_TXFULL )
        {
            temp = __byte((int *)I2C_Params->pTX_MsgBuffer,I2C_Params->WritePtrAllData);
            I2C_putData(I2C_Params->base, temp);
            I2C_Params->WritePtrAllData++;
            I2C_Params->WritePtr++;
            if( I2C_Params->WritePtr == I2C_Params->NumOfDataBytes )
            {
                I2C_Params->state = I2C_WriteBusy;
                I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
                break;
            }
        }
        if(I2C_getTxFIFOStatus(I2C_Params->base) == I2C_FIFO_TXFULL)
        {
            I2C_Params->I2C_FIFO_TXFULL_Count ++;
        }
        break;
    case I2C_WriteBusy:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        status = handleNACK(I2C_Params->base);
        if(status)
        {
//            ESTOP0;
            I2C_Params->state =I2C_Error;
        }
        if(I2C_getStatus(I2C_Params->base) & I2C_STS_STOP_CONDITION)
        {
            I2C_clearStatus(I2C_Params->base,I2C_STS_STOP_CONDITION);
            I2C_Params->state = I2C_WaitWriteEeprom1;
            I2C_Params->WritePtrAllData -= I2C_Params->NumOfDataBytes;
            I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        }
        break;
    case I2C_WaitWriteEeprom1:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        status = checkBusStatus(I2C_Params->base);
        if(status)
        {
            return;
        }
        I2C_disableFIFO(I2C_Params->base);
        I2C_enableFIFO(I2C_Params->base);
        I2C_setTargetAddress(I2C_Params->base, I2C_Params->TargetAddr + (I2C_Params->ControlAddr+I2C_Params->WritePtrAllData)/dEEPROM_AddrLength);
        u16EEPROMAddr = (I2C_Params->ControlAddr + I2C_Params->WritePtrAllData) % dEEPROM_AddrLength;
        I2C_putData(I2C_Params->base, u16EEPROMAddr >> 8);
        I2C_putData(I2C_Params->base, u16EEPROMAddr & 0xff);
        I2C_setConfig(I2C_Params->base, (I2C_CONTROLLER_SEND_MODE|I2C_REPEAT_MODE));
        I2C_sendStartCondition(I2C_Params->base);
        I2C_Params->state = I2C_WaitWriteEeprom2;
        I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        break;
    case I2C_WaitWriteEeprom2:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        if(I2C_getStatus(I2C_Params->base) & I2C_STS_REG_ACCESS_RDY)
        {
            I2C_clearStatus(I2C_Params->base, I2C_STS_REG_ACCESS_RDY);
            status = handleNACK(I2C_Params->base);
            if(status)
            {
                I2C_sendStartCondition(I2C_Params->base);
            }
            else
            {
                I2C_sendStopCondition(I2C_Params->base);
                I2C_Params->state = I2C_WriteCheck;
                I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
            }
        }
        break;
    case I2C_WriteCheck:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        status = checkBusStatus(I2C_Params->base);
        if(status)
        {
            return;
        }
        I2C_setConfig(I2C_Params->base, I2C_CONTROLLER_RECEIVE_MODE|I2C_REPEAT_MODE);
        I2C_sendStartCondition(I2CA_BASE);
        I2C_Params->WritePtr = 0;
        I2C_Params->state = I2C_WriteCheckRead;
        I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        break;
    case I2C_WriteCheckRead:
//        I2C_Params->WriteCount ++;
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;

        }
        while(I2C_getRxFIFOStatus(I2C_Params->base))
        {
            Uint16 AAA = I2C_getRxFIFOStatus(I2C_Params->base);
            temp = __byte((int *)I2C_Params->pTX_MsgBuffer,I2C_Params->WritePtrAllData);
            temp_1 = I2C_getData(I2C_Params->base);
            if( temp != temp_1 )
            {
//                ESTOP0;
                I2C_Params->state = I2C_Error;
                ESTOP0;
            }
            I2C_Params->WritePtr++;
            I2C_Params->WritePtrAllData++;
            if(I2C_Params->WritePtr == I2C_Params->NumOfDataBytes)
            {
                I2C_sendStopCondition(I2CA_BASE);
                I2C_Params->state = I2C_WritePrepare;
                break;
            }
        }
        break;
    case I2C_Error:

        NOP;
        NOP;
        NOP;
        NOP;
        NOP;
        break;
    }
}

void I2C_readData(struct I2CHandle *I2C_Params)
{
    uint16_t status;
    uint16_t temp_PageByte;
    uint16_t u16EEPROMAddr;
    switch(I2C_Params->state)
    {
    case I2C_ReadPrepare:
        DataFlash.EEPROM.NumOfDataBytes = DataFlash.EEPROM.NumOfAllDataBytes - DataFlash.EEPROM.WritePtrAllData;
        if( DataFlash.EEPROM.NumOfDataBytes > dEEPROM_PageDataBytesLength )
        {
            DataFlash.EEPROM.NumOfDataBytes = dEEPROM_PageDataBytesLength;
        }
        temp_PageByte = (I2C_Params->ControlAddr+I2C_Params->WritePtrAllData)%dEEPROM_PageDataBytesLength;
        if( temp_PageByte && DataFlash.EEPROM.NumOfDataBytes > (dEEPROM_PageDataBytesLength - temp_PageByte) )              //M24C08一个Page只有dEEPROM_PageDataBytesLength个Byte
        {
            DataFlash.EEPROM.NumOfDataBytes = dEEPROM_PageDataBytesLength - temp_PageByte;
        }
        if( DataFlash.EEPROM.NumOfDataBytes == 0 )
        {
            I2C_Params->state = I2C_Wait;
        }
        else
        {
            I2C_Params->state = I2C_ReadSetAddress;
            I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        }
        break;
    case I2C_ReadSetAddress:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
//            ESTOP0;
        }
        status = checkBusStatus(I2C_Params->base);
        if(status)
        {
            return;
        }
        I2C_disableFIFO(I2C_Params->base);
        I2C_enableFIFO(I2C_Params->base);
        I2C_setTargetAddress(I2C_Params->base, I2C_Params->TargetAddr + (I2C_Params->ControlAddr+I2C_Params->WritePtrAllData)/dEEPROM_AddrLength);
        u16EEPROMAddr = (I2C_Params->ControlAddr + I2C_Params->WritePtrAllData) % dEEPROM_AddrLength;
        I2C_putData(I2C_Params->base, u16EEPROMAddr >> 8);
        I2C_putData(I2C_Params->base, u16EEPROMAddr & 0xff);
        I2C_setConfig(I2C_Params->base, (I2C_CONTROLLER_SEND_MODE|I2C_REPEAT_MODE));
        I2C_sendStartCondition(I2C_Params->base);
        I2C_Params->state = I2C_WaitReadSetAddress;
        I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        break;
    case I2C_WaitReadSetAddress:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
//            ESTOP0;
        }
        if(I2C_getStatus(I2C_Params->base) & I2C_STS_REG_ACCESS_RDY)
        {
            I2C_clearStatus(I2C_Params->base, I2C_STS_REG_ACCESS_RDY);
            status = handleNACK(I2C_Params->base);
            if(status)
            {
                I2C_sendStartCondition(I2C_Params->base);
            }
            else
            {
                I2C_sendStopCondition(I2C_Params->base);
                I2C_Params->state = I2C_StartRead;
                I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
            }
        }
        break;
    case I2C_StartRead:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        status = checkBusStatus(I2C_Params->base);
        if(status)
        {
            return;
        }
        I2C_setConfig(I2C_Params->base, I2C_CONTROLLER_RECEIVE_MODE|I2C_REPEAT_MODE);
        I2C_sendStartCondition(I2CA_BASE);
        I2C_Params->WritePtr = 0;
        I2C_Params->state = I2C_ReadData;
        I2C_Params->timer.start(&I2C_Params->timer, 8000U);         //8ms
        break;
    case I2C_ReadData:
        if ( I2C_Params->timer.expiredTimer(&I2C_Params->timer) )
        {
            I2C_Params->state =I2C_Error;
            ESTOP0;
        }
        while(I2C_getRxFIFOStatus(I2C_Params->base))
        {
            __byte((int *)I2C_Params->pRX_MsgBuffer,I2C_Params->WritePtrAllData) = I2C_getData(I2C_Params->base);
            I2C_Params->WritePtr++;
            I2C_Params->WritePtrAllData++;
            if(I2C_Params->WritePtr == I2C_Params->NumOfDataBytes)
            {
                I2C_sendStopCondition(I2CA_BASE);
                I2C_Params->state = I2C_ReadPrepare;

                break;
            }
        }
        break;
    }
}



void I2C_loopStates()
{
    switch(DataFlash.EEPROM.E2state)
    {
    case EEPROM_Wait:
        DataFlash.EEPROM.ControlAddr    = 0;
        DataFlash.EEPROM.NumOfAllDataBytes = 2;
        DataFlash.EEPROM.WritePtr = 0;
        DataFlash.EEPROM.WritePtrAllData = 0;
        DataFlash.EEPROM.state = I2C_WritePrepare;
        DataFlash.EEPROM.E2state = EEPROM_WaitClear1AA1;
        break;
    case EEPROM_WaitClear1AA1:
        if( DataFlash.EEPROM.state == I2C_Wait )
        {
            DataFlash.EEPROM.ControlAddr    = (1 - DataFlash.flashCount) * 512;
            DataFlash.EEPROM.NumOfAllDataBytes = 256*2;
            DataFlash.EEPROM.WritePtr = 0;
            DataFlash.EEPROM.WritePtrAllData = 0;
            DataFlash.EEPROM.state = I2C_WritePrepare;
            DataFlash.EEPROM.E2state = EEPROM_WaitWriteData;
        }
        break;
    case EEPROM_WaitWriteData:
        if( DataFlash.EEPROM.state == I2C_Wait )
        {
            if( DataFlash.flashCount )
            {
                DataFlash.flashCount = 0;
            }
            else
            {
                DataFlash.flashCount = 1;
            }
            DataFlash.EEPROM.E2state = EEPROM_Wait;
            DataFlash.state = DF_Wait;
        }
        break;
    }
    I2C_writeData(&DataFlash.EEPROM);
    I2C_readData(&DataFlash.EEPROM);
}

#endif
