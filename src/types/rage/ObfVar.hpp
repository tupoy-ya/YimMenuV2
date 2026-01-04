#pragma once
#include <cstdint>
#include <ctime>

namespace rage
{
	class Obf32
	{
	private:
		uint32_t m_unk1[2];
		uint32_t m_unk3;
		uint32_t m_unk4;

	public:
		uint32_t getData()
		{
			auto v105 = m_unk4;
			auto v28 = m_unk1[0] & v105;
			auto v94 = m_unk1[1] & ~v105;
			return v28 | v94;
		}

		operator uint32_t()
		{
			return getData();
		}

		void setData(uint32_t val)
		{
			auto seed = time(nullptr);
			m_unk3 = seed;
			seed = time(nullptr);
			m_unk4 = seed;

			m_unk1[0] = val & seed;
			m_unk1[1] = val & ~seed;
		}

		void operator=(uint32_t val)
		{
			setData(val);
		}
	};

	class Obf64
	{
	private:
		uint32_t m_unk1[4];
		uint32_t m_unk3;
		uint32_t m_unk4;

	public:
		uint64_t getData()
		{
			uint32_t lower_bits = m_unk1[2] & ~m_unk4 | m_unk1[0] & m_unk4;
			uint64_t upper_bits = m_unk1[3] & ~m_unk4 | m_unk1[1] & m_unk4;

			return lower_bits | (upper_bits << 32);
		}

		operator uint64_t()
		{
			return getData();
		}

		void setData(uint64_t val)
		{
			uint32_t lower_bits = (uint32_t)val;
			uint32_t upper_bits = (uint32_t)(val >> 32);

			auto seed = time(nullptr);
			m_unk3 = seed;
			seed = time(nullptr);
			m_unk4 = seed;

			m_unk1[0] = lower_bits & seed;
			m_unk1[1] = upper_bits & seed;
			m_unk1[2] = lower_bits & ~seed;
			m_unk1[3] = upper_bits & ~seed;
		}

		void operator=(uint32_t val)
		{
			setData(val);
		}
	};
}