#include <graphene/protocol/get_config.hpp>
#include <graphene/protocol/config.hpp>
#include <graphene/protocol/asset.hpp>
#include <graphene/protocol/version.hpp>

namespace graphene {
    namespace protocol {

        fc::variant_object get_config() {
            fc::mutable_variant_object result;

            result["CHAIN_100_PERCENT"] = CHAIN_100_PERCENT;
            result["CHAIN_1_PERCENT"] = CHAIN_1_PERCENT;
            result["CHAIN_ADDRESS_PREFIX"] = CHAIN_ADDRESS_PREFIX;
            result["CHAIN_BANDWIDTH_AVERAGE_WINDOW_SECONDS"] = CHAIN_BANDWIDTH_AVERAGE_WINDOW_SECONDS;
            result["CHAIN_BANDWIDTH_PRECISION"] = CHAIN_BANDWIDTH_PRECISION;
            result["CONSENSUS_BANDWIDTH_RESERVE_PERCENT"] = CONSENSUS_BANDWIDTH_RESERVE_PERCENT;
            result["CONSENSUS_BANDWIDTH_RESERVE_BELOW"] = CONSENSUS_BANDWIDTH_RESERVE_BELOW;
            result["CHAIN_HARDFORK_VERSION"] = CHAIN_HARDFORK_VERSION;
            result["CHAIN_VERSION"] = CHAIN_VERSION;
            result["CHAIN_BLOCK_INTERVAL"] = CHAIN_BLOCK_INTERVAL;
            result["CHAIN_BLOCKS_PER_DAY"] = CHAIN_BLOCKS_PER_DAY;
            result["CHAIN_BLOCKS_PER_YEAR"] = CHAIN_BLOCKS_PER_YEAR;
            result["CHAIN_CASHOUT_WINDOW_SECONDS"] = CHAIN_CASHOUT_WINDOW_SECONDS;
            result["CHAIN_ID"] = CHAIN_ID;
            result["CHAIN_HARDFORK_REQUIRED_WITNESSES"] = CHAIN_HARDFORK_REQUIRED_WITNESSES;
            result["CHAIN_INITIATOR_NAME"] = CHAIN_INITIATOR_NAME;
            result["CHAIN_INITIATOR_PUBLIC_KEY_STR"] = CHAIN_INITIATOR_PUBLIC_KEY_STR;
            result["CHAIN_INIT_SUPPLY"] = CHAIN_INIT_SUPPLY;
            result["CHAIN_COMMITTEE_ACCOUNT"] = CHAIN_COMMITTEE_ACCOUNT;
            result["CHAIN_COMMITTEE_PUBLIC_KEY_STR"] = CHAIN_COMMITTEE_PUBLIC_KEY_STR;
            result["CHAIN_IRREVERSIBLE_THRESHOLD"] = CHAIN_IRREVERSIBLE_THRESHOLD;
            result["CHAIN_IRREVERSIBLE_SUPPORT_MIN_RUN"] = CHAIN_IRREVERSIBLE_SUPPORT_MIN_RUN;
            result["CHAIN_MAX_ACCOUNT_NAME_LENGTH"] = CHAIN_MAX_ACCOUNT_NAME_LENGTH;
            result["CHAIN_MAX_ACCOUNT_WITNESS_VOTES"] = CHAIN_MAX_ACCOUNT_WITNESS_VOTES;
            result["CHAIN_BLOCK_SIZE"] = CHAIN_BLOCK_SIZE;
            result["CHAIN_MAX_COMMENT_DEPTH"] = CHAIN_MAX_COMMENT_DEPTH;
            result["CHAIN_MAX_MEMO_SIZE"] = CHAIN_MAX_MEMO_SIZE;
            result["CHAIN_MAX_WITNESSES"] = CHAIN_MAX_WITNESSES;
            result["CHAIN_MAX_PROXY_RECURSION_DEPTH"] = CHAIN_MAX_PROXY_RECURSION_DEPTH;
            result["CHAIN_MAX_RESERVE_RATIO"] = CHAIN_MAX_RESERVE_RATIO;
            result["CHAIN_MAX_SUPPORT_WITNESSES"] = CHAIN_MAX_SUPPORT_WITNESSES;
            result["CHAIN_MAX_SHARE_SUPPLY"] = CHAIN_MAX_SHARE_SUPPLY;
            result["CHAIN_MAX_SIG_CHECK_DEPTH"] = CHAIN_MAX_SIG_CHECK_DEPTH;
            result["CHAIN_MAX_TIME_UNTIL_EXPIRATION"] = CHAIN_MAX_TIME_UNTIL_EXPIRATION;
            result["CHAIN_MAX_TRANSACTION_SIZE"] = CHAIN_MAX_TRANSACTION_SIZE;
            result["CHAIN_MAX_UNDO_HISTORY"] = CHAIN_MAX_UNDO_HISTORY;
            result["CHAIN_MAX_VOTE_CHANGES"] = CHAIN_MAX_VOTE_CHANGES;
            result["CHAIN_MAX_TOP_WITNESSES"] = CHAIN_MAX_TOP_WITNESSES;
            result["CHAIN_MAX_WITHDRAW_ROUTES"] = CHAIN_MAX_WITHDRAW_ROUTES;
            result["CHAIN_MAX_WITNESS_URL_LENGTH"] = CHAIN_MAX_WITNESS_URL_LENGTH;
            result["CHAIN_MIN_ACCOUNT_CREATION_FEE"] = CHAIN_MIN_ACCOUNT_CREATION_FEE;
            result["CHAIN_MIN_ACCOUNT_NAME_LENGTH"] = CHAIN_MIN_ACCOUNT_NAME_LENGTH;
            result["CHAIN_MIN_BLOCK_SIZE_LIMIT"] = CHAIN_MIN_BLOCK_SIZE_LIMIT;
            result["CHAIN_MAX_BLOCK_SIZE_LIMIT"] = CHAIN_MAX_BLOCK_SIZE_LIMIT;
            result["CHAIN_NULL_ACCOUNT"] = CHAIN_NULL_ACCOUNT;
            result["CHAIN_NUM_INITIATORS"] = CHAIN_NUM_INITIATORS;
            result["CHAIN_PROXY_TO_SELF_ACCOUNT"] = CHAIN_PROXY_TO_SELF_ACCOUNT;
            result["CHAIN_SECONDS_PER_YEAR"] = CHAIN_SECONDS_PER_YEAR;
            result["CHAIN_VESTING_WITHDRAW_INTERVALS"] = CHAIN_VESTING_WITHDRAW_INTERVALS;
            result["CHAIN_VESTING_WITHDRAW_INTERVAL_SECONDS"] = CHAIN_VESTING_WITHDRAW_INTERVAL_SECONDS;
            result["CHAIN_ENERGY_REGENERATION_SECONDS"] = CHAIN_ENERGY_REGENERATION_SECONDS;
            result["TOKEN_SYMBOL"] = TOKEN_SYMBOL;
            result["SHARES_SYMBOL"] = SHARES_SYMBOL;
            result["CHAIN_NAME"] = CHAIN_NAME;

            return result;
        }

    }
} // graphene::protocol