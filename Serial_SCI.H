/*
 * Serial_SCI.h
 *
 *  Created on: 2024年1月22日
 *      Author: 79155
 */

//确定使用的SCI模块(A/B)
//使用时将此文件包含于f28003x.h中（CLA除外）
/*
#if !defined(__TMS320C28XX_CLA__)
#include "Lib_SerialDebug/Serial_SCI.h"
#include "stdio.h"  //日志
#include "string.h"
#endif
*/
//在初始化时调用 Serial_SCI_Init(); 在1ms定时器中断调用 Serial_Tx_Process(); 在while循环调用 Serial_SCI_RX()和Serial_FiFO_TX();

#ifndef LIB_SERIALDEBUG_SERIAL_SCI_H_
#define LIB_SERIALDEBUG_SERIAL_SCI_H_

#include "f28002x_device.h"         // f28002x Headerfile Include File
#include "f28002x.h"       // f28002x Include File

#define dSysCfgEnable            1  //是否使用工具SysCfg配置SCI模块
#if dSysCfgEnable == 1
#define SCI_BASE                 mySCI_debug_BASE
#define SCI_BAUDRATE             mySCI_debug_BAUDRATE
#define SCI_WordLength           mySCI_debug_CONFIG_WLEN
#define SCI_StopBits             mySCI_debug_CONFIG_STOP
#define SCI_Parity               mySCI_debug_CONFIG_PAR
#elif dSysCfgEnable == 0
#define SCI_BASE                 SCIA_BASE
#define SCI_BAUDRATE             115200
#define SCI_WordLength           SCI_CONFIG_WLEN_8
#define SCI_StopBits             SCI_CONFIG_STOP_ONE
#define SCI_Parity               SCI_CONFIG_PAR_NONE
#endif
#define SCI_TxHead               0x1AA1
#define HardWare_FIFO_Level      16
#define HardWare_Frame_Bits      8
#define Serial_SendTime_Init     5  //ms
#define SCI_TXData_Words         8   //word,需要同时修改Serial_SCI_TxData_Struct结构体
#define SCI_RXData_Words         5   //word
#define Serial_CRC_Method        1  // 0:不做校验   1:查表法   2:调用硬件VCRC计算    3:直接计算 (所有方式均基于CRC16_ModBus(参考https://www.lddgo.net/encrypt/crc))

extern volatile Uint16 Serial_Time_Count;

#define Serial_OTA_Enable               Serial_SyncCommand.Serial_BIT3
#define Serial_OTA_SlaveEnable          Serial_SyncCommand.Serial_BIT4

#define dVOFA_Enable                    Serial_SyncCommand.Serial_BIT1
#define SCI_VOFA_ChanelNum       4  //VOFA波形通道数，最多支持4通道
union FLOAT2_Union
{
    float all;
    Uint32 u32;
};
struct VOFA_Frame_STRUCT
{
    union FLOAT2_Union fdata[SCI_VOFA_ChanelNum];
    Uint16 tail[4];
};
extern struct VOFA_Frame_STRUCT VOFA_Frame;
extern void Serial_VOFA_TX_Prepare();
extern void Serial_FiFO_TX();

