#pragma once

#include <functional>
#include <map>
#include <memory>
#include <vector>

#include <fc/optional.hpp>
#include <fc/variant_object.hpp>
#include <graphene/plugins/json_rpc/utility.hpp>
#include <graphene/plugins/json_rpc/plugin.hpp>
#include <graphene/plugins/database_api/state.hpp>
#include <graphene/plugins/database_api/api_objects/master_authority_history_api_object.hpp>
#include <graphene/plugins/database_api/api_objects/account_recovery_request_api_object.hpp>
#include <graphene/plugins/database_api/api_objects/proposal_api_object.hpp>
#include <graphene/plugins/chain/plugin.hpp>

#include <graphene/api/chain_api_properties.hpp>

#include "forward.hpp"

namespace graphene { namespace plugins { namespace database_api {

using namespace graphene::chain;
using namespace graphene::protocol;
using namespace graphene::api;
using fc::variant;
using std::vector;
using plugins::json_rpc::void_type;
using plugins::json_rpc::msg_pack;
using plugins::json_rpc::msg_pack_transfer;

struct database_index_info {
    std::string name;
    std::size_t record_count;
};

struct database_info {
    std::size_t total_size;
    std::size_t free_size;
    std::size_t reserved_size;
    std::size_t used_size;

    std::vector<database_index_info> index_list;
};

struct scheduled_hardfork {
    hardfork_version hf_version;
    fc::time_point_sec live_time;
};

struct withdraw_route {
    std::string from_account;
    std::string to_account;
    uint16_t percent;
    bool auto_vest;
};

enum withdraw_route_type {
    incoming, outgoing, all
};

enum delegations_type {
    delegated, received
};


struct tag_count_object {
    string tag;
    uint32_t count;
};

struct get_tags_used_by_author {
    vector<tag_count_object> tags;
};

struct signed_block_api_object : public signed_block {
    signed_block_api_object(const signed_block &block) : signed_block(block) {
        block_id = id();
        signing_key = signee();
        transaction_ids.reserve(transactions.size());
        for (const signed_transaction &tx : transactions) {
            transaction_ids.push_back(tx.id());
        }
    }

    signed_block_api_object() {
    }

    block_id_type block_id;
    public_key_type signing_key;
    vector<transaction_id_type> transaction_ids;
};

struct account_on_sale_api_object {
    std::string account;
    std::string account_seller;
    asset account_offer_price = asset(0, TOKEN_SYMBOL);
    time_point_sec account_on_sale_start_time = fc::time_point_sec::min();

    std::string target_buyer;//set target buyer, without auction

    bool account_on_auction = false;
    asset current_bid = asset(0, TOKEN_SYMBOL);//next bid must be >= max(account_offer_price/10,current_bid-last_bid)
    std::string current_bidder;
    public_key_type current_bidder_key;
    asset last_bid = asset(0, TOKEN_SYMBOL);

    account_on_sale_api_object(const graphene::chain::account_object &a)
    :   account(a.name), account_seller(a.account_seller), account_offer_price(a.account_offer_price),
        account_on_sale_start_time(a.account_on_sale_start_time),
        target_buyer(a.target_buyer), current_bid(a.current_bid),
        current_bidder(a.current_bidder), current_bidder_key(a.current_bidder_key), last_bid(a.last_bid){
    }


    account_on_sale_api_object() {
    }
};

struct subaccount_on_sale_api_object {
    std::string account;
    std::string subaccount_seller;
    asset subaccount_offer_price = asset(0, TOKEN_SYMBOL);

    subaccount_on_sale_api_object(const graphene::chain::account_object &a)
    :   account(a.name), subaccount_seller(a.subaccount_seller), subaccount_offer_price(a.subaccount_offer_price) {
    }

