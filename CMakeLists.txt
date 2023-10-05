# Sources
file(
	GLOB_RECURSE
	MAPP_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/include/**.h #
	${CMAKE_CURRENT_SOURCE_DIR}/include/**.inl #
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp #
)

# Combine the sources
set(ALL_SOURCES ${MAPP_SOURCES} ${PLATFORM_SOURCES})

# Solution Filters
foreach(_source IN ITEMS ${ALL_SOURCES})
	if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

# Exclude files from the build but keep them in project
file(GLOB_RECURSE MAPP_NOBUILD "${CMAKE_CURRENT_SOURCE_DIR}/src/amalgamated.*")
foreach(MAPP_SRC ${MAPP_NOBUILD})
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR} PROPERTIES HEADER_FILE_ONLY ON)
endforeach()

add_library(mapp STATIC ${ALL_SOURCES})

# Build system specific configurations
if(MINGW)
	set(MAPP_COMPAT_PLATFORM mingw)
elseif(WIN32)
	set(MAPP_COMPAT_PLATFORM msvc)
elseif(APPLE) # APPLE is technically UNIX... ORDERING MATTERS!
	set(MAPP_COMPAT_PLATFORM osx)
elseif(UNIX)
	set(MAPP_COMPAT_PLATFORM linux)
endif()

# Includes
target_include_directories(
	mapp
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include #
	${CMAKE_CURRENT_SOURCE_DIR}/include/compat/${MAPP_COMPAT_PLATFORM} #
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty #
)

# All configurations
target_compile_definitions(mapp PUBLIC "BX_CONFIG_DEBUG=$<IF:$<CONFIG:Debug>,1,$<BOOL:${BX_CONFIG_DEBUG}>>")
target_compile_definitions(mapp PUBLIC "__STDC_LIMIT_MACROS")
target_compile_definitions(mapp PUBLIC "__STDC_FORMAT_MACROS")
target_compile_definitions(mapp PUBLIC "__STDC_CONSTANT_MACROS")

# Special Visual Studio Flags
if(MSVC)
	target_compile_definitions(mapp PUBLIC "_CRT_SECURE_NO_WARNINGS")
endif()

target_compile_features(mapp PUBLIC cxx_std_14)
target_compile_options(mapp PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)

# Link against psapi on Windows
if(WIN32)
	target_link_libraries(mapp PUBLIC psapi user32)
endif()

# Additional dependencies on Unix
if(ANDROID)
	# For __android_log_write
	find_library(LOG_LIBRARY log)
	mark_as_advanced(LOG_LIBRARY)
	target_link_libraries(mapp PUBLIC ${LOG_LIBRARY})
elseif(APPLE)
	find_library(FOUNDATION_LIBRARY Foundation)
	mark_as_advanced(FOUNDATION_LIBRARY)
	target_link_libraries(mapp PUBLIC ${FOUNDATION_LIBRARY})
elseif(UNIX)
	# Threads
	find_package(Threads)
	target_link_libraries(mapp ${CMAKE_THREAD_LIBS_INIT} dl)

	# Real time (for clock_gettime)
	target_link_libraries(mapp rt)
endif()