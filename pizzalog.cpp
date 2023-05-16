#include "pizzalog.hpp"

namespace log {
  void pizzalog::log(name contract, name event, std::vector<std::string> args, uint64_t millis) {
    require_auth(contract);

    subscription_tlb subscriptions(_self, contract.value);
    for (auto itr = subscriptions.begin(); itr != subscriptions.end(); itr++) {
      if (itr->event == ALLEVENT || itr->event == event) {
        require_recipient(itr->subscriber);
      }
    }
  };

  void pizzalog::subscribe(name subscriber, name contract, name event) {
    require_auth(_self);

    subscription_tlb subscriptions(_self, contract.value);
    for (auto itr = subscriptions.begin(); itr != subscriptions.end(); itr++) {
      if (itr->subscriber == subscriber && (itr->event == ALLEVENT || itr->event == event)) {
        check(false, "already subscribe this event");
      }
    }

    subscriptions.emplace(_self, [&](auto& row) {
      row.id = subscriptions.available_primary_key();
      row.subscriber = subscriber;
      row.event = event;
    });
  };

  void pizzalog::unsubscribe(name subscriber, name contract, name event) {
    require_auth(_self);

    subscription_tlb subscriptions(_self, contract.value);
    auto itr = subscriptions.begin();
    while (itr != subscriptions.end()) {
      if (itr->subscriber == subscriber) {
        if (event == ALLEVENT) {
          itr = subscriptions.erase(itr);
          continue;
        }
        if (event == itr->event) {
          itr = subscriptions.erase(itr);
          break;
        }
      }
      itr++;
    }
  };
}
