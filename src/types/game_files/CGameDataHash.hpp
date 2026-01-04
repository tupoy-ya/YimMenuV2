#pragma once
#include "types/rage/ObfVar.hpp"
#include <array>

#pragma pack(push, 8)
class CGameDataHash
{
public:
	bool m_IsJapaneseVersion;
	std::array<rage::Obf32, 16> m_Data;
	rage::Obf64 m_GameSkeletonHash;
};
static_assert(sizeof(CGameDataHash) == 0x11C);
#pragma pack(pop)
