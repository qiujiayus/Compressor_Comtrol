/*********************************************************************
* 内容摘要：温度采集功能头文件
* 其它说明：
* 当前版本：V0.0.0
* 作   者： 拓邦微电研发中心
* 完成日期：2022-12-12
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
**********************************************************************/
#define dDebug_NoESTOP0             1                       //正式发布的版本屏蔽所有ESTOP0 1屏蔽 0调试
#define dDebug_NoNTCCheck           0                       //不进行NTC检测

#ifndef INVTEMPERATURE_H_
#define INVTEMPERATURE_H_
#if dDebugMode == 1
#define dNTCOTP__AIRTemp            63          //环境过温保护温度值
#define dNTCOTP__MOS_SR1Temp        70//90          //SR1 mos过温保护温度值
#define dNTCOTP__MOS_PFCTemp        70//140         //PFC mos过温保护温度值
#else
#define dNTCOTP__AIRTemp            63          //环境过温保护温度值
#define dNTCOTP__MOS_SR1Temp        90          //SR1 mos过温保护温度值
#define dNTCOTP__MOS_PFCTemp        140         //PFC mos过温保护温度值
#endif

#define dNTCOTW__AIRTemp            (dNTCOTP__AIRTemp-3)          //环境过温警告温度值
#define dNTCOTW__MOS_SR1Temp        (dNTCOTP__MOS_SR1Temp-3)          //SR1 mos过温警告温度值
#define dNTCOTW__MOS_PFCTemp        (dNTCOTP__MOS_PFCTemp-3)         //PFC mos过温警告温度值

#define dNTCOTP_AIRRecoverTemp          (dNTCOTP__AIRTemp-6)          //环境恢复温度
#define dNTCOTP_MOS_SR1RecoverTemp      (dNTCOTP__MOS_SR1Temp-20)          //SR1 mos恢复温度
#define dNTCOTP_MOS_PFCRecoverTemp      (dNTCOTP__MOS_PFCTemp-20)         //PFC mos恢复温度

#define dNTCOTP_ReducePowerTemp     80          //降低系统功率设定值
#define dNTCLTP__AIRTemp         -20          //环境低温保护温度值
#define dNTCLTP__MOS_SR1Temp         -20          //环境低温保护温度值
#define dNTCLTP__MOS_SR2Temp        -20          //散热器低温保护温度值
#define dNTCLTP_RecoverTemp         -10          //恢复温度

#define NTC_Period_Timer_ms     (1)

typedef union Temp_Flag_union
{
    Uint16        Flags;
    struct {
        Uint16 AIR_OTW :1;
        Uint16 AIR_OTP :1;
        Uint16 SR_OTW :1;
        Uint16 SR_OTP :1;
        Uint16 PFC_OTW :1;
        Uint16 PFC_OTP :1;
    }bit;
}TEMP_FLAG;

struct NTC_Temp_STRUCT
{
    int16 s16_NTC_IGBT_Temperature;

    Uint16 u16_NTC_IGBT_ADC;
    Uint16 u16_NTC_IGBT_AdcAver;
    Uint32 u32_NTC_IGBT_AdcSum;

    Uint16 u16Count_intial;
    TEMP_FLAG unFlag;
};
extern struct NTC_Temp_STRUCT NTC_Temp;

extern void NTCTempInit(void);
extern void NTCTempProtect(void);

#endif /* INVTEMPERATURE_H_ */
