/******************************************************************************/
/** 模块名称：扩展外设驱动                                                   **/
/** 文件名称：ms5536function.h                                               **/
/** 版    本：V1.0.0                                                         **/
/** 简    介：用于实现MS5536C表压传感器的操作                                **/
/**           每台MS5536C表压传感器都有出厂校准的6个修正系数：               **/
/**           C1: Pressure sensitivity (13 Bit)                              **/
/**           C2: Pressure offset (13 Bit)                                   **/
/**           C3: Temperature coefficient of pressure sensitivity (9 Bit)    **/
/**           C4: Temperature coefficient of pressure offset (9 Bit)         **/
/**           C5: Reference Temperature (12 Bit)                             **/
/**           C6: Temperature coefficient of the temperature (8 Bit)         **/
/**           分别存储于4个16位字中，可通过逻辑运算得到：                    **/
/**   Word 1：                                                               **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   |        C3/II (8-Bit)          |         C5/II(8-Bit)          |      **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   |DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|      **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   Word 2：                                                               **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   |        C4/II (8-Bit)          |         C6(8-Bit)             |      **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   |DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|      **/
/**   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+      **/
/**   Word 3：                                                               **/
/**   +-----------------+-------------------------------------------------+  **/
/**   |  C5/I (4-Bit)   |                 C1/II (12-Bit)                  |  **/
/**   +----+----+---+---+----+----+---+---+---+---+---+---+---+---+---+---+  **/
/**   |DB11|DB10|DB9|DB8|DB11|DB10|DB9|DB8|DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|  **/
/**   +----+----+---+---+----+----+---+---+---+---+---+---+---+---+---+---+  **/
/**   Word 4：                                                               **/
/**   +----+----+----+------------------------------------------------------+**/
/**   |C1/I|C4/I|C3/I|                     C2 (13-Bit)                      |**/
/**   +----+----+----+----+----+----+---+---+---+---+---+---+---+---+---+---+**/
/**   |DB12|DB8 |DB8 |DB12|DB11|DB10|DB9|DB8|DB7|DB6|DB5|DB4|DB3|DB2|DB1|DB0|**/
/**   +----+----+----+----+----+----+---+---+---+---+---+---+---+---+---+---+**/
/**           命令由：3位起始位+指令+3位停止位构成：                         **/
/**           1、读压力数值命令序列：                                        **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|            **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |  1 |  1 |  1 |  1 |  0 |  1 |  0 |  0 |  0 |  0 |            **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |   Start-bit  |     Setup-bits    |    Stop-bit  |            **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           2、读温度数值命令序列：                                        **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|            **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |  1 |  1 |  1 |  1 |  0 |  0 |  1 |  0 |  0 |  0 |            **/
/**           +----+----+----+----+----+----+----+----+----+----+            **/
/**           |   Start-bit  |     Setup-bits    |    Stop-bit  |            **/
/**           +--------------+-------------------+--------------+            **/
/**           3、读校准系数Word1命令序列：                                   **/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|Bit10|Bit11|**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |  1 |  1 |  1 |  0 |  1 |  0 |  1 |  0 |  1 |  0 |  0  |  0  |**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |   Start-bit  |           Setup-bits        |     Stop-bit   |**/
/**           +--------------+-----------------------------+----------------+**/
/**           4、读校准系数Word3命令序列：                                   **/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|Bit10|Bit11|**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |  1 |  1 |  1 |  0 |  1 |  1 |  0 |  0 |  1 |  0 |  0  |  0  |**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |   Start-bit  |           Setup-bits        |     Stop-bit   |**/
/**           +--------------+-----------------------------+----------------+**/
/**           5、读校准系数Word2命令序列：                                   **/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|Bit10|Bit11|**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |  1 |  1 |  1 |  0 |  1 |  0 |  1 |  1 |  0 |  0 |  0  |  0  |**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |   Start-bit  |           Setup-bits        |     Stop-bit   |**/
/**           +--------------+-----------------------------+----------------+**/
/**           6、读校准系数Word4命令序列：                                   **/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |Bit0|Bit1|Bit2|Bit3|Bit4|Bit5|Bit6|Bit7|Bit8|Bit9|Bit10|Bit11|**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |  1 |  1 |  1 |  0 |  1 |  1 |  0 |  1 |  0 |  0 |  0  |  0  |**/
/**           +----+----+----+----+----+----+----+----+----+----+-----+-----+**/
/**           |   Start-bit  |           Setup-bits        |     Stop-bit   |**/
/**           +--------------+-----------------------------+----------------+**/
/**           7、复位命令由：16位的10序列及5位低电平组成。                   **/
/**--------------------------------------------------------------------------**/
/**使用说明：                                                                **/
/**    1、声明MS5536cObjectType对象实体，并调用函数Ms5536cInitialization初始 **/
/**       化对象实体。并读取校准数据                                         **/
/**    2、实现形如uint8_t f(uint8_t *txData,uint8_t *rxData,uint16_t number) **/
/**       的函数，将其赋值给MS5536cObjectType对象实体的ReadWriteMS函数指针。 **/
/**       实现数据的收发。                                                   **/
/**    3、实现形如void f(bool en)的函数，并将其赋值给MS5536cObjectType对象实 **/
/**       体的SetPhase函数指针。实现SCLK的相位切换，因为在给MS5536C发送命令时**/
/**       使用时钟的第1个跳变沿（模式0），而在接收MS5536C的数据时，使用时钟的**/
/**       第2个跳变沿（模式1）。                                             **/
/**    4、实现形如void f(volatile uint32_t nTime)的函数，并将其赋值给        **/
/**       MS5536cObjectType对象实体的Delayms函数指针。实现操作延时，单位毫秒 **/
/**    5、调用相应的函数实现需要的操作。                                     **/
/**--------------------------------------------------------------------------**/
/** 修改记录：                                                               **/
/**     版本      日期              作者              说明                   **/
/**     V1.0.0  2018-07-13          木南              创建文件               **/
/**                                                                          **/
/******************************************************************************/ 

