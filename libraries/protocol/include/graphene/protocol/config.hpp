#pragma once

#define CHAIN_VERSION              (version(1, 0, 0))
#define CHAIN_HARDFORK_VERSION     (hardfork_version(CHAIN_VERSION))

#define CHAIN_NAME                         "VIZ"
#define CHAIN_ID                        (fc::sha256::hash(CHAIN_NAME))
#define CHAIN_ADDRESS_PREFIX                  "VIZ"

#define SHARES_SYMBOL  (uint64_t(6) | (uint64_t('S') << 8) | (uint64_t('H') << 16) | (uint64_t('A') << 24) | (uint64_t('R') << 32) | (uint64_t('E') << 40) | (uint64_t('S') << 48))
#define TOKEN_SYMBOL  (uint64_t(3) | (uint64_t('V') << 8) | (uint64_t('I') << 16) | (uint64_t('Z') << 24))

#define CHAIN_GENESIS_TIME                    (fc::time_point_sec(1476788400))
#define CHAIN_CASHOUT_WINDOW_SECONDS          (60*60*24)  // 1 day

#define CHAIN_MAX_PROPOSAL_LIFETIME_SEC       (60*60*24*7*4) /// 4 weeks
#define CHAIN_MAX_PROPOSAL_DEPTH              (2)

#define CHAIN_MIN_ACCOUNT_CREATION_FEE        1000

#define CHAIN_OWNER_AUTH_RECOVERY_PERIOD                  fc::days(30)
#define CHAIN_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::days(1)
#define CHAIN_OWNER_UPDATE_LIMIT                          fc::minutes(60)

#define CHAIN_BLOCK_INTERVAL                  1
#define CHAIN_BLOCK_WITNESS_REPEAT            3
#define CHAIN_BLOCKS_PER_YEAR                 (365*24*60*60/CHAIN_BLOCK_INTERVAL)
#define CHAIN_BLOCKS_PER_DAY                  (24*60*60/CHAIN_BLOCK_INTERVAL)
#define CHAIN_BLOCKS_PER_HOUR                 (60*60/CHAIN_BLOCK_INTERVAL)
#define CHAIN_MAX_WITNESS_MISSED_BLOCKS       200

#define CHAIN_INITIATOR_NAME                 "viz"
#define CHAIN_INITIATOR_PUBLIC_KEY_STR       "VIZ6MyX5QiXAXRZk7SYCiqpi6Mtm8UbHWDFSV8HPpt7FJyahCnc2T"
#define CHAIN_INITIATOR_PUBLIC_KEY            (graphene::protocol::public_key_type(CHAIN_INITIATOR_PUBLIC_KEY_STR))
#define CHAIN_NUM_INITIATORS                  0
#define CHAIN_MAX_TOP_WITNESSES               10
#define CHAIN_MAX_SUPPORT_WITNESSES           11
#define CHAIN_MAX_WITNESSES                   (CHAIN_MAX_TOP_WITNESSES+CHAIN_MAX_SUPPORT_WITNESSES) /// 21 is more than enough
#define CHAIN_HARDFORK_REQUIRED_WITNESSES     17 // 17 of the 20 dpos witnesses (19 elected and 1 virtual time) required for hardfork. This guarantees 75% participation on all subsequent rounds.
#define CHAIN_MAX_TIME_UNTIL_EXPIRATION       (60*60) // seconds,  aka: 1 hour
#define CHAIN_MAX_MEMO_SIZE                   2048
#define CHAIN_MAX_PROXY_RECURSION_DEPTH       4
#define CHAIN_VESTING_WITHDRAW_INTERVALS      28
#define CHAIN_VESTING_WITHDRAW_INTERVAL_SECONDS (60*60*24) // 1 day per interval
#define CHAIN_MAX_WITHDRAW_ROUTES             10
#define CHAIN_VOTE_REGENERATION_SECONDS       (5*60*60*24) // 5 days
#define CHAIN_MAX_VOTE_CHANGES                5
#define CHAIN_UPVOTE_LOCKOUT                  (fc::minutes(1))
#define CHAIN_MIN_VOTE_INTERVAL_SEC           1
#define CHAIN_MAX_COMMENT_BENEFICIARIES       64
#define CHAIN_VOTE_POWER_RATE					1

#define CHAIN_MIN_ROOT_COMMENT_INTERVAL       (fc::seconds(1))
#define CHAIN_MIN_REPLY_INTERVAL              (fc::seconds(1))

#define CHAIN_MAX_ACCOUNT_WITNESS_VOTES       2

#define CHAIN_100_PERCENT                     10000
#define CHAIN_1_PERCENT                       (CHAIN_100_PERCENT/100)
#define CHAIN_1_TENTH_PERCENT                 (CHAIN_100_PERCENT/1000)

