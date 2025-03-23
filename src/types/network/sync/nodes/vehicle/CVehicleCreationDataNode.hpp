#pragma once
#include <cstdint>
#include "types/network/sync/CProjectBaseSyncDataNode.hpp"

class CVehicleCreationDataNode : CProjectBaseSyncDataNode
{
public:
    uint32_t m_PopType; //0x00C0
    uint32_t m_RandomSeed; //0x00C4
    uint32_t m_ModelHash; //0x00C8
    uint32_t m_VehicleStatus; //0x00CC
    uint32_t m_MaxHealth; //0x00D0
    uint32_t m_CreationToken; //0x00D4
    bool m_CarBudget; //0x00D8
    bool m_NeedsToBeHotwired; //0x00D9
    bool m_TiresDontBurst; //0x00DA
    char pad_00DB[165]; //0x00DB
}; //Size: 0x0180
static_assert(sizeof(CVehicleCreationDataNode) == 0x180);
