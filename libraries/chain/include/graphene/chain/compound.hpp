#pragma once

#include <cstdint>

#ifdef BUILD_TESTNET
#include <graphene/protocol/config_testnet.hpp>
#else
#include <graphene/protocol/config.hpp>
#endif
#include <graphene/protocol/types.hpp>

#include <fc/uint128_t.hpp>

namespace graphene {
    namespace protocol {
    }
}