#define dSerial_ViewVar1    MainState
#define dSerial_ViewVar2    PMSM_SpeedRPM_Set
#define dSerial_ViewVar3    PMSMState
#define dSerial_ViewVar4    Samp_Vars.fBusVolt
#define dSerial_ViewVar5    Samp_Vars.fCurPMSM.a
#define dSerial_ViewVar6    Samp_Vars.fCurPMSM.b
#define dSerial_ViewVar7    Samp_Vars.fCurPMSM.c
#define dSerial_ViewVar8    FOC.Motor.Speed_RPM
#define dSerial_ViewVar9    FOC.ActivePower
#define dSerial_ViewVar10    FOC.FW.Vout_Mag
#define dSerial_ViewVar11    FOC.FW.Vout_MagMax
#define dSerial_ViewVar12    FOC.FW.Vout_Modulation
#define dSerial_ViewVar13    FOC.FW.IdRefInte
#define dSerial_ViewVar14    FOC.FW.IdRefCalc
#define dSerial_ViewVar15    FOC.Id_PID.Ref
#define dSerial_ViewVar16    FOC.Iq_PID.Ref
#define dSerial_ViewVar17    FaultType.all
#define dSerial_ViewVar18    FaultType.all
#define dSerial_ViewVar19    SpeedRef.ValueRef
#define dSerial_ViewVar20    PMSM_SpeedRPM_Set_CAN
#define dSerial_ViewVar21    SYSFlag.bit.CAN_Connected
#define dSerial_ViewVar22    SYSFlag.bit.CAN_DisConnected_Count
#define dSerial_ViewVar23    ComCan.Can_DisConnect_Count
#define dSerial_ViewVar24    ComCan.Can_Status
#define dSerial_ViewVar25    SYSFlag.bit.CAN_AutoBusOn_Flag
#define dSerial_ViewVar26    SYSFlag.bit.CAN_AutoBusOn_Flag_Locked
#define dSerial_ViewVar27    Freq_PMSM.F_SW_Set
#define dSerial_ViewVar28    FOC.CurRC_Enabled
#define dSerial_ViewVar29    DebugVars.RC_Gain_Test
#define dSerial_ViewVar30    DebugVars.BandWidth_Coff_Test
#define dSerial_ViewVar31    Serial_SCI.TestCnt
#define dSerial_ViewVar32    Serial_SCI.TestCnt
#define dSerial_ViewVar33    Serial_SCI.TestCnt
#define dSerial_ViewVar34    Serial_SCI.TestCnt
#define dSerial_ViewVar35    Serial_SCI.TestCnt
#define dSerial_ViewVar36    Serial_SCI.TestCnt
#define dSerial_ViewVar37    Serial_SCI.TestCnt
#define dSerial_ViewVar38    Serial_SCI.TestCnt
#define dSerial_ViewVar39    Serial_SCI.TestCnt
#define dSerial_ViewVar40    Serial_SCI.TestCnt


/* FIFO数据的类型，可以是结构体类型 */
#define QUEUE_DataType Uint16

/* FIFO长度，实际存放的数据=SERIEL_FIFO_SIZE-1 */
#define SERIEL_FIFO_SIZE    100

union WORD_Union
{
    Uint16 all;
    struct
    {
        Uint16 LByte:8;
        Uint16 HByte:8;
    }byte;
};
union FLOAT_Union
{
    float all;
    struct
    {
        union WORD_Union LWord;
        union WORD_Union HWord;
    }word;
};
union SerialCommand_Union
{
    Uint16 all;
    struct
    {
        Uint16 CmdType:8;
        Uint16 Serial_BIT1:1;
        Uint16 Serial_BIT2:1;
        Uint16 Serial_BIT3:1;
        Uint16 Serial_BIT4:1;
        Uint16 Serial_BIT5:1;
        Uint16 Serial_BIT6:1;
        Uint16 Serial_BIT7:1;
        Uint16 Serial_BIT8:1;
    };
};

extern union SerialCommand_Union Serial_SyncCommand;
struct Serial_SCI_TxData_Struct
{
    union FLOAT2_Union fVar1;
    union FLOAT2_Union fVar2;
};
struct Serial_SCI_RxData_Struct
{
    union WORD_Union Head;
    union WORD_Union Var1;
    union FLOAT_Union fVar;
    union WORD_Union CRC;
};

typedef enum {
    queue_Normal,
    queue_FULL,
    queue_EMPTY,
    queue_ERROR
}queue_status_enum;

typedef struct {
    Uint16 addr_wr;        /* 写地址 */
    Uint16 addr_rd;        /* 读地址 */
    Uint16 length;         /* FIFO长度，实际存放的数据=length-1 */
    QUEUE_DataType fifo[SERIEL_FIFO_SIZE];  //虽然是16位，但数据的有效位数取决于HardWare_Frame_Bits
}queue_struct;

typedef struct {
    Uint16 buffer1[SCI_VOFA_ChanelNum*4 + 4];
    Uint16 buffer1_ready;   /* Buffer1数据是否已准备好 */
    Uint16 buffer2[SCI_VOFA_ChanelNum*4 + 4];
    Uint16 buffer2_ready;   /* Buffer1数据是否已准备好 */
    Uint16 data_accessing;  /* SCI正在访问数据 */
    Uint16 addr_wr;         /* 写地址 */
    Uint16 addr_rd;         /* 读地址 */
    Uint16 buffer_data_rd;  /* SCI发送时buffer内部读取Index */
}VOFA_struct;

