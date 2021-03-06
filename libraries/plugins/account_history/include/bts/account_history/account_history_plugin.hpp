#pragma once

#include <bts/app/plugin.hpp>
#include <bts/chain/database.hpp>

#include <fc/thread/future.hpp>

namespace bts { namespace account_history {
using namespace chain;
namespace bpo = boost::program_options;

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
#ifndef ACCOUNT_HISTORY_SPACE_ID
#define ACCOUNT_HISTORY_SPACE_ID 5
#endif

enum account_history_object_type
{
   key_account_object_type
};

class key_account_object : public abstract_object<key_account_object>
{
   public:
      static const uint8_t space_id = ACCOUNT_HISTORY_SPACE_ID;
      static const uint8_t type_id  = key_account_object_type;

      key_account_object() {}
      key_account_object( const address& a ) : key(a) {}

      address                   key;
      flat_set<account_id_type> account_ids;
};

struct by_key{};
typedef multi_index_container<
   key_account_object,
   indexed_by<
      hashed_unique< tag<by_id>, member< object, object_id_type, &object::id > >,
      ordered_unique< tag<by_key>, member< key_account_object, address, &key_account_object::key > >
   >
> key_account_object_multi_index_type;

typedef generic_index<key_account_object, key_account_object_multi_index_type> key_account_index;

namespace detail
{
    class account_history_plugin_impl;
}

class account_history_plugin : public bts::app::plugin
{
   public:
      account_history_plugin();
      virtual ~account_history_plugin();

      std::string plugin_name()const override;
      virtual void plugin_set_program_options(bpo::options_description& cli, bpo::options_description& cfg) override;
      virtual void plugin_initialize(const bpo::variables_map& options) override;
      virtual void plugin_startup() override;

      flat_set<account_id_type> tracked_accounts()const;

      friend class detail::account_history_plugin_impl;
      std::unique_ptr<detail::account_history_plugin_impl> my;
};

} } //bts::account_history

FC_REFLECT_DERIVED( bts::account_history::key_account_object,
            (bts::db::object),
            (key)
            (account_ids)
          )
