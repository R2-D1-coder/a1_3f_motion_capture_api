#ifndef __MPRO_H__
#define __MPRO_H__
#include "define/HppRigidDef.h"
#include <memory>
#include <list>
namespace Brute
{
    class ControllerHandler;
}

class Mpro
{
public:
    Mpro();
    ~Mpro();
    int32_t Init();
    int32_t Start();
    int32_t Deinit();
    int32_t GetResult(std::list<HPP::RigidInfo>& lis);
   
private:
    std::shared_ptr<Brute::ControllerHandler> m_alg;
};


#endif