struct Serial_SCI_Struct
{
    struct Serial_SCI_TxData_Struct TxData;
    struct Serial_SCI_RxData_Struct RxData;
    Uint16  getNewRxDataFlag;
    queue_struct rxFIFO;
    queue_struct txFIFO;
    VOFA_struct VOFA;
    Uint16 SendTime;
    float TestCnt;
    Uint16 HeadErr_Count;
};
extern struct Serial_SCI_Struct Serial_SCI;

/* FIFO是否为空 */
inline Uint16 QUEUE_isEmpty(queue_struct *q)
{
    return (q->addr_wr == q->addr_rd);
}

/* FIFO是否为满 */
inline Uint16 QUEUE_isFull(queue_struct *q)
{
    return ((q->addr_wr + 1) % q->length == q->addr_rd);
}

/* FIFO内数据的个数 */
inline Uint16 QUEUE_count(queue_struct *q)
{
    if(q->addr_rd <= q->addr_wr)
        return (q->addr_wr - q->addr_rd);
    //addr_rd > addr_wr;
    return (q->length + q->addr_wr - q->addr_rd);
}

/* FIFO复位 */
inline queue_status_enum QUEUE_reset(queue_struct *q)
{
//    int i = 0;

    q->addr_wr = 0;
    q->addr_rd = 0;
    q->length = SERIEL_FIFO_SIZE;
//    for(i = 0; i < q->length; i++)
//        q->fifo[i] = 0;

    return queue_Normal;
}

/* FIFO写入数据 */
inline queue_status_enum QUEUE_write(queue_struct *q, QUEUE_DataType data)
{
    if(QUEUE_isFull(q))
    {
        return queue_FULL;
    }

    q->fifo[q->addr_wr] = data;
    q->addr_wr = (q->addr_wr + 1) % q->length;

    return queue_Normal;
}

/* FIFO读出数据 */
inline queue_status_enum QUEUE_read(queue_struct *q, QUEUE_DataType *pdata)
{
    if(QUEUE_isEmpty(q))
    {
        return queue_EMPTY;
    }

    *pdata = q->fifo[q->addr_rd];
    q->addr_rd = (q->addr_rd + 1) % q->length;

    return queue_Normal;
}

inline queue_status_enum QUEUE_memcpy(queue_struct *q, QUEUE_DataType *pdata, Uint16 num)
{
    if(QUEUE_count(q) < num)
    {
        return queue_ERROR;
    }
    if( q->addr_rd + num > q->length )
    {
        Uint16 num2 = q->length - q->addr_rd;
        memcpy(pdata, &q->fifo[q->addr_rd], num2);
        memcpy(pdata + num2, &q->fifo[0], num - num2);
    }
    else
    {
        memcpy(pdata, &q->fifo[q->addr_rd], num);
    }
    return queue_Normal;
}

extern void Serial_SCI_Init();
extern void Serial_SCI_TX();
extern void Serial_SCI_RX();
extern void Serial_RxData_Process();
extern Uint16 Serial_ComputeCRC16_Modbus(Uint16 *Data, Uint16 ByteNum, Uint16 OffSet);

//串口定时发送函数，放在1ms定时中断内
inline void Serial_Tx_Process()
{
    if(dVOFA_Enable)
    {
        return;
    }
    if( Serial_Time_Count ++ >= Serial_SCI.SendTime )
    {
        Serial_Time_Count = 0;
        Serial_SCI_TX();
    }
}