    subaccount_on_sale_api_object() {
    }
};

using block_applied_callback = std::function<void(const variant &block_header)>;

///               API,                                    args,                return
DEFINE_API_ARGS(get_block_header,                 msg_pack, optional<block_header>)
DEFINE_API_ARGS(get_block,                        msg_pack, optional<signed_block>)
DEFINE_API_ARGS(get_irreversible_block_header,    msg_pack, optional<block_header>)
DEFINE_API_ARGS(get_irreversible_block,           msg_pack, optional<signed_block>)
DEFINE_API_ARGS(set_block_applied_callback,       msg_pack, void_type)
DEFINE_API_ARGS(get_config,                       msg_pack, variant_object)
DEFINE_API_ARGS(get_dynamic_global_properties,    msg_pack, dynamic_global_property_api_object)
DEFINE_API_ARGS(get_chain_properties,             msg_pack, chain_api_properties)
DEFINE_API_ARGS(get_hardfork_version,             msg_pack, hardfork_version)
DEFINE_API_ARGS(get_next_scheduled_hardfork,      msg_pack, scheduled_hardfork)
DEFINE_API_ARGS(get_accounts,                     msg_pack, std::vector<account_api_object>)
DEFINE_API_ARGS(lookup_account_names,             msg_pack, std::vector<optional<account_api_object> >)
DEFINE_API_ARGS(lookup_accounts,                  msg_pack, std::set<std::string>)
DEFINE_API_ARGS(get_account_count,                msg_pack, uint64_t)
DEFINE_API_ARGS(get_master_history,               msg_pack, std::vector<master_authority_history_api_object>)
DEFINE_API_ARGS(get_recovery_request,             msg_pack, optional<account_recovery_request_api_object>)
DEFINE_API_ARGS(get_escrow,                       msg_pack, optional<escrow_api_object>)
DEFINE_API_ARGS(get_withdraw_routes,              msg_pack, std::vector<withdraw_route>)

DEFINE_API_ARGS(get_vesting_delegations,          msg_pack, vector<vesting_delegation_api_object>)
DEFINE_API_ARGS(get_expiring_vesting_delegations, msg_pack, vector<vesting_delegation_expiration_api_object>)

DEFINE_API_ARGS(get_transaction_hex,              msg_pack, std::string)
DEFINE_API_ARGS(get_required_signatures,          msg_pack, std::set<public_key_type>)
DEFINE_API_ARGS(get_potential_signatures,         msg_pack, std::set<public_key_type>)
DEFINE_API_ARGS(verify_authority,                 msg_pack, bool)
DEFINE_API_ARGS(verify_account_authority,         msg_pack, bool)
DEFINE_API_ARGS(get_database_info,                msg_pack, database_info)
DEFINE_API_ARGS(get_proposed_transactions,        msg_pack, std::vector<proposal_api_object>)

DEFINE_API_ARGS(get_accounts_on_sale,             msg_pack, std::vector<account_on_sale_api_object>)
DEFINE_API_ARGS(get_accounts_on_auction,          msg_pack, std::vector<account_on_sale_api_object>)
DEFINE_API_ARGS(get_subaccounts_on_sale,          msg_pack, std::vector<subaccount_on_sale_api_object>)


/**
 * @brief The database_api class implements the RPC API for the chain database.
 *
 * This API exposes accessors on the database which query state tracked by a blockchain validating node. This API is
 * read-only; all modifications to the database must be performed via transactions. Transactions are broadcast via
 * the @ref network_broadcast_api.
 */
class plugin final : public appbase::plugin<plugin> {
public:
    constexpr static const char *plugin_name = "database_api";

    static const std::string &name() {
        static std::string name = plugin_name;
        return name;
    }

    APPBASE_PLUGIN_REQUIRES(
            (json_rpc::plugin)
            (chain::plugin)
    )

    void set_program_options(boost::program_options::options_description &cli, boost::program_options::options_description &cfg) override{}

    void plugin_initialize(const boost::program_options::variables_map &options) override;

    void plugin_startup() override;

    void plugin_shutdown() override{}

    plugin();

    ~plugin();

    ///////////////////
    // Subscriptions //
    ///////////////////

    void set_subscribe_callback(std::function<void(const variant &)> cb, bool clear_filter);

    void set_pending_transaction_callback(std::function<void(const variant &)> cb);

    /**
     * @brief Stop receiving any notifications
     *
     * This unsubscribes from all subscribed objects.
     */
    void cancel_all_subscriptions();


    /**
     * @brief Clear disconnected callbacks on applied block
     */

    void clear_block_applied_callback();

