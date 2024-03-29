/*
 * Copyright 2011-2023 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/base/blob/master/LICENSE
 */

#ifndef BASE_SETTINGS_H_HEADER_GUARD
#define BASE_SETTINGS_H_HEADER_GUARD

#include "allocator.h"
#include "readerwriter.h"
#include "string.h"

namespace base
{
	///
	class Settings
	{
	public:
		///
		Settings(AllocatorI* _allocator, const void* _data = NULL, uint32_t _len = 0);

		///
		~Settings();

		///
		void clear();

		///
		void load(const void* _data, uint32_t _len);

		///
		StringView get(const StringView& _name) const;

		///
		void set(const StringView& _name, const StringView& _value = "");

		///
		void remove(const StringView& _name) const;

		///
		int32_t read(ReaderSeekerI* _reader, Error* _err);

		///
		int32_t write(WriterI* _writer, Error* _err) const;

	private:
		Settings();

		AllocatorI* m_allocator;
		void* m_ini;
	};

	///
	int32_t read(ReaderSeekerI* _reader, Settings& _settings, Error* _err);

	///
	int32_t write(WriterI* _writer, const Settings& _settings, Error* _err);

} // namespace base

#endif // BASE_SETTINGS_H_HEADER_GUARD