#ifndef __MS5536_FUNCTION_H
#define __MS5536_FUNCTION_H

#include "stdint.h"
//#include "stdbool.h"

/*定义MS5536C时钟相位*/
typedef enum MS5536cPhase {
  MS5536_SCLK_RISE,     //上升沿
  MS5536_SCLK_FALL      //下降沿
}MS5536cPhaseType;

/*定义MS5536C对象类型*/
typedef struct MS5536cObject {
  uint16_t coef1;        //校准系数C1
  uint16_t coef2;        //校准系数C2
  uint16_t coef3;        //校准系数C3
  uint16_t coef4;        //校准系数C4
  uint16_t coef5;        //校准系数C5
  uint16_t coef6;        //校准系数C6
  
  int32_t pressure;     //压力100倍整数值
  int32_t temperature;  //温度100倍整数值
  
  float fPressure;     //压力浮点数值
  float fTemperature;  //温度浮点数值
  
  void (*ReadWriteMS)(uint8_t *txData,uint8_t *rxData,uint16_t number);	//读写MS5536C函数指针
  void (*Delayms)(volatile uint32_t Delay);	//毫秒延时函数指针
  void (*SetPhase)(MS5536cPhaseType phase);	//相位配置函数指针
}MS5536cObjectType;

/*读写MS5536C函数类型*/
typedef void (*MS5536cReadWriteMS)(uint8_t *txData,uint8_t *rxData,uint16_t number);
/*毫秒延时函数类型*/
typedef void (*MS5536cDelayms)(volatile uint32_t Delay);
/*相位操作函数类型*/
typedef void (*MS5536cSetPhase)(MS5536cPhaseType phase);

/* 对MS5536C对象进行初始化 */
void Ms5536cInitialization(MS5536cObjectType *ms,               //MS5536对象
                           MS5536cReadWriteMS readwrite,        //读写操作
                           MS5536cSetPhase setPhase,            //相位设置操作
                           MS5536cDelayms delayms               //延时操作
                          );

/*对MS5336C进行软件复位*/
void Ms5336cSoftwareReset(MS5536cObjectType *ms);

/* 获取压力和温度数据 */
void GetMeasureForMs5536c(MS5536cObjectType *ms);

#endif

/*********** (C) COPYRIGHT 1999-2019 Moonan Technology *********END OF FILE****/
