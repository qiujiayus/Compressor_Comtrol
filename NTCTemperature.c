/*********************************************************************
* 内容摘要： LLC温度采集功能文件
* 其它说明：
* 当前版本： V0.0.5
* 作   者： qiujiayu -- 拓邦微电研发中心
* 完成日期： 2025-3-18
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
**********************************************************************/
#include "f28002x_device.h"         // f28002x Headerfile Include File
#include "f28002x.h"       // f28002x Examples Include File
#include "NTC_Table.h"

//#pragma DATA_SECTION(u16NTC3435TempTable, ".TI.ramconsts");
//#pragma DATA_SECTION(u16NTC3950TempTable, ".TI.ramconsts");

struct NTC_Temp_STRUCT NTC_Temp;
/**********************************************************************
* 功能描述： 通过查表方式初始化时温度检测，找出当前初始温度值，
*           减少后续功能执行时代码查找的工作量
*
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 其它说明：
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
***********************************************************************/
int16 s16TemperatureInit(const Uint16 u16Array[], Uint16 u16ADCValue, Uint16 Table_Size)
{
    int16 s16TableIndex;

    for( s16TableIndex = 0 ; s16TableIndex < Table_Size ; s16TableIndex++ )   //初始化先轮询一遍温度表，匹配初始温度值
    {
        if( u16ADCValue > u16Array[s16TableIndex] )
        {
            break;
        }
    }

    return s16TableIndex - 40;                                       //温度值表0代表-40℃，需要减去40才是实际温度值
}
/**********************************************************************
* 功能描述： 获取温度，根据初始化获取的温度值作为基数进行查表，减少查表次数
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 其它说明：
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
***********************************************************************/
int16 s16GetTemperature(const Uint16 u16Array[], int16 s16LastTemp, Uint16 u16NewADCValue)
{
    int16 s16TableIndex = s16LastTemp + 40;

    if( u16NewADCValue < u16Array[s16TableIndex] )
    {
        s16TableIndex++;
    }
    else if( ( u16NewADCValue > u16Array[s16TableIndex - 1] ) && ( s16TableIndex >= 1 ) )
    {
        s16TableIndex--;
    }

    return s16TableIndex - 40;
}

/**********************************************************************
* 功能描述： 初始化获取温度值，减少查表次数
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 其它说明：
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
***********************************************************************/
void NTCTempInit(void)
{
    static Uint16 NTCInit_Count = 0;

    NTC_Temp.u16_NTC_IGBT_ADC = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER14);

    if( SYSFlag.bit.NTCInit_OK == 0 )
    {
        ADC_forceSOC(myADCA_BASE, ADC_SOC_NUMBER14);

        NTC_Temp.u32_NTC_IGBT_AdcSum += NTC_Temp.u16_NTC_IGBT_ADC;

        NTCInit_Count++;
    }
    if( NTCInit_Count >= 16 )
    {
        NTC_Temp.u16_NTC_IGBT_AdcAver = NTC_Temp.u32_NTC_IGBT_AdcSum/16;

        NTC_Temp.u32_NTC_IGBT_AdcSum = 0;

        NTCInit_Count = 0;
        SYSFlag.bit.NTCInit_OK = 1;

        NTC_Temp.s16_NTC_IGBT_Temperature = s16TemperatureInit(u16NTC_GD40PIX120C5S_TempTable, NTC_Temp.u16_NTC_IGBT_AdcAver, dNTC_GD40PIX120C5S_TempTable_Size);


    }


}

/**********************************************************************
* 功能描述： 获取温度值
* 输入参数： 无
* 输出参数： 无
* 返 回 值： 无
* 其它说明：
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
***********************************************************************/
void NTCTempProtect(void)
{
    static Uint16 NTC_Samp_Count = 0;

    NTC_Temp.u16_NTC_IGBT_ADC = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER14);


    ADC_forceSOC(myADCA_BASE, ADC_SOC_NUMBER14);

    NTC_Temp.u32_NTC_IGBT_AdcSum += NTC_Temp.u16_NTC_IGBT_ADC;

    NTC_Samp_Count++;

    if( NTC_Samp_Count >= 16 )
    {
        NTC_Temp.u16_NTC_IGBT_AdcAver = NTC_Temp.u32_NTC_IGBT_AdcSum/16;

        NTC_Temp.u32_NTC_IGBT_AdcSum = 0;

        NTC_Samp_Count = 0;


        NTC_Temp.s16_NTC_IGBT_Temperature = s16GetTemperature(u16NTC_GD40PIX120C5S_TempTable, NTC_Temp.s16_NTC_IGBT_Temperature, NTC_Temp.u16_NTC_IGBT_AdcAver);

    }
}
