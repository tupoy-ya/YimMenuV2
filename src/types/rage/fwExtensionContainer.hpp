#pragma once

#include "fwExtension.hpp"

namespace rage
{
	class fwExtensionContainer
	{
	public:
		fwExtension* m_Entry;         //0x0000
		fwExtensionContainer* m_Next; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(fwExtensionContainer) == 0x10);
}