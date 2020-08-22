#pragma once

#include <appbase/application.hpp>
#include <graphene/plugins/chain/plugin.hpp>
#include <graphene/api/account_api_object.hpp>
#include <graphene/plugins/custom_protocol_api/custom_protocol_api_object.hpp>
#include <graphene/plugins/custom_protocol_api/custom_protocol_api_visitor.hpp>

namespace graphene { namespace plugins { namespace custom_protocol_api {
    using plugins::json_rpc::msg_pack;
    using graphene::api::account_api_object;
    using graphene::plugins::custom_protocol_api::custom_protocol_object;
    using namespace graphene::chain;
    DEFINE_API_ARGS(get_account, msg_pack, account_api_object)

    class custom_protocol_api_plugin final: public appbase::plugin<custom_protocol_api_plugin> {
    public:
        APPBASE_PLUGIN_REQUIRES (
            (chain::plugin)
            (json_rpc::plugin)
        )

        DECLARE_API(
            /**
             * @brief Get accaount by name with custom protocol reference by id
             * @param name Name of the account
             * @param custom_protocol Id of the custom protocol
             * @return The account with custom protocol reference if founded in custom_sequence and custom_sequence_block_num
             */
            (get_account)
        )

        custom_protocol_api_plugin();
        ~custom_protocol_api_plugin();

        void set_program_options(
            boost::program_options::options_description&,
            boost::program_options::options_description& config_file_options
        ) override;

        static const std::string& name();

        void plugin_initialize(const boost::program_options::variables_map& options) override;

        void plugin_startup() override;
        void plugin_shutdown() override;

    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };
} } } // graphene::plugins::custom_protocol_api