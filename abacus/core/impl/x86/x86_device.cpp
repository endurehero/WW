#include "abacus/core/device.h"

namespace WW{
namespace abacus{

template<>
void Device<X86>::getInfo(){
    LOG(ERROR) << "X86v device getinfo is not implemented!";
}

template<>
void Device<X86>::createStream(){
    LOG(ERROR) << "X86v device createStream is not implemented!";
}
} //namespace abacus
} // namespace WW