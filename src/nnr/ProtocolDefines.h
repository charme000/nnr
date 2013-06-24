#ifndef PROTOCOLDEFINES_H
#define PROTOCOLDEFINES_H

//协议头
//标识
#define PG_SIGN 0x72//nnr
//版本
#define PG_VER_INVALID 0
#define PG_VER_01 0x01 //01
//类型
#define PKG_TYPE_BROAD 0x2 //广播包
#define PKG_TYPE_DIRECT 0x1 //定向包
#define PKG_TYPE_NONE 0x0f //无效包

//协议头长度
#define PG_HEADER_LEN 0x02


// 定义协议版本
#define _PROTOCOL_1_0 "nnr_1_0"
#define _PROTOCOL_2_0 "nnr_2_0"


#endif // PROTOCOLDEFINES_H
