/*
 * LYNode.h
 * 灵影LYNode应用层程序
 * 日期: 2025.11.13
 * 作者: txl
 */

#ifndef _LY_NODE_H
#define _LY_NODE_H

//内存表定义
//-------EPROM(只读)--------
#define LY_NODE_MODEL_L 3
#define LY_NODE_MODEL_H 4

//-------EPROM(读写)--------
#define LY_NODE_ID 5
#define LY_NODE_BAUD_RATE 6
#define LY_NODE_MIN_ANGLE_LIMIT_L 9
#define LY_NODE_MIN_ANGLE_LIMIT_H 10
#define LY_NODE_MAX_ANGLE_LIMIT_L 11
#define LY_NODE_MAX_ANGLE_LIMIT_H 12
#define LY_NODE_CW_DEAD 26
#define LY_NODE_CCW_DEAD 27
#define LY_NODE_OFS_L 31
#define LY_NODE_OFS_H 32
#define LY_NODE_MODE 33

//-------SRAM(读写)--------
#define LY_NODE_TORQUE_ENABLE 40
#define LY_NODE_ACC 41
#define LY_NODE_GOAL_POSITION_L 42
#define LY_NODE_GOAL_POSITION_H 43
#define LY_NODE_GOAL_TIME_L 44
#define LY_NODE_GOAL_TIME_H 45
#define LY_NODE_GOAL_SPEED_L 46
#define LY_NODE_GOAL_SPEED_H 47
#define LY_NODE_TORQUE_LIMIT_L 48
#define LY_NODE_TORQUE_LIMIT_H 49
#define LY_NODE_LOCK 55

//-------SRAM(只读)--------
#define LY_NODE_PRESENT_POSITION_L 56
#define LY_NODE_PRESENT_POSITION_H 57
#define LY_NODE_PRESENT_SPEED_L 58
#define LY_NODE_PRESENT_SPEED_H 59
#define LY_NODE_PRESENT_LOAD_L 60
#define LY_NODE_PRESENT_LOAD_H 61
#define LY_NODE_PRESENT_VOLTAGE 62
#define LY_NODE_PRESENT_TEMPERATURE 63
#define LY_NODE_MOVING 66
#define LY_NODE_PRESENT_CURRENT_L 69
#define LY_NODE_PRESENT_CURRENT_H 70

#include "SCSerial.h"

class NodeClass : public SCSerial
{
public:
	NodeClass();
	NodeClass(u8 End);
	NodeClass(u8 End, u8 Level);
	int ledSinglectrl(u8 node_id, u8 led_ch, u8 r, u8 g, u8 b);//根据led的ch将rgb(0~7)颜色传输至node模块缓存
	int ledAllctrl(u8 node_id, u8 num, u8 r, u8 g, u8);//全部led写入相同rgb(0~7)颜色传输至node模块缓存
	int ledFlush(u8 node_id, u8 led_num = 3);//刷新node模块点亮相应led灯,默认led为3通道
	int sbusFlush(u8 node_id);//将node模块中sbus数据通过总线缓存至内存
	u16 sbusGetch(u8 node_id, u8 ch);//解码sbus缓存返回通道数据
	u8 sbusStatus(u8 node_id);//解码sbus缓存返回状态
private:
	u8 sbus_frame[16*2+1];//sbus缓存
};

#endif