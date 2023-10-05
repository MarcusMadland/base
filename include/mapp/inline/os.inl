/*
 * Copyright 2010-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/mapp/blob/master/LICENSE
 */

#ifndef BX_H_HEADER_GUARD
#	error "Must be included from mapp/os.h!"
#endif // BX_H_HEADER_GUARD

namespace bx
{
	template<typename ProtoT>
	inline ProtoT dlsym(void* _handle, const StringView& _symbol)
	{
		return reinterpret_cast<ProtoT>(dlsym(_handle, _symbol) );
	}

} // namespace bx
