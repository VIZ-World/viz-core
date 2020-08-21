#pragma once

#include <graphene/plugins/custom_protocol_api/custom_protocol_api_object.hpp>
#include <boost/algorithm/string.hpp>

namespace graphene { namespace plugins { namespace custom_protocol_api {
    using graphene::plugins::custom_protocol_api::custom_protocol_object;
    struct operation_visitor {
        operation_visitor(database& db, const uint8_t custom_protocol_store_size);
        using result_type = void;

        database& _db;
        uint8_t _custom_protocol_store_size;

        void operator()(const custom_operation& op) const;

        template<typename Op>
        void operator()(Op&&) const {
        } /// ignore all other ops
    };

} } } // graphene::plugins::custom_protocol_api
