/*  Copyright (c) 2013-2015 Tencent. All rights reserved.  */
/*
 * SocketSelect.h
 *
 *  Created on: 2013-7-23
 *      Author: jiahongxu
 */


#ifndef _NETWORK_SOCKSTSELECT_H_
#define _NETWORK_SOCKSTSELECT_H_

#include "network/socketpoll.h"
#include "network/socketbreaker.h"

namespace libcommon {
    
    
    class SocketSelect {
    public:
        SocketSelect(SocketBreaker& _breaker, bool _autoclear = false);
        virtual ~SocketSelect();
        
        void PreSelect();
        void Read_FD_SET(SOCKET _socket);
        void Write_FD_SET(SOCKET _socket);
        void Exception_FD_SET(SOCKET _socket);
        
        virtual int Select();
        virtual int Select(int _msec);
        
        int  Ret() const;
        int  Errno() const;
        
        int  Read_FD_ISSET(SOCKET _socket) const;
        int  Write_FD_ISSET(SOCKET _socket) const;
        int  Exception_FD_ISSET(SOCKET _socket) const;
        
        bool IsBreak() const;
        bool IsException() const;
        
        SocketBreaker& Breaker();
        
        SocketPoll&  Poll();
        
    private:
        SocketSelect(const SocketSelect&);
        SocketSelect& operator=(const SocketSelect&);
        
    protected:
        SocketPoll  socket_poll_;
    };
    
}

#endif
