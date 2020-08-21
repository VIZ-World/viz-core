#pragma once

#include <appbase/application.hpp>
#include <graphene/plugins/chain/plugin.hpp>
#include <graphene/protocol/types.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <graphene/chain/chain_object_types.hpp>
#include <graphene/chain/content_object.hpp>

namespace graphene { namespace plugins { namespace custom_protocol_api {
    using namespace graphene::chain;
    using chainbase::object;
    using chainbase::object_id;
    using chainbase::allocator;

        //
        // Plugins should #define their SPACE_ID's so plugins with
        // conflicting SPACE_ID assignments can be compiled into the
        // same binary (by simply re-assigning some of the conflicting #defined
        // SPACE_ID's in a build script).
        //
        // Assignment of SPACE_ID's cannot be done at run-time because
        // various template automagic depends on them being known at compile
        // time.
    	//
    	// Take ID as new plugin catalogue counter
        //
    #ifndef CUSTOM_PROTOCOL_SPACE_ID
    #define CUSTOM_PROTOCOL_SPACE_ID 25
    #endif

    enum custom_protocol_api_object_type {
        custom_protocol_object_type = (CUSTOM_PROTOCOL_SPACE_ID << 8)
    };

    class custom_protocol_object: public object<custom_protocol_object_type, custom_protocol_object> {
    public:
        template<typename Constructor, typename Allocator>
        custom_protocol_object(Constructor&& c, allocator<Allocator> a) {
            c(*this);
        }

        id_type id;
        account_name_type account;
        account_name_type custom_protocol_id;
        uint64_t custom_sequence = 0;
        uint64_t custom_sequence_block_num = 0;
    };

    typedef object_id<custom_protocol_object> custom_protocol_id_type;

    using namespace boost::multi_index;

    struct by_account_custom_sequence_block_num;
    using custom_protocol_index = multi_index_container<
        custom_protocol_object,
        indexed_by<
            ordered_unique<
                tag<by_id>,
                member<custom_protocol_object, custom_protocol_id_type, &custom_protocol_object::id>>,
            ordered_non_unique<
                tag<by_account_custom_sequence_block_num>,
                composite_key<
                    custom_protocol_object,
                    member<custom_protocol_object, account_name_type, &custom_protocol_object::account>,
                    member<custom_protocol_object, uint64_t, &custom_protocol_object::custom_sequence_block_num>
                >,
                composite_key_compare <std::less<account_name_type>, std::greater<uint64_t>>
            >
        >,
        allocator<custom_protocol_object>>;
} } } // graphene::api


CHAINBASE_SET_INDEX_TYPE(
    graphene::plugins::custom_protocol_api::custom_protocol_object,
    graphene::plugins::custom_protocol_api::custom_protocol_index)
FC_REFLECT((graphene::plugins::custom_protocol_api::custom_protocol_object),(account)(id)(custom_sequence)(custom_sequence_block_num));