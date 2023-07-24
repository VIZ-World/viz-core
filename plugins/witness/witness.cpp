
#include <graphene/plugins/witness/witness.hpp>

#include <graphene/chain/database_exceptions.hpp>
#include <graphene/chain/account_object.hpp>
#include <graphene/chain/chain_objects.hpp>
#include <graphene/chain/chain_object_types.hpp>
#include <graphene/chain/witness_objects.hpp>
#include <graphene/time/time.hpp>

#include <graphene/utilities/key_conversion.hpp>

#include <fc/smart_ref_impl.hpp>

#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using std::string;
using std::vector;

namespace bpo = boost::program_options;

void new_chain_banner(const graphene::chain::database &db) {
    std::cerr << "\n"
            "********************************\n"
            "*                              *\n"
            "*   ------- NEW CHAIN ------   *\n"
            "*   -    Welcome to VIZ!   -   *\n"
            "*   ------------------------   *\n"
            "*                              *\n"
            "********************************\n"
            "\n";
    return;
}

template<typename T>
T dejsonify(const string &s) {
    return fc::json::from_string(s).as<T>();
}

#define DEFAULT_VALUE_VECTOR(value) default_value({fc::json::to_string(value)}, fc::json::to_string(value))
#define LOAD_VALUE_SET(options, name, container, type) \
            if( options.count(name) ) { \
                  const std::vector<std::string>& ops = options[name].as<std::vector<std::string>>(); \
                  std::transform(ops.begin(), ops.end(), std::inserter(container, container.end()), &dejsonify<type>); \
            }

namespace graphene {
    namespace plugins {
        namespace witness_plugin {

            namespace asio = boost::asio;
            namespace posix_time = boost::posix_time;
            namespace system = boost::system;

            struct witness_plugin::impl final {
                impl():
                    p2p_(appbase::app().get_plugin<graphene::plugins::p2p::p2p_plugin>()),
                    chain_(appbase::app().get_plugin<graphene::plugins::chain::plugin>()),
                    production_timer_(appbase::app().get_io_service()) {
                }

                ~impl(){}

                graphene::chain::database& database() {
                    return chain_.db();
                }

                graphene::chain::database& database() const {
                    return chain_.db();
                }

                graphene::plugins::chain::plugin& chain() {
                    return chain_;
                }

                graphene::plugins::chain::plugin& chain() const {
                    return chain_;
                }

                graphene::plugins::p2p::p2p_plugin& p2p(){
                    return p2p_;
                };

                graphene::plugins::p2p::p2p_plugin& p2p() const {
                    return p2p_;
                };

                graphene::plugins::p2p::p2p_plugin& p2p_;

                graphene::plugins::chain::plugin& chain_;

                void schedule_production_loop();

                block_production_condition::block_production_condition_enum block_production_loop();

                block_production_condition::block_production_condition_enum maybe_produce_block(fc::mutable_variant_object &capture);

                boost::program_options::variables_map _options;
                uint32_t _required_witness_participation = 33 * CHAIN_1_PERCENT;

                std::atomic<uint64_t> head_block_num_;
                block_id_type head_block_id_ = block_id_type();
                std::atomic<uint64_t> total_hashes_;
                fc::time_point hash_start_time_;

                uint32_t _production_skip_flags = graphene::chain::database::skip_nothing;
                bool _production_enabled = false;
                asio::deadline_timer production_timer_;

                std::map<public_key_type, fc::ecc::private_key> _private_keys;
                std::set<string> _witnesses;

                fc::time_point last_block_post_validation_time;
            };