inline void Serial_ViewVarRx()
{
#pragma diag_suppress 190 //本段代码忽略#190警告
    switch(Serial_SyncCommand.CmdType)
    {
    case 41:
        dSerial_ViewVar1 = Serial_SCI.RxData.fVar.all;
        break;
    case 42:
        dSerial_ViewVar2 = Serial_SCI.RxData.fVar.all;
        break;
    case 43:
        dSerial_ViewVar3 = Serial_SCI.RxData.fVar.all;
        break;
    case 44:
        dSerial_ViewVar4 = Serial_SCI.RxData.fVar.all;
        break;
    case 45:
        dSerial_ViewVar5 = Serial_SCI.RxData.fVar.all;
        break;
    case 46:
        dSerial_ViewVar6 = Serial_SCI.RxData.fVar.all;
        break;
    case 47:
        dSerial_ViewVar7 = Serial_SCI.RxData.fVar.all;
        break;
    case 48:
        dSerial_ViewVar8 = Serial_SCI.RxData.fVar.all;
        break;
    case 49:
        dSerial_ViewVar9 = Serial_SCI.RxData.fVar.all;
        break;
    case 50:
        dSerial_ViewVar10 = Serial_SCI.RxData.fVar.all;
        break;
    case 51:
        dSerial_ViewVar11 = Serial_SCI.RxData.fVar.all;
        break;
    case 52:
        dSerial_ViewVar12 = Serial_SCI.RxData.fVar.all;
        break;
    case 53:
        dSerial_ViewVar13 = Serial_SCI.RxData.fVar.all;
        break;
    case 54:
        dSerial_ViewVar14 = Serial_SCI.RxData.fVar.all;
        break;
    case 55:
        dSerial_ViewVar15 = Serial_SCI.RxData.fVar.all;
        break;
    case 56:
        dSerial_ViewVar16 = Serial_SCI.RxData.fVar.all;
        break;
    case 57:
        dSerial_ViewVar17 = Serial_SCI.RxData.fVar.all;
        break;
    case 58:
        dSerial_ViewVar18 = Serial_SCI.RxData.fVar.all;
        break;
    case 59:
        dSerial_ViewVar19 = Serial_SCI.RxData.fVar.all;
        break;
    case 60:
        dSerial_ViewVar20 = Serial_SCI.RxData.fVar.all;
        break;
    case 61:
        dSerial_ViewVar21 = Serial_SCI.RxData.fVar.all;
        break;
    case 62:
        dSerial_ViewVar22 = Serial_SCI.RxData.fVar.all;
        break;
    case 63:
        dSerial_ViewVar23 = Serial_SCI.RxData.fVar.all;
        break;
    case 64:
        dSerial_ViewVar24 = Serial_SCI.RxData.fVar.all;
        break;
    case 65:
        dSerial_ViewVar25 = Serial_SCI.RxData.fVar.all;
        break;
    case 66:
        dSerial_ViewVar26 = Serial_SCI.RxData.fVar.all;
        break;
    case 67:
        dSerial_ViewVar27 = Serial_SCI.RxData.fVar.all;
        break;
    case 68:
        dSerial_ViewVar28 = Serial_SCI.RxData.fVar.all;
        break;
    case 69:
        dSerial_ViewVar29 = Serial_SCI.RxData.fVar.all;
        break;
    case 70:
        dSerial_ViewVar30 = Serial_SCI.RxData.fVar.all;
        break;
    case 71:
        dSerial_ViewVar31 = Serial_SCI.RxData.fVar.all;
        break;
    case 72:
        dSerial_ViewVar32 = Serial_SCI.RxData.fVar.all;
        break;
    case 73:
        dSerial_ViewVar33 = Serial_SCI.RxData.fVar.all;
        break;
    case 74:
        dSerial_ViewVar34 = Serial_SCI.RxData.fVar.all;
        break;
    case 75:
        dSerial_ViewVar35 = Serial_SCI.RxData.fVar.all;
        break;
    case 76:
        dSerial_ViewVar36 = Serial_SCI.RxData.fVar.all;
        break;
    case 77:
        dSerial_ViewVar37 = Serial_SCI.RxData.fVar.all;
        break;
    case 78:
        dSerial_ViewVar38 = Serial_SCI.RxData.fVar.all;
        break;
    case 79:
        dSerial_ViewVar39 = Serial_SCI.RxData.fVar.all;
        break;
    case 80:
        dSerial_ViewVar40 = Serial_SCI.RxData.fVar.all;
        break;
    }
#pragma diag_default 190
}

inline Uint16 Serial_rxFIFORead(Uint16 index)
{
    return Serial_SCI.rxFIFO.fifo[(Serial_SCI.rxFIFO.addr_rd + index) % Serial_SCI.rxFIFO.length];
}
#endif /* LIB_SERIALDEBUG_SERIAL_SCI_H_ */
