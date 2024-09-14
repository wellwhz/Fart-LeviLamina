#include "Fart.h"

#include <memory>

#include "ll/api/mod/RegisterHelper.h"

namespace fart {

static std::unique_ptr<Fart> instance;

Fart& Fart::getInstance() { return *instance; }

bool Fart::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool Fart::enable() {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool Fart::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace fart

LL_REGISTER_MOD(fart::Fart, fart::instance);