            void witness_plugin::set_program_options(
                    boost::program_options::options_description &command_line_options,
                    boost::program_options::options_description &config_file_options) {
                    string witness_id_example = "initwitness";

                command_line_options.add_options()
                        ("enable-stale-production", bpo::value<bool>()->implicit_value(false) , "Enable block production, even if the chain is stale.")
                        ("required-participation", bpo::value<int>()->implicit_value(uint32_t(3 * CHAIN_1_PERCENT)), "Percent of witnesses (0-99) that must be participating in order to produce blocks")
                        ("witness,w", bpo::value<vector<string>>()->composing()->multitoken(), ("name of witness controlled by this node (e.g. " + witness_id_example + " )").c_str())
                        ("private-key", bpo::value<vector<string>>()->composing()->multitoken(), "WIF PRIVATE KEY to be used by one or more witnesses")
                        ;

                config_file_options.add(command_line_options);
            }

            using std::vector;
            using std::pair;
            using std::string;

            void witness_plugin::plugin_initialize(const boost::program_options::variables_map &options) {
                try {
                    ilog("witness plugin:  plugin_initialize() begin");
                    pimpl = std::make_unique<witness_plugin::impl>();

                    pimpl->total_hashes_.store(0, std::memory_order_relaxed);
                    pimpl->_options = &options;
                    LOAD_VALUE_SET(options, "witness", pimpl->_witnesses, string)
                    edump((pimpl->_witnesses));

                    if(options.count("enable-stale-production")){
                        pimpl->_production_enabled = options["enable-stale-production"].as<bool>();
                    }

                    if(options.count("required-participation")){
                        int e = static_cast<int>(options["required-participation"].as<int>());
                        pimpl->_required_witness_participation = uint32_t(e * CHAIN_1_PERCENT);
                    }

                    if (options.count("private-key")) {
                        const std::vector<std::string> keys = options["private-key"].as<std::vector<std::string>>();
                        for (const std::string &wif_key : keys) {
                            fc::optional<fc::ecc::private_key> private_key = graphene::utilities::wif_to_key(wif_key);
                            FC_ASSERT(private_key.valid(), "unable to parse private key");
                            pimpl->_private_keys[private_key->get_public_key()] = *private_key;
                        }
                    }

                    ilog("witness plugin:  plugin_initialize() end");
                } FC_LOG_AND_RETHROW()
            }

            void witness_plugin::plugin_startup() {
                try {
                    ilog("witness plugin:  plugin_startup() begin");
                    auto &d = pimpl->database();
                    //Start NTP time client
                    graphene::time::now();

                    if (!pimpl->_witnesses.empty()) {
                        ilog("Launching block production for ${n} witnesses.", ("n", pimpl->_witnesses.size()));
                        pimpl->p2p().set_block_production(true);
                        if (pimpl->_production_enabled) {
                            if (d.head_block_num() == 0) {
                                new_chain_banner(d);
                            }
                            pimpl->_production_skip_flags |= graphene::chain::database::skip_undo_history_check;
                        }
                        pimpl->schedule_production_loop();
                    } else
                        elog("No witnesses configured! Please add witness names and private keys to configuration.");
                    ilog("witness plugin:  plugin_startup() end");
                } FC_CAPTURE_AND_RETHROW()
            }

            void witness_plugin::plugin_shutdown() {
                graphene::time::shutdown_ntp_time();
                if (!pimpl->_witnesses.empty()) {
                    ilog("shutting downing production timer");
                    pimpl->production_timer_.cancel();
                }
            }

            witness_plugin::witness_plugin() {}

            witness_plugin::~witness_plugin() {}

            void witness_plugin::impl::schedule_production_loop() {
                //Schedule for the next second's tick regardless of chain state
                // If we would wait less than 50ms, wait for the whole second.
                int64_t ntp_microseconds = graphene::time::now().time_since_epoch().count();
                int64_t next_microseconds = 1000000 - ( ntp_microseconds % 1000000 );
                if (next_microseconds < 50000) { // we must sleep for at least 50ms
                    next_microseconds += 1000000 ;
                }

                production_timer_.expires_from_now( posix_time::microseconds(next_microseconds) );
                production_timer_.async_wait( [this](const system::error_code &) { block_production_loop(); } );
            }

