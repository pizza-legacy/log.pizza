#include <eosio/eosio.hpp>

using namespace eosio;

#define ALLEVENT name("all")

namespace log {
  class [[eosio::contract]] pizzalog : public contract {
  public:
    using contract::contract;

    [[eosio::action]]
    void log(name contract, name event, std::vector<std::string> args, uint64_t millis);

    [[eosio::action]]
    void subscribe(name subscriber, name contract, name event);

    [[eosio::action]]
    void unsubscribe(name subscriber, name contract, name event);

  private:
    struct [[eosio::table]] subscription{
      uint64_t id;
      name event;
      name subscriber;

      uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<name("subscription"), subscription> subscription_tlb;
  };
}