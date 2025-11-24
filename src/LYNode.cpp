/*
 * LYNode.cpp
 * 灵影LYNode应用层程序
 * 日期: 2025.11.13
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

int NodeClass::WritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC)
{
	if(Position<0){
		Position = -Position;
		Position |= (1<<15);
	}
	u8 bBuf[7];
	bBuf[0] = ACC;
	Host2SCS(bBuf+1, bBuf+2, Position);
	Host2SCS(bBuf+3, bBuf+4, 0);
	Host2SCS(bBuf+5, bBuf+6, Speed);
	
	return genWrite(ID, NodeClass_ACC, bBuf, 7);
}

int NodeClass::RegWritePosEx(u8 ID, s16 Position, u16 Speed, u8 ACC)
{
	if(Position<0){
		Position = -Position;
		Position |= (1<<15);
	}
	u8 bBuf[7];
	bBuf[0] = ACC;
	Host2SCS(bBuf+1, bBuf+2, Position);
	Host2SCS(bBuf+3, bBuf+4, 0);
	Host2SCS(bBuf+5, bBuf+6, Speed);
	
	return regWrite(ID, NodeClass_ACC, bBuf, 7);
}

void NodeClass::SyncWritePosEx(u8 ID[], u8 IDN, s16 Position[], u16 Speed[], u8 ACC[])
{
    u8 offbuf[7*IDN];
    for(u8 i = 0; i<IDN; i++){
		if(Position[i]<0){
			Position[i] = -Position[i];
			Position[i] |= (1<<15);
		}
		u16 V;
		if(Speed){
			V = Speed[i];
		}else{
			V = 0;
		}
		if(ACC){
			offbuf[i*7] = ACC[i];
		}else{
			offbuf[i*7] = 0;
		}
        Host2SCS(offbuf+i*7+1, offbuf+i*7+2, Position[i]);
        Host2SCS(offbuf+i*7+3, offbuf+i*7+4, 0);
        Host2SCS(offbuf+i*7+5, offbuf+i*7+6, V);
    }
    syncWrite(ID, IDN, NodeClass_ACC, offbuf, 7);
}

void NodeClass::SyncWriteSpe(u8 ID[], u8 IDN, s16 Speed[], u8 ACC[])
{
    u8 offbuf[7*IDN];
    for(u8 i = 0; i<IDN; i++){
		if(Speed[i]<0){
			Speed[i] = -Speed[i];
			Speed[i] |= (1<<15);
		}
		if(ACC){
			offbuf[i*7] = ACC[i];
		}else{
			offbuf[i*7] = 0;
		}
        Host2SCS(offbuf+i*7+1, offbuf+i*7+2, 0);
        Host2SCS(offbuf+i*7+3, offbuf+i*7+4, 0);
        Host2SCS(offbuf+i*7+5, offbuf+i*7+6, Speed[i]);
    }
    syncWrite(ID, IDN, NodeClass_ACC, offbuf, 7);
}

int NodeClass::WheelMode(u8 ID)
{
	return writeByte(ID, NodeClass_MODE, 1);		
}

int NodeClass::WriteSpe(u8 ID, s16 Speed, u8 ACC)
{
	if(Speed<0){
		Speed = -Speed;
		Speed |= (1<<15);
	}
	u8 bBuf[7];
	bBuf[0] = ACC;
	Host2SCS(bBuf+1, bBuf+2, 0);
	Host2SCS(bBuf+3, bBuf+4, 0);
	Host2SCS(bBuf+5, bBuf+6, Speed);
	
	return genWrite(ID, NodeClass_ACC, bBuf, 7);
}

int NodeClass::EnableTorque(u8 ID, u8 Enable)
{
	return writeByte(ID, NodeClass_TORQUE_ENABLE, Enable);
}

int NodeClass::unLockEprom(u8 ID)
{
	return writeByte(ID, NodeClass_LOCK, 0);
}

int NodeClass::LockEprom(u8 ID)
{
	return writeByte(ID, NodeClass_LOCK, 1);
}

int NodeClass::CalibrationOfs(u8 ID)
{
	return writeByte(ID, NodeClass_TORQUE_ENABLE, 128);
}

int NodeClass::FeedBack(int ID)
{
	int nLen = Read(ID, NodeClass_PRESENT_POSITION_L, Mem, sizeof(Mem));
	if(nLen!=sizeof(Mem)){
		return -1;
	}
	return nLen;
}

int NodeClass::ReadPos(int ID)
{
	int Pos = -1;
	if(ID==-1){
		Pos = Mem[NodeClass_PRESENT_POSITION_H-NodeClass_PRESENT_POSITION_L];
		Pos <<= 8;
		Pos |= Mem[NodeClass_PRESENT_POSITION_L-NodeClass_PRESENT_POSITION_L];
	}else{
		Pos = readWord(ID, NodeClass_PRESENT_POSITION_L);
	}
	if(Pos&(1<<15)){
		Pos = -(Pos&~(1<<15));
	}
	
	return Pos;
}

int NodeClass::ReadSpeed(int ID)
{
	int Speed = -1;
	if(ID==-1){
		Speed = Mem[NodeClass_PRESENT_SPEED_H-NodeClass_PRESENT_POSITION_L];
		Speed <<= 8;
		Speed |= Mem[NodeClass_PRESENT_SPEED_L-NodeClass_PRESENT_POSITION_L];
	}else{
		Speed = readWord(ID, NodeClass_PRESENT_SPEED_L);
	}
	if(Speed&(1<<15)){
		Speed = -(Speed&~(1<<15));
	}	
	return Speed;
}

int NodeClass::ReadLoad(int ID)
{
	int Load = -1;
	if(ID==-1){
		Load = Mem[NodeClass_PRESENT_LOAD_H-NodeClass_PRESENT_POSITION_L];
		Load <<= 8;
		Load |= Mem[NodeClass_PRESENT_LOAD_L-NodeClass_PRESENT_POSITION_L];
	}else{
		Load = readWord(ID, NodeClass_PRESENT_LOAD_L);
	}
	if(Load&(1<<10)){
		Load = -(Load&~(1<<10));
	}
	return Load;
}

int NodeClass::ReadVoltage(int ID)
{	
	int Voltage = -1;
	if(ID==-1){
		Voltage = Mem[NodeClass_PRESENT_VOLTAGE-NodeClass_PRESENT_POSITION_L];	
	}else{
		Voltage = readByte(ID, NodeClass_PRESENT_VOLTAGE);
	}
	return Voltage;
}

int NodeClass::ReadTemper(int ID)
{	
	int Temper = -1;
	if(ID==-1){
		Temper = Mem[NodeClass_PRESENT_TEMPERATURE-NodeClass_PRESENT_POSITION_L];	
	}else{
		Temper = readByte(ID, NodeClass_PRESENT_TEMPERATURE);
	}
	return Temper;
}

int NodeClass::ReadMove(int ID)
{
	int Move = -1;
	if(ID==-1){
		Move = Mem[NodeClass_MOVING-NodeClass_PRESENT_POSITION_L];	
	}else{
		Move = readByte(ID, NodeClass_MOVING);
	}
	return Move;
}

int NodeClass::ReadCurrent(int ID)
{
	int Current = -1;
	if(ID==-1){
		Current = Mem[NodeClass_PRESENT_CURRENT_H-NodeClass_PRESENT_POSITION_L];
		Current <<= 8;
		Current |= Mem[NodeClass_PRESENT_CURRENT_L-NodeClass_PRESENT_POSITION_L];
	}else{
		Current = readWord(ID, NodeClass_PRESENT_CURRENT_L);
	}
	if(Current&(1<<15)){
		Current = -(Current&~(1<<15));
	}	
	return Current;
}

int NodeClass::ServoMode(u8 ID)
{
	return writeByte(ID, NodeClass_MODE, 0);		
}
