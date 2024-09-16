#pragma once

#include "ll/api/mod/NativeMod.h"
#include "ll/api/event/ListenerBase.h"

#include "Config.h"

namespace fart {

Config config;

class Fart {

public:
    static Fart& getInstance();

    Fart(ll::mod::NativeMod& self) : mSelf(self) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();

    bool enable();

    bool disable();

private:
    ll::mod::NativeMod& mSelf;

    ll::event::ListenerPtr mPlayerSneakingListener;
};

} // namespace fart