            block_production_condition::block_production_condition_enum witness_plugin::impl::block_production_loop() {
                block_production_condition::block_production_condition_enum result;
                fc::mutable_variant_object capture;
                try {
                    result = maybe_produce_block(capture);
                }
                catch (const fc::canceled_exception &) {
                    //We're trying to exit. Go ahead and let this one out.
                    throw;
                }
                catch (const graphene::chain::unknown_hardfork_exception &e) {
                    // Hit a hardfork that the current node know nothing about, stop production and inform user
                    elog("${e}\nNode may be out of date...", ("e", e.to_detail_string()));
                    throw;
                }
                catch (const fc::exception &e) {
                    elog("Got exception while generating block:\n${e}", ("e", e.to_detail_string()));
                    result = block_production_condition::exception_producing_block;
                }

                switch (result) {
                    case block_production_condition::produced:
                        ilog("Generated block #${n} with timestamp ${t} at time ${c} by ${w}", (capture));
                        break;
                    case block_production_condition::not_synced:
                        // This log-record is commented, because it outputs very often
                        // ilog("Not producing block because production is disabled until we receive a recent block (see: --enable-stale-production)");
                        break;
                    case block_production_condition::not_my_turn:
                        // This log-record is commented, because it outputs very often
                        // ilog("Not producing block because it isn't my turn");
                        break;
                    case block_production_condition::not_time_yet:
                        // This log-record is commented, because it outputs very often
                        // ilog("Not producing block because slot has not yet arrived");
                        break;
                    case block_production_condition::no_private_key:
                        ilog("Not producing block for ${scheduled_witness} because I don't have the private key for ${scheduled_key}",
                             (capture));
                        break;
                    case block_production_condition::low_participation:
                        elog("Not producing block because node appears to be on a minority fork with only ${pct}% witness participation",
                             (capture));
                        break;
                    case block_production_condition::lag:
                        elog("Not producing block because node didn't wake up within 500ms of the slot time.");
                        break;
                    case block_production_condition::consecutive:
                        elog("Not producing block because the last block was generated by the same witness.\nThis node is probably disconnected from the network so block production has been disabled.\nDisable this check with --allow-consecutive option.");
                        break;
                    case block_production_condition::exception_producing_block:
                        elog("Failure when producing block with no transactions");
                        break;
                }

                schedule_production_loop();
                return result;
            }

