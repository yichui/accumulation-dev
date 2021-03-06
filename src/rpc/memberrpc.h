#ifndef _MEMBER_RPC_H
#define _MEMBER_RPC_H

#include <string>

#include "jsonrpc.h"
#include "msgpackrpc.h"
#include "drpc.h"

template<typename T, typename RPCPROTOCOL = dodo::MsgpackProtocol >
class MemberRpcService : public dodo::rpc < RPCPROTOCOL>
{
public:
    MemberRpcService()
    {
        mObj = nullptr;
    }

    void        setObject(T* p)
    {
        mObj = p;
    }

    template<typename... Args>
    void        registerHandle(std::string name, void (T::*callback)(Args...))
    {
        def(name.c_str(), [this, callback](Args... args){
            if (mObj != nullptr)
            {
                (mObj->*callback)(args...);
            }
            mObj = nullptr;
        });
    }

    template<typename PBType, typename... Args>
    void        registerPBHandle(void (T::*callback)(PBType, Args...))
    {
        registerHandle(std::remove_reference<PBType>::type::descriptor()->full_name(), callback);
    }

private:
    T*         mObj;
};
#endif