#define CHAIN_FIXED_INFLATION					(1000) //10%
#define CHAIN_CONTENT_REWARD_PERCENT          (30*CHAIN_1_PERCENT) //30% of inflation
#define CHAIN_REWARD_FUND_CURATOR_PERCENT     (5*100/30*CHAIN_1_PERCENT) //5% of inflation from reward fund
#define CHAIN_VESTING_FUND_PERCENT            (40*CHAIN_1_PERCENT) //40% of inflation
#define CHAIN_COMMITTEE_FUND_PERCENT          (15*CHAIN_1_PERCENT) //15% of inflation

#define CHAIN_BANDWIDTH_AVERAGE_WINDOW_SECONDS (60*60*24*7) ///< 1 week
#define CHAIN_BANDWIDTH_PRECISION             1000000ll ///< 1 million
#define CONSENSUS_BANDWIDTH_RESERVE_PERCENT     (CHAIN_100_PERCENT/10) // 10%
#define CONSENSUS_BANDWIDTH_RESERVE_BELOW       int64_t(500000000) // 500 SHARES AMOUNT
#define CHAIN_MAX_COMMENT_DEPTH               0xfff0 // 64k - 16

#define CHAIN_MAX_RESERVE_RATIO   (20000)

#define CHAIN_CREATE_ACCOUNT_DELEGATION_RATIO       10
#define CHAIN_CREATE_ACCOUNT_DELEGATION_TIME        (fc::days(30))
#define CHAIN_MIN_DELEGATION                        1

#define CHAIN_EQUIHASH_N                      140
#define CHAIN_EQUIHASH_K                      6

#define CHAIN_MIN_ACCOUNT_NAME_LENGTH          2
#define CHAIN_CREATE_MIN_ACCOUNT_NAME_LENGTH   3
#define CHAIN_MAX_ACCOUNT_NAME_LENGTH         25

#define CHAIN_MIN_PERMLINK_LENGTH             0
#define CHAIN_MAX_PERMLINK_LENGTH             256
#define CHAIN_MAX_WITNESS_URL_LENGTH          2048

#define CHAIN_INIT_SUPPLY                     int64_t(50000000000)
#define CHAIN_MAX_SHARE_SUPPLY                int64_t(1000000000000000ll)
#define CHAIN_MAX_SIG_CHECK_DEPTH             2

#define CHAIN_SECONDS_PER_YEAR                (uint64_t(60*60*24*365ll))

#define CHAIN_MAX_TRANSACTION_SIZE            (1024*64)
#define CHAIN_MIN_BLOCK_SIZE_LIMIT            (CHAIN_MAX_TRANSACTION_SIZE)
#define CHAIN_MAX_BLOCK_SIZE_LIMIT            (2*1024*1024) // 2Mb p2p graphene limit : /libraries/net/include/graphene/net/config.hpp#L39
#define CHAIN_BLOCK_SIZE                      (CHAIN_MAX_BLOCK_SIZE_LIMIT*CHAIN_BLOCK_INTERVAL) // initial block size, preset to max block size limit

#define CHAIN_MAX_UNDO_HISTORY                10000

#define CHAIN_IRREVERSIBLE_THRESHOLD          (75 * CHAIN_1_PERCENT)
#define COMMITTEE_MIN_DURATION                  (60*60*24*5)
#define COMMITTEE_MAX_DURATION                  (60*60*24*30)
#define COMMITTEE_MAX_REQUIRED_AMOUNT           int64_t(CHAIN_INIT_SUPPLY/100)
#define COMMITTEE_REQUEST_PER_TIME              (60*60*24)
#define COMMITTEE_REQUEST_PROCESSING            (CHAIN_BLOCKS_PER_HOUR/6) //every 10 minutes

/**
 *  Reserved Account IDs with special meaning
 */
///@{
/// Represents the canonical account with NO authority (nobody can access funds in null account)
#define CHAIN_NULL_ACCOUNT                    "null"
/// Represents the canonical account with NO authority (nobody can access funds in committee account, all income transfers going to committee fund)
#define CHAIN_COMMITTEE_ACCOUNT               "committee"
/// Private key 5Hw9YPABaFxa2LooiANLrhUK5TPryy8f7v9Y1rk923PuYqbYdfC (committee viz sign)
#define CHAIN_COMMITTEE_PUBLIC_KEY_STR        "VIZ6Yt7d6LsngBoXQr47aLv97bJVs7jyr7esZTM4UUSpLUf3nbRKS"
#define CHAIN_COMMITTEE_PUBLIC_KEY            (graphene::protocol::public_key_type(CHAIN_COMMITTEE_PUBLIC_KEY_STR))

/// Represents the canonical account with WILDCARD authority (anybody can access funds in temp account)
#define CHAIN_TEMP_ACCOUNT                    "temp"
/// Represents the canonical account with NO authority (nobody can access funds in committee account, all income transfers going to new anonymous sub-account)
#define CHAIN_ANONYMOUS_ACCOUNT                    "anonymous"
/// Represents the canonical account for specifying you will vote for directly (as opposed to a proxy)
#define CHAIN_PROXY_TO_SELF_ACCOUNT           ""
/// Represents the canonical root post parent account
#define CHAIN_ROOT_POST_PARENT                (account_name_type())
///@}
