/*
 * DataStorage_Flash_API.h
 *
 *  Created on: 2023年10月16日
 *      Author: wangyz1
 */

#ifndef LIB_FLASH_API_DATASTORAGE_FLASH_API_H_
#define LIB_FLASH_API_DATASTORAGE_FLASH_API_H_
#include "F021_F28002x_C28x.h"


#define StorageType_EEPROM  1
#define StorageType_FLASH   0
#define StorageType StorageType_EEPROM

#define FlashBank0StartAddress      0x80000U
#define Bzero_Sector0_start         0x80000U
#define Bzero_Sector1_start         0x81000U
#define Bzero_Sector2_start         0x82000U
#define Bzero_Sector3_start         0x83000U
#define Bzero_Sector4_start         0x84000U
#define Bzero_Sector5_start         0x85000U
#define Bzero_Sector6_start         0x86000U
#define Bzero_Sector7_start         0x87000U
#define Bzero_Sector8_start         0x88000U
#define Bzero_Sector9_start         0x89000U
#define Bzero_Sector10_start        0x8A000U
#define FlashBank0EndAddress        0x8AFFFU

//Sector length in number of 16bits
#define Sector8KB_u16length   0x1000U

//Sector length in number of 32bits
#define Sector8KB_u32length   0x800U
//
// FMC Registers, Masks and Values
//
#define FLASH_O_ACC_EP           0x148U    // Flash Accumulated Erase Pulses
#define FLASH_O_ERA_PUL          0x136U    // Flash Max Erase Pulses
#define FLASH_O_OPT              0x144U    // Flash Optimization

#define FLASH_ERA_PUL_MAX_ERA_PUL_M      0xFFFU  // Flash Max Erase Pulses Mask
#define OPT_ENABLE                       0x5U    // Flash Optimization Enable
#define OPT_DISABLE                      0x2U    // Flash Optimization Disable
#define MAX_ERASE_PULSE                  0x7D0U  // Flash Max Erase Pulses

#define MAX_BUFFER_SIZE             16


#define I2C_FIFO_LEVEL              16

#define MAX_7_BIT_ADDRESS 127U
#define MAX_10_BIT_ADDRESS 1023U

#define I2C_ERROR_BUS_BUSY              0x1000
#define I2C_ERROR_NACK_RECEIVED         0x2000
#define I2C_ERROR_ARBITRATION_LOST      0x3000
#define I2C_ERROR_STOP_NOT_READY        0x5555
#define I2C_SUCCESS                     0x0000

#define dBlackInfoPerGroupLength        32
#define dBlackInfoGroupNum              16
#define dDataStorage_DataLength         dBlackInfoPerGroupLength * dBlackInfoGroupNum
#define dEEPROM_PageDataBytesLength     32
#define dEEPROM_DataBytes               dDataStorage_DataLength * 2
#define dEEPROM_DataMaxBytes            0x2000U
#define dEEPROM_DataMaxNum              (dEEPROM_DataMaxBytes / (dEEPROM_DataBytes))
#define dEEPROM_AddrLength              0xFFFF
#define dEEPROM_AddrBytes               2

typedef union BLACKBOX_RECORD_INFO_union
{
    Uint16 u16Data[dDataStorage_DataLength];
    struct{
        Uint32 u32TotalTime;
        Uint16 u16AutoTurnONDelayTime;
    };
}BLACKBOX_RECORD_INFO;
extern volatile BLACKBOX_RECORD_INFO unBlackBoxStorageInfo;

struct BlackBox_Info_STRUCT            //
{
    Uint16 DataFlash_EEPROM_ControlAddr[dBlackInfoGroupNum];
    Uint16 FOC_CurRC_Enabled[dBlackInfoGroupNum];
    Uint32 FaultType_all[dBlackInfoGroupNum];
    float Samp_Vars_fCurPMSM_a[dBlackInfoGroupNum];
    float Samp_Vars_fCurPMSM_b[dBlackInfoGroupNum];
    float Samp_Vars_fCurPMSM_c[dBlackInfoGroupNum];
    float Samp_Vars_fBusVolt[dBlackInfoGroupNum];
    float FOC_Motor_Speed_RPM[dBlackInfoGroupNum];
    float FOC_ActivePower[dBlackInfoGroupNum];
    float FOC_FW_Vout_Modulation[dBlackInfoGroupNum];
    float PMSM_SpeedRPM_Set_CAN[dBlackInfoGroupNum];
    float Freq_PMSM_F_SW_Set[dBlackInfoGroupNum];
    float NTC_Temp_s16_NTC_IGBT_Temperature[dBlackInfoGroupNum];
    float StorageIndex[dBlackInfoGroupNum];
};
extern struct BlackBox_Info_STRUCT BlackBox_Info;
// State machine constants
typedef enum {
    DF_Initial,
    DF_Read,
    DF_Read2,
    DF_Wait,
    DF_Storage_DataPrepare,
    DF_Storage_Clearmore,
    DF_Storage_Erase,
    DF_Storage_WaitErase,
    DF_Storage_Program,
    DF_Storage_Verify,
    EEP_DataPrepare,
    EEP_Program,
    EEP_WaitErase,
    EEP_Erase,
    COR_Write
} DataFlashState;

// EERPOM State machine constants
typedef enum {
    EEPROM_Wait,
    EEPROM_WaitReadHead1,           //M24C08的第一组数据区
    EEPROM_WaitReadHead2,           //M24C08的第二组数据区
    EEPROM_WaitReadData,
    EEPROM_WaitClear1AA1,
    EEPROM_WaitWriteData
} EEPROMDataFlashState;

typedef enum {
    I2C_Wait,
    I2C_WritePrepare,
    I2C_StartWrite,
    I2C_WriteData,
    I2C_WriteBusy,
    I2C_WaitWriteEeprom1,
    I2C_WaitWriteEeprom2,
    I2C_WriteCheck,
    I2C_WriteCheckRead,
    I2C_ReadPrepare,
    I2C_ReadSetAddress,
    I2C_WaitReadSetAddress,
    I2C_StartRead,
    I2C_ReadData,
    I2C_Error
} I2CState;

#if StorageType==StorageType_EEPROM
struct I2CHandle
{
    I2CState state;
    EEPROMDataFlashState E2state;
    uint32_t base;
    uint16_t TargetAddr;                  // target address tied to the message.
    uint16_t ControlAddr;
    uint16_t *pTX_MsgBuffer;             // Pointer to TX message buffer
    uint16_t *pRX_MsgBuffer;             // Pointer to RX message buffer
    uint16_t NumOfAllDataBytes;             // Number of valid bytes in message.
    uint16_t NumOfDataBytes;             // Number of valid bytes in message.
    uint16_t WritePtrAllData;            // 写数据指针位置.
    uint16_t WritePtr;                   // 写数据指针位置.
    struct I2CHandle *currentHandlePtr;

    uint16_t numofSixteenByte;
    uint16_t remainingBytes;

    uint16_t WriteCycleTime_in_us;      //  target write cycle time. Depends on target.
                                        //  Please check target device datasheet

    uint16_t NumOfAttempts;             //  Number of attempts to make before reporting
                                        //  target not ready (NACK condition)
    uint16_t Delay_us;                  //  Delay time in microsecs (us)
    Timer timer;                        //  防卡死计时器
    uint16_t WriteCount;
    uint16_t I2C_FIFO_TXFULL_Count;
    uint16_t Storage_EEPROM_Finished_Flag;
    Uint16   Storage_EEPROM_Finished_Count;
    uint16_t Erase_EEPROM_Finished_Flag;
    Uint16   Erase_EEPROM_Finished_Count;
    Uint16 n;
};
#endif

struct DataFlash_STRUCT
{
    Uint16      data[dDataStorage_DataLength];  //地址一定要偶对齐，不然会出错
    Uint16      data2[1];         //地址一定要偶对齐，不然会出错
    DataFlashState state;
    Uint16      Storage_EEPROM;
    Uint16      Storage_programCommand;
    Uint16      Storage_programCommand_Finished_Flag;
    Uint16      Storage_eraseOK;
    Uint32      Storage_programCount;
    Uint32      Storage_flashCount;
    Uint32      flashCount;
    Uint32      programCount;
    Uint16      i;
    Uint32      pu32SectorAddress;              //擦除扇区地址
    Uint32      pu32DataAddress;                //编程数据地址
    Uint16      otaCommand;
    Uint16      otaCommandType;
    Uint16      CorrectionStoreCommand;
    Uint16      CorrectionStoreState;

#if StorageType==StorageType_EEPROM
    struct I2CHandle EEPROM;
#endif
    Fapi_StatusType  oReturnCheck;
    Fapi_FlashStatusType  oFlashStatus;
    Fapi_FlashStatusWordType  oFlashStatusWord;
};

extern struct DataFlash_STRUCT DataFlash;

extern void DataStorage_loopStates();
extern void UpdateBlackBoxStorageInfo();
extern void DataStorage_Init();
extern void FAPI_Storage_CheckDemage(void);
extern void FAPI_Clearmore(void);
extern void FAPI_EraseSector(void);
extern void FAPI_WaitErase(void);
extern void FAPI_Storage_DataPrepare();
extern void FAPI_WaitBankErase(void);
extern void FAPI_Program();
extern void FAPI_Verify();
extern void FAPI_Correction_Write();
extern void Example_Error(Fapi_StatusType status);
extern void FMSTAT_Fail(void);
extern void ECC_Fail(void);

extern void I2C_writeData();
extern void I2C_readData();
extern void I2C_loopStates();
extern void readFlash_StorageData(Uint16 *Addr);
extern void readFlash_Data(Uint16 *Addr,Uint16 *u16Data,uint16 u16Len);
extern uint16_t float_to_uint16(float value, Uint16 *out, uint16_t u16Data_Index);
extern void EEPROM_Storage_DataPrepare();
extern void DataStorage_Init_EEPROM();
extern float uint16_to_float(uint16_t in1, uint16_t in2);
extern float uint16_to_uint32(uint16_t in1, uint16_t in2);
#endif /* LIB_FLASH_API_DATASTORAGE_FLASH_API_H_ */
