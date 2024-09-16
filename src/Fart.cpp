#include "Fart.h"

#include <memory>
#include <random>

#include "ll/api/Config.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/event/player/PlayerSneakEvent.h"
#include "ll/api/mod/RegisterHelper.h"

#include "mc/enums/FertilizerType.h"
#include "mc/world/actor/player/Player.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/block/Block.h"
#include "mc/world/level/block/CropBlock.h"
#include "mc/world/level/dimension/Dimension.h"

#include "ParticleAPI.h"

namespace fart {

static std::unique_ptr<Fart> instance;

Fart& Fart::getInstance() { return *instance; }

bool Fart::load() {
    // 读取配置文件
    const auto& configFilePath = getSelf().getConfigDir() / "config.json";
    if (!ll::config::loadConfig(config, configFilePath)) {
        getSelf().getLogger().warn("Cannot load configurations from {}", configFilePath);
        getSelf().getLogger().info("Saving default configurations");

        if (!ll::config::saveConfig(config, configFilePath)) {
            getSelf().getLogger().error("Cannot save default configurations to {}", configFilePath);
        }
    }

    return true;
}

bool Fart::enable() {
    auto& eventBus = ll::event::EventBus::getInstance();

    // 注册玩家潜行事件
    mPlayerSneakingListener = eventBus.emplaceListener<ll::event::player::PlayerSneakingEvent>([this](auto& event) {
        auto& player   = event.self();
        auto  blockPos = player.getFeetBlockPos();

        std::random_device                    rd;
        std::default_random_engine            eng(rd());
        std::uniform_real_distribution<float> distr(0, 1);

        int minDis = static_cast<int>(config.minFartDistance);
        int maxDis = static_cast<int>(config.maxFartDistance + 0.5);

        for (int i = -maxDis; i <= maxDis; i++) {
            for (int j = -maxDis; j <= maxDis; j++) {
                for (int k = -maxDis; k <= maxDis; k++) {
                    auto  tmpPos      = blockPos.add({i, j, k});
                    auto  distance    = blockPos.distanceTo(tmpPos);
                    auto& blockSource = player.getDimension().getBlockSourceFromMainChunkSource();
                    if (distance > maxDis || distance < minDis) {
                        continue;
                    }
                    auto& block = blockSource.getBlock(tmpPos);
                    if (block.canBeFertilized(blockSource, tmpPos, block)) {
                        // 生成粒子效果
                        for (int x = 1; x <= config.particleCount; x++) {
                            PTAPI_spawnParticle(
                                UINT_MAX,
                                Vec3(tmpPos.x + distr(eng), tmpPos.y + distr(eng), tmpPos.z + distr(eng)),
                                "minecraft:villager_happy",
                                player.getDimensionId()
                            );
                        }

                        // 判断概率催熟
                        if (distr(eng) <= config.successRate) {
                            block.onFertilized(blockSource, tmpPos, &player, FertilizerType());
                        }
                    }
                }
            }
        }
    });

    return true;
}

bool Fart::disable() {
    // 卸载玩家事件
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.removeListener(mPlayerSneakingListener);

    return true;
}

} // namespace fart

LL_REGISTER_MOD(fart::Fart, fart::instance);
