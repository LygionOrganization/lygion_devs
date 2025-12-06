/*
 * LYNode.cpp
 * 灵影LYNode应用层程序
 * 日期: 2025.12.06
 * 作者: txl
 */

#include "LYNode.h"

NodeClass::NodeClass()
{
	End = 0;
}

NodeClass::NodeClass(u8 End):SCSerial(End)
{
}

NodeClass::NodeClass(u8 End, u8 Level):SCSerial(End, Level)
{
}

//根据led的ch将rgb(0~7)颜色传输至node模块缓存
int NodeClass::ledSingleCtrl(u8 node_id, u8 led_ch, u8 r, u8 g, u8 b)
{
	u8 rgb = (r<<6)|(g<<3)|(b<<1);
	u8 ch = led_ch + LY_NODE_RGB_CH1;
	return writeByte(node_id, ch, rgb);
}

//刷新node模块点亮相应led灯,默认led为2通道
int NodeClass::ledFlush(u8 node_id, u8 led_num)
{
	return writeByte(node_id, LY_NODE_RGB_CTL, led_num);
}

//全部led写入相同rgb(0~7)颜色传输至node模块缓存
int NodeClass::ledAllCtrl(u8 node_id, u8 num, u8 r, u8 g, u8 b)
{
	u8 rgb = (r<<6)|(g<<3)|(b<<1);
	u8 buf[num+1];
	buf[0] = num;
	for(u8 i= 0; i<num; i++){
		buf[i+1] = rgb;
	}
	return genWrite(node_id, LY_NODE_RGB_CTL, buf, num);
}

//将node模块中sbus数据通过总线缓存至内存
int NodeClass::sbusFlush(u8 node_id)
{
	return Read(node_id, LY_NODE_SBUS_STATUS, sbus_frame, sizeof(sbus_frame));
}
