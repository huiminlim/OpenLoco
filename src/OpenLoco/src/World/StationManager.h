#pragma once

#include "Engine/Limits.h"
#include "Station.h"
#include <OpenLoco/Core/LocoFixedVector.hpp>
#include <array>
#include <cstddef>
#include <vector>

namespace OpenLoco::StationManager
{
    void reset();
    FixedVector<Station, Limits::kMaxStations> stations();
    Station* get(StationId id);
    void update();
    void updateLabels();
    void updateDaily();
    void sub_437F29(CompanyId cid, uint8_t arg1);
    StringId generateNewStationName(StationId stationId, TownId townId, World::Pos3 position, uint8_t mode);
    void zeroUnused();
    void registerHooks();
    uint16_t deliverCargoToNearbyStations(const uint8_t cargoType, const uint8_t cargoQty, const World::Pos2& pos, const World::TilePos2& size);
    uint16_t deliverCargoToStations(const std::vector<StationId>& stations, const uint8_t cargoType, const uint8_t cargoQty);
}