            block_production_condition::block_production_condition_enum witness_plugin::impl::maybe_produce_block(fc::mutable_variant_object &capture) {
                auto &db = database();
                fc::time_point now_fine = graphene::time::now();
                fc::time_point_sec now = now_fine + fc::microseconds( 500000 );

                // If the next block production opportunity is in the present or future, we're synced.
                if (!_production_enabled) {
                    if (db.get_slot_time(1) >= now) {
                        _production_enabled = true;
                    } else {
                        return block_production_condition::not_synced;
                    }
                }

                //try get block post validation list for each witness
                //if witness can validate it, sign chain_id and block_id for message
                //broadcast validation message by p2p plugin
                if(last_block_post_validation_time < now_fine ){
                    last_block_post_validation_time = now;
                    //ilog("! tick last_block_post_validation_time");
                    //get block post validation for each witness we have
                    for (auto &witness_account : _witnesses) {
                        bool ignore_witness = false;
                        auto block_post_validations = db.get_block_post_validations(witness_account);
                        if (block_post_validations.size() > 0) {
                            const auto &witness_by_name = db.get_index<graphene::chain::witness_index>().indices().get<graphene::chain::by_name>();
                            auto w_itr = witness_by_name.find(witness_account);
                            graphene::protocol::public_key_type witness_pub_key = w_itr->signing_key;
                            auto private_key_itr = _private_keys.find(witness_pub_key);

                            if (private_key_itr == _private_keys.end()) {
                                ilog("No private key to public ${p} for ${w}", ("p", witness_pub_key)("w", witness_account));
                                ignore_witness= true;
                            }
                            if(!ignore_witness){
                                graphene::protocol::private_key_type witness_priv_key = private_key_itr->second;
                                //we have block post validations for this witness
                                //check if we have a block
                                for(uint8_t i = 0; i < block_post_validations.size(); i++) {
                                    if(0 != block_post_validations[i].block_num){
                                        if(block_post_validations[i].block_id != block_id_type()){
                                            graphene::protocol::digest_type::encoder enc;
                                            fc::raw::pack(enc, db.get_chain_id().str().append(block_post_validations[i].block_id.str()));
                                            //sign the enc by witness_priv_key
                                            graphene::protocol::signature_type bpv_signature = witness_priv_key.sign_compact(enc.result());
                                            //ilog("Witness ${w} signed block post validation #${n} ${b} with signature ${s}", ("w", witness_account)("n", block_post_validations[i].block_num)("b", block_post_validations[i].block_id)("s", bpv_signature));
                                            p2p().broadcast_block_post_validation(block_post_validations[i].block_id, witness_account, bpv_signature);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // is anyone scheduled to produce now or one second in the future?
                uint32_t slot = db.get_slot_at_time(now);
                if (slot == 0) {
                    capture("next_time", db.get_slot_time(1));
                    return block_production_condition::not_time_yet;
                }

                //
                // this assert should not fail, because now <= db.head_block_time()
                // should have resulted in slot == 0.
                //
                // if this assert triggers, there is a serious bug in get_slot_at_time()
                // which would result in allowing a later block to have a timestamp
                // less than or equal to the previous block
                //
                assert(now > db.head_block_time());

                string scheduled_witness = db.get_scheduled_witness(slot);
                // we must control the witness scheduled to produce the next block.
                if (_witnesses.find(scheduled_witness) == _witnesses.end()) {
                    capture("scheduled_witness", scheduled_witness);
                    return block_production_condition::not_my_turn;
                }

                const auto &witness_by_name = db.get_index<graphene::chain::witness_index>().indices().get<graphene::chain::by_name>();
                auto itr = witness_by_name.find(scheduled_witness);

                fc::time_point_sec scheduled_time = db.get_slot_time(slot);
                graphene::protocol::public_key_type scheduled_key = itr->signing_key;
                auto private_key_itr = _private_keys.find(scheduled_key);

                if (private_key_itr == _private_keys.end()) {
                    capture("scheduled_witness", scheduled_witness);
                    capture("scheduled_key", scheduled_key);
                    return block_production_condition::no_private_key;
                }

                uint32_t prate = db.witness_participation_rate();
                if (prate < _required_witness_participation) {
                    capture("pct", uint32_t(100 * uint64_t(prate) / CHAIN_1_PERCENT));
                    return block_production_condition::low_participation;
                }

                if (llabs((scheduled_time - now).count()) > fc::milliseconds(500).count()) {
                    capture("scheduled_time", scheduled_time)("now", now);
                    return block_production_condition::lag;
                }

                int retry = 0;
                do {
                    try {
                        // TODO: the same thread as used in chain-plugin,
                        //       but in the future it should refactored to calling of a chain-plugin function
                        auto block = db.generate_block(
                                scheduled_time,
                                scheduled_witness,
                                private_key_itr->second,
                                _production_skip_flags
                        );
                        capture("n", block.block_num())("t", block.timestamp)("c", now)("w", scheduled_witness);
                        p2p().broadcast_block(block);

                        return block_production_condition::produced;
                    }
                    catch (fc::exception &e) {
                        elog("${e}", ("e", e.to_detail_string()));
                        elog("Clearing pending transactions and attempting again");
                        db.clear_pending();
                        retry++;
                    }
                } while (retry < 2);

                return block_production_condition::exception_producing_block;
            }
        }
    }
}
