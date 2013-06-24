#ifndef DEFINES_H
#define DEFINES_H

// 监听者类型定义
#define _LISTENER_TCP_ "tcp_listener"
#define _LISTENER_PIPE_ "pipe_listener"


enum Flag_Tag
{
    InvalidFlag = 0,
    TcpFlag,
    PipeFlag
};

#define _EXE_ "exe"
#define _DLL_ "dll"
#define _XML_ "xml"
#define _XBEL_ "xbel"

#define _CLIENT_ATTRIBUTE_ "client"
#define _SERVER_ATTRIBUTE_ "server"

#endif // DEFINES_H