    DECLARE_API(
        /**
         *  This API is a short-cut for returning all of the state required for a particular URL
         *  with a single query.
         */


        /////////////////////////////
        // Blocks and transactions //
        /////////////////////////////

        /**
         * @brief Retrieve a block header
         * @param block_num Height of the block whose header should be returned
         * @return header of the referenced block, or null if no matching block was found
         */
        (get_block_header)

        /**
         * @brief Retrieve a full, signed block
         * @param block_num Height of the block to be returned
         * @return the referenced block, or null if no matching block was found
         */
        (get_block)

        /**
         * @brief Retrieve a irreversible block header
         * @param block_num Height of the block whose header should be returned
         * @return header of the referenced block, or null if no matching block was found
         */
        (get_irreversible_block_header)

        /**
         * @brief Retrieve a full, signed irreversible block
         * @param block_num Height of the block to be returned
         * @return the referenced block, or null if no matching block was found
         */
        (get_irreversible_block)

        /**
         * @brief Set callback which is triggered on each generated block
         * @param callback function which should be called
         */
        (set_block_applied_callback)

        /////////////
        // Globals //
        /////////////

        /**
         * @brief Retrieve compile-time constants
         */
        (get_config)

        /**
         * @brief Retrieve the current @ref dynamic_global_property_object
         */
        (get_dynamic_global_properties)

        (get_chain_properties)

        (get_hardfork_version)

        (get_next_scheduled_hardfork)


        //////////////
        // Accounts //
        //////////////

        (get_accounts)
        /**
         * @brief Get a list of accounts by name
         * @param account_names Names of the accounts to retrieve
         * @return The accounts holding the provided names
         *
         * This function has semantics identical to @ref get_objects
         */
        (lookup_account_names)

        /**
         * @brief Get names and IDs for registered accounts
         * @param lower_bound_name Lower bound of the first name to return
         * @param limit Maximum number of results to return -- must not exceed 1000
         * @return Map of account names to corresponding IDs
         */
        (lookup_accounts)

        //////////////
        // Balances //
        //////////////

        /**
         * @brief Get an account's balances in various assets
         * @param name of the account to get balances for
         * @param assets names of the assets to get balances of; if empty, get all assets account has a balance in
         * @return Balances of the account
         */


        /**
         * @brief Get the total number of accounts registered with the blockchain
         */
        (get_account_count)

        (get_master_history)

        (get_recovery_request)

        (get_escrow)

        (get_withdraw_routes)

        (get_vesting_delegations)
        (get_expiring_vesting_delegations)
        // (list_vesting_delegations)
        // (find_vesting_delegations)
        // (list_vesting_delegation_expirations)
        // (find_vesting_delegation_expirations)

        ////////////
        // Assets //
        ////////////




        ////////////////////////////
        // Authority / Validation //
        ////////////////////////////

        /// @brief Get a hexdump of the serialized binary form of a transaction
        (get_transaction_hex)

        /**
         *  This API will take a partially signed transaction and a set of public keys that the owner has the ability to sign for
         *  and return the minimal subset of public keys that should add signatures to the transaction.
         */
        (get_required_signatures)

        /**
         *  This method will return the set of all public keys that could possibly sign for a given transaction.  This call can
         *  be used by wallets to filter their set of public keys to just the relevant subset prior to calling @ref get_required_signatures
         *  to get the minimum subset.
         */
        (get_potential_signatures)

        /**
         * @return true of the @ref trx has all of the required signatures, otherwise throws an exception
         */
        (verify_authority)

        /*
        * @return true if the signers have enough authority to authorize an account
        */
        (verify_account_authority)


        (get_database_info)

        (get_proposed_transactions)

        /**
         * @brief Get names for accounts on sale or subaccounts on sale
         * @param from -- offset
         * @param limit -- number of results to return -- must not exceed 1000
         * @return List of accounts on sale or subaccounts on sale
         */
        (get_accounts_on_sale)
        (get_accounts_on_auction)
        (get_subaccounts_on_sale)
    )

private:
    struct api_impl;
    std::shared_ptr<api_impl> my;
};

inline void register_database_api(){
    appbase::app().register_plugin<plugin>();
}

} } } // graphene::plugins::database_api


FC_REFLECT((graphene::plugins::database_api::scheduled_hardfork), (hf_version)(live_time))
FC_REFLECT((graphene::plugins::database_api::withdraw_route), (from_account)(to_account)(percent)(auto_vest))

FC_REFLECT_ENUM(graphene::plugins::database_api::withdraw_route_type, (incoming)(outgoing)(all))
FC_REFLECT_ENUM(graphene::plugins::database_api::delegations_type, (delegated)(received))

FC_REFLECT((graphene::plugins::database_api::tag_count_object), (tag)(count))

FC_REFLECT((graphene::plugins::database_api::get_tags_used_by_author), (tags))

FC_REFLECT((graphene::plugins::database_api::signed_block_api_object), (block_id)(signing_key)(transaction_ids))

FC_REFLECT((graphene::plugins::database_api::database_index_info), (name)(record_count))
FC_REFLECT((graphene::plugins::database_api::database_info), (total_size)(free_size)(reserved_size)(used_size)(index_list))

FC_REFLECT((graphene::plugins::database_api::account_on_sale_api_object), (account)(account_seller)(account_offer_price)(account_on_sale_start_time)(target_buyer)(current_bid)(current_bidder)(current_bidder_key)(last_bid))
FC_REFLECT((graphene::plugins::database_api::subaccount_on_sale_api_object), (account)(subaccount_seller)(subaccount_offer_price))
