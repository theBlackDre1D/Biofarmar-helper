#include "memory_routines.h"

namespace structures {
	byte & MemoryRoutines::byteSet(byte & B)
	{
		return B = MAX_BYTE;
	}
	byte & MemoryRoutines::byteReset(byte & B)
	{
		return B = 0;
	}
	byte & MemoryRoutines::byteXOR(byte & B)
	{
		return B ^= MAX_BYTE;
	}
	byte & MemoryRoutines::byteSHL(byte & B)
	{
		return B <<= 1;
	}
	byte & MemoryRoutines::byteSHR(byte & B)
	{
		return B >>= 1;
	}
	bool MemoryRoutines::byteNthBitGet(byte & B, int n)
	{
		return (B & (1 << n)) > 0;
	}

	byte & MemoryRoutines::byteNthBitTo0(byte & B, int n)
	{
		return B &= ~(1 << n);
	}

	byte & MemoryRoutines::byteNthBitTo1(byte & B, int n)
	{
		return B |= (1 << n);
	}

	byte & MemoryRoutines::byteNthBitToggle(byte & B, int n)
	{
		return B ^= (1 << n);
	}

	MemoryRoutines::MemoryRoutines()
	{
	}

}
