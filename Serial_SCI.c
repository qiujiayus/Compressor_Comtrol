/*
 * Serial_SCI.c
 *
 *  Created on: 2024年1月22日
 *      Author: 79155
 */
#include "f28002x_device.h"         // f28002x Headerfile Include File
#include "f28002x.h"       // f28002x Include File
#include "stdio.h"

#if Serial_CRC_Method == 1
#include "Serial_CRC16.h"
#endif

void* testInput;

__attribute__((blocked))
struct Serial_SCI_Struct Serial_SCI;

volatile Uint16 Serial_Time_Count = 0;
union SerialCommand_Union Serial_SyncCommand;

struct VOFA_Frame_STRUCT VOFA_Frame;

void Serial_TxData_Prepare()
{
    static Uint16 TxCnt = 1;
    Serial_SCI.TestCnt += 0.001f;
    Serial_SyncCommand.CmdType = TxCnt;
    switch(TxCnt)
    {
    case 1:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar1;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar2;
        TxCnt ++;
        break;
    case 2:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar3;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar4;
        TxCnt ++;
        break;
    case 3:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar5;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar6;
        TxCnt ++;
        break;
    case 4:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar7;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar8;
        TxCnt ++;
        break;
    case 5:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar9;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar10;
        TxCnt ++;
        break;
    case 6:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar11;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar12;
        TxCnt ++;
        break;
    case 7:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar13;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar14;
        TxCnt ++;
        break;
    case 8:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar15;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar16;
        TxCnt ++;
        break;
    case 9:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar17;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar18;
        TxCnt ++;
        break;
    case 10:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar19;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar20;
        TxCnt ++;
        break;
    case 11:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar21;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar22;
        TxCnt ++;
        break;
    case 12:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar23;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar24;
        TxCnt ++;
        break;
    case 13:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar25;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar26;
        TxCnt ++;
        break;
    case 14:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar27;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar28;
        TxCnt ++;
        break;
    case 15:
        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar29;
        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar30;
        TxCnt = 1;
        break;
//    case 16:
//        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar31;
//        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar32;
//        TxCnt ++;
//        break;
//    case 17:
//        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar33;
//        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar34;
//        TxCnt ++;
//        break;
//    case 18:
//        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar35;
//        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar36;
//        TxCnt ++;
//        break;
//    case 19:
//        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar37;
//        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar38;
//        TxCnt ++;
//        break;
//    case 20:
//        Serial_SCI.TxData.fVar1.all = dSerial_ViewVar39;
//        Serial_SCI.TxData.fVar2.all = dSerial_ViewVar40;
//        TxCnt = 1;
//        break;
    }
}
void Serial_RxData_Process()
{
///////////////这一部分不修改////////////////////////////
    Serial_SyncCommand.all = Serial_SCI.RxData.Var1.all;

    //VOFA模式下发送 1A A1 00 00 00 00 00 00 1B 00 退出VOFA
    if(dVOFA_Enable)
    {
        SCI_setBaud(SCI_BASE, DEVICE_LSPCLK_FREQ, DEVICE_LSPCLK_FREQ / 16);
    }
    else
    {
        SCI_setBaud(SCI_BASE, DEVICE_LSPCLK_FREQ, SCI_BAUDRATE);
    }

    switch(Serial_SyncCommand.CmdType)
    {
    case 90://固定90为开关机指令
        SYSFlag.bit.PowerOn_Command = (Uint16)Serial_SCI.RxData.fVar.all;
        break;
    case 101: //改变DSP波特率（波特率上限为DEVICE_LSPCLK_FREQ/16），且实际波特率与所设波特率有一定误差
        SCI_setBaud(SCI_BASE, DEVICE_LSPCLK_FREQ, (uint32_t)Serial_SCI.RxData.fVar.all);
        break;
    case 102://改变变量发送速率
        Serial_SCI.SendTime = (Uint16)Serial_SCI.RxData.fVar.all;
        break;
    }
    Serial_ViewVarRx();
///////////////////////////////////////////////////////
}

void Serial_SCI_Init()
{
#if dSysCfgEnable == 0
    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_SCIA);
    GPIO_setPinConfig(GPIO_28_SCIA_RX);
    GPIO_setPadConfig(28, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(28, GPIO_QUAL_ASYNC);
    GPIO_setPinConfig(GPIO_29_SCIA_TX);
    GPIO_setPadConfig(29, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(29, GPIO_QUAL_ASYNC);

    SCI_clearInterruptStatus(SCI_BASE, SCI_INT_RXFF | SCI_INT_TXFF | SCI_INT_FE | SCI_INT_OE | SCI_INT_PE | SCI_INT_RXERR | SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
    SCI_clearOverflowStatus(SCI_BASE);
    SCI_resetTxFIFO(SCI_BASE);
    SCI_resetRxFIFO(SCI_BASE);
    SCI_resetChannels(SCI_BASE);
    SCI_setConfig(SCI_BASE, DEVICE_LSPCLK_FREQ, SCI_BAUDRATE, (SCI_WordLength|SCI_StopBits|SCI_Parity));
    SCI_disableLoopback(SCI_BASE);
    SCI_performSoftwareReset(SCI_BASE);
    SCI_enableFIFO(SCI_BASE);
    SCI_enableModule(SCI_BASE);
#endif
    memset((void *)&Serial_SCI,0,sizeof(Serial_SCI));
    QUEUE_reset(&Serial_SCI.rxFIFO);
    QUEUE_reset(&Serial_SCI.txFIFO);
    Serial_SCI.SendTime = Serial_SendTime_Init;
    VOFA_Frame.tail[0] = 0x0000;
    VOFA_Frame.tail[1] = 0x0000;
    VOFA_Frame.tail[2] = 0x0080;
    VOFA_Frame.tail[3] = 0x007F;
}


void Serial_SCI_TX()
{
    Serial_TxData_Prepare();

    Uint16 TxData[16];
    Uint16 i;
    TxData[0] = Serial_SyncCommand.all>>8 & 0xff;
    TxData[1] = Serial_SyncCommand.all & 0xff;
    TxData[2] = Serial_SCI.TxData.fVar1.u32>>24 & 0xff;
    TxData[3] = Serial_SCI.TxData.fVar1.u32>>16 & 0xff;
    TxData[4] = Serial_SCI.TxData.fVar1.u32>>8 & 0xff;
    TxData[5] = Serial_SCI.TxData.fVar1.u32 & 0xff;
    TxData[6] = Serial_SCI.TxData.fVar2.u32>>24 & 0xff;
    TxData[7] = Serial_SCI.TxData.fVar2.u32>>16 & 0xff;
    TxData[8] = Serial_SCI.TxData.fVar2.u32>>8 & 0xff;
    TxData[9] = Serial_SCI.TxData.fVar2.u32 & 0xff;
    Uint16 TxCrc = Serial_ComputeCRC16_Modbus(TxData, 12, 0);
    TxData[12] = TxCrc>>8 & 0xff;
    TxData[13] = TxCrc & 0xff;

    if(SERIEL_FIFO_SIZE - QUEUE_count(&Serial_SCI.txFIFO) > SCI_TXData_Words*2)
    {
        QUEUE_write(&Serial_SCI.txFIFO, SCI_TxHead>>8);
        QUEUE_write(&Serial_SCI.txFIFO, SCI_TxHead);
        for(i=0; i<14; i++)
        {
            QUEUE_write(&Serial_SCI.txFIFO, TxData[i]);
        }
    }
}

//四通道整个函数开销约100时钟周期
void Serial_VOFA_TX_Prepare()
{
    if( dVOFA_Enable == 0 )
    {
        return;
    }

    Uint16 *p_txBuffer;
    if( Serial_SCI.VOFA.addr_wr )
    {
        if( Serial_SCI.VOFA.buffer2_ready == 0 )
        {
            Serial_SCI.VOFA.addr_wr = 0;
            p_txBuffer = Serial_SCI.VOFA.buffer2;
        }
        else
        {
            return;
        }
    }
    else
    {
        if( Serial_SCI.VOFA.buffer1_ready == 0 )
        {
            Serial_SCI.VOFA.addr_wr = 1;
            p_txBuffer = Serial_SCI.VOFA.buffer1;
        }
        else
        {
            return;
        }
    }

    Uint16 i;
    Uint16 addr = 0;
    for( i = 0 ; i < SCI_VOFA_ChanelNum ; i++ )
    {
        //VOFA JustFloat 为小端模式
        p_txBuffer[addr] = __byte((int*) &VOFA_Frame.fdata[i].u32, 0);
        addr++;
        p_txBuffer[addr] = __byte((int*) &VOFA_Frame.fdata[i].u32, 1);
        addr++;
        p_txBuffer[addr] = __byte((int*) &VOFA_Frame.fdata[i].u32, 2);
        addr++;
        p_txBuffer[addr] = __byte((int*) &VOFA_Frame.fdata[i].u32, 3);
        addr++;
    }
//    for( i = 0 ; i < 4 ; i++ )
//    {
//        p_txBuffer[addr] = VOFA_Frame.tail[i];
//        addr++;
//    }
    p_txBuffer[addr++] = 0x00;
    p_txBuffer[addr++] = 0x00;
    p_txBuffer[addr++] = 0x80;
    p_txBuffer[addr++] = 0x7F;
    p_txBuffer[addr] = 1;               //buffer_ready
}
void Serial_FiFO_TX()
{
    Uint16 i;
    if( SCI_getTxFIFOStatus(SCI_BASE) < 9 )
    {
        if( dVOFA_Enable )
        {
            if(Serial_SCI.VOFA.addr_rd)
            {
                if( Serial_SCI.VOFA.buffer2_ready )
                {
                    for(i = 0; i < 8; i++)
                    {
                        if( Serial_SCI.VOFA.buffer_data_rd < 20 )
                        {
                            SCI_writeCharNonBlocking(SCI_BASE,Serial_SCI.VOFA.buffer2[Serial_SCI.VOFA.buffer_data_rd]);
                            Serial_SCI.VOFA.buffer_data_rd++;
                        }
                        else
                        {
                            Serial_SCI.VOFA.buffer_data_rd = 0;
                            Serial_SCI.VOFA.buffer2_ready = 0;
                            Serial_SCI.VOFA.addr_rd = 0;
                            return;
                        }
                    }
                }
            }
            else
            {
                if( Serial_SCI.VOFA.buffer1_ready )
                {
                    for(i = 0; i < 8; i++)
                    {
                        if( Serial_SCI.VOFA.buffer_data_rd < 20 )
                        {
                            SCI_writeCharNonBlocking(SCI_BASE,Serial_SCI.VOFA.buffer1[Serial_SCI.VOFA.buffer_data_rd]);
                            Serial_SCI.VOFA.buffer_data_rd++;
                        }
                        else
                        {
                            Serial_SCI.VOFA.buffer_data_rd = 0;
                            Serial_SCI.VOFA.buffer1_ready = 0;
                            Serial_SCI.VOFA.addr_rd = 1;
                            return;
                        }
                    }
                }
            }
        }
        else
        {
            for(i = 0; i < 8; i++)
            {
                if( !QUEUE_isEmpty(&Serial_SCI.txFIFO) )
                {
                    SCI_writeCharNonBlocking(SCI_BASE,Serial_SCI.txFIFO.fifo[Serial_SCI.txFIFO.addr_rd]);
                    Serial_SCI.txFIFO.addr_rd = (Serial_SCI.txFIFO.addr_rd + 1) % Serial_SCI.txFIFO.length;
                }
                else
                    return;
            }
        }
    }
}

void Serial_SCI_RX()
{
    Uint16 i;
    static Uint16 Serial_RxNum;
    static Uint16 Serial_Array[SCI_RXData_Words];
    Uint16 Serial_ByteArray[SCI_RXData_Words*2];
    Serial_RxNum = SCI_getRxFIFOStatus(SCI_BASE);
    if( (SCI_getRxStatus(SCI_BASE)&(SCI_RXSTATUS_PARITY|SCI_RXSTATUS_OVERRUN|SCI_RXSTATUS_FRAMING|SCI_RXSTATUS_BREAK|SCI_RXSTATUS_ERROR)) )
    {
        SCI_performSoftwareReset(SCI_BASE);
    }
    while(Serial_RxNum >= 1)
    {
        QUEUE_write( &Serial_SCI.rxFIFO, SCI_readCharNonBlocking(SCI_BASE) );
        Serial_RxNum --;
    }
    while( QUEUE_count(&Serial_SCI.rxFIFO) >= 2 )
    {
        Serial_SCI.RxData.Head.all = Serial_rxFIFORead(0)<<HardWare_Frame_Bits | Serial_rxFIFORead(1)&0x00FF;
        if( Serial_SCI.RxData.Head.byte.HByte != 0x1A || Serial_SCI.RxData.Head.byte.LByte != 0xA1 )
        {
            Serial_SCI.rxFIFO.addr_rd = (Serial_SCI.rxFIFO.addr_rd + 1) % Serial_SCI.rxFIFO.length;
            Serial_SCI.HeadErr_Count ++;
            if(Serial_SCI.HeadErr_Count > 4 && dVOFA_Enable)
            {
                dVOFA_Enable = 0;
                SCI_setBaud(SCI_BASE, DEVICE_LSPCLK_FREQ, SCI_BAUDRATE);
                Serial_SCI.HeadErr_Count = 0;
            }
        }
        else if( QUEUE_count(&Serial_SCI.rxFIFO) >= SCI_RXData_Words*2 )
        {
            QUEUE_memcpy(&Serial_SCI.rxFIFO, Serial_ByteArray, SCI_RXData_Words*2);
            for(i = 0; i < SCI_RXData_Words; i++)
            {
                Serial_Array[i] = Serial_rxFIFORead(i*2)<<HardWare_Frame_Bits | Serial_rxFIFORead(i*2+1)&0x00FF;
            }
#if Serial_CRC_Method == 1 || Serial_CRC_Method == 2 || Serial_CRC_Method == 3
            static Uint16 Serial_CRC;
            Serial_CRC = Serial_ComputeCRC16_Modbus(&Serial_ByteArray[2], SCI_RXData_Words*2 - 4, 0);
            if( Serial_CRC == Serial_Array[SCI_RXData_Words - 1] )
            {
                memcpy(&Serial_SCI.RxData, Serial_Array, SCI_RXData_Words);
                Serial_SCI.rxFIFO.addr_rd = (Serial_SCI.rxFIFO.addr_rd + SCI_RXData_Words*2) % Serial_SCI.rxFIFO.length;
                Serial_RxData_Process();
                break;
            }
            else
            {
                Serial_SCI.rxFIFO.addr_rd = (Serial_SCI.rxFIFO.addr_rd + 2) % Serial_SCI.rxFIFO.length;
            }
#elif Serial_CRC_Method == 0
            memcpy(&Serial_SCI.RxData, Serial_Array, SCI_RXData_Words);
            Serial_SCI.rxFIFO.addr_rd = (Serial_SCI.rxFIFO.addr_rd + SCI_RXData_Words*2) % Serial_SCI.rxFIFO.length;
            Serial_RxData_Process();
            break;
#endif
        }
        else
        {
            break;
        }
    }
}


#if Serial_CRC_Method == 1
Uint16 Serial_ComputeCRC16_Modbus(Uint16 *Data, Uint16 ByteNum, Uint16 OffSet)
{
    Uint16 crc = 0xFFFF;
    Uint16 temp;
    Uint16 i;

    for(i = 0;i < ByteNum;i++)
    {
        temp = crc & 0xFF;
        crc = (Uint16)((crc >> 8) ^ Serial_CRC_MODBUS16[(temp ^ Data[i]) & 0xFF]);
    }

    return crc;
}
#elif Serial_CRC_Method == 2
Uint16 Serial_ComputeCRC16_Modbus(Uint16 *Data, Uint16 ByteNum, Uint16 OffSet)
{
    unsigned int crc_result;
    Uint16 i;

    myVCRC_16_Modbus_Handle->seedValue      = 0xFFFF;
    myVCRC_16_Modbus_Handle->nMsgBytes      = ByteNum;
    myVCRC_16_Modbus_Handle->run            = (void (*)(void *))CRC_run16BitPoly1Reflected;
    myVCRC_16_Modbus_Handle->pMsgBuffer     = Data;
    myVCRC_16_Modbus_Handle->run(myVCRC_16_Modbus_Handle);
    crc_result = (unsigned int)myVCRC_16_Modbus_Handle->crcResult;

    //输出字节内按位反转再高低字节互换
    crc_result = (((crc_result & 0xaaaa) >> 1) | ((crc_result & 0x5555) << 1));
    crc_result = (((crc_result & 0xcccc) >> 2) | ((crc_result & 0x3333) << 2));
    crc_result = (((crc_result & 0xf0f0) >> 4) | ((crc_result & 0x0f0f) << 4));
    crc_result = ((crc_result >> 8) | (crc_result << 8));

    return crc_result;
}
#elif Serial_CRC_Method == 3
Uint16 Serial_ComputeCRC16_Modbus(Uint16 *Data, Uint16 ByteNum, Uint16 OffSet)
{
    unsigned int crc = 0xFFFF;
    int pos = 0;
    int i = 0;

    for (pos = 0; pos < ByteNum; pos++)
    {
        // XOR byte into least sig. byte of crc
        crc ^= (unsigned int) Data[pos];

        // Loop over each bit
        for (i = 8; i != 0; i--)
        {

            // If the LSB is set
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;      // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else
                crc >>= 1;      // Just shift right
        }
    }
    return crc;
}
#endif

