cmake_minimum_required(VERSION 2.6)
project(SDL2_mixer)

### configuration ###

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
# enable all file formats which are supported natively
#set(SUPPORTED_FORMATS CMD WAV MODPLUG MOD MID OGG MP3 FLAC)
set(SUPPORTED_FORMATS WAV MODPLUG MID OGG MP3 FLAC)
#set(SUPPORTED_MIDI TIMIDITY FLUIDSYNTH NATIVE)
set(SUPPORTED_MIDI TIMIDITY NATIVE)

# enable all file formats which are supported through external dependencies
# first try to load them statically (lib file in vcpkg installation)
# if this fails try to make them a dynamic dependency (dll will be loaded at runtime) if possible. vcpkg cannot resolve these dependencies!
# else do not support this file format at all
set(DEPENDENCIES MODPLUG OGG FLAC MP3)

# patch library names for preprocessor flags
#set(JPEG_FLAG JPG)
#set(TIFF_FLAG TIF)

# names of potentially dynamically loaded libraries
set(MODPLUG_DYNAMIC \"libmodplug-1.dll\")
set(OGG_DYNAMIC \"libvorbisfile-3.dll\")
set(FLAC_DYNAMIC \"libFLAC-8.dll\")
set(MP3_DYNAMIC \"smpeg2.dll\")

### implementation ###

add_library(SDL2_mixer
	wavestream.c
	music_ogg.c
	music_modplug.c
	music_mod.c
	music_mad.c
	music_flac.c
	music_cmd.c
	music.c
	mixer.c
	load_voc.c
	load_ogg.c
	load_mp3.c
	load_flac.c
	load_aiff.c
	fluidsynth.c
	effects_internal.c
	effect_stereoreverse.c
	effect_position.c
	dynamic_ogg.c
	dynamic_mp3.c
	dynamic_modplug.c
	dynamic_mod.c
	dynamic_fluidsynth.c
	dynamic_flac.c
	timidity/timidity.c
	timidity/tables.c
	timidity/sdl_c.c
	timidity/sdl_a.c
	timidity/resample.c
	timidity/readmidi.c
	timidity/playmidi.c
	timidity/output.c
	timidity/mix.c
	timidity/instrum.c
	timidity/filter.c
	timidity/ctrlmode.c
	timidity/common.c
	native_midi/native_midi_win32.c
	native_midi/native_midi_macosx.c
	native_midi/native_midi_mac.c
	native_midi/native_midi_haiku.cpp
	native_midi/native_midi_common.c
	)

foreach(FORMAT ${SUPPORTED_FORMATS})
	add_definitions(-D${FORMAT}_MUSIC)
endforeach(FORMAT)

foreach(FORMAT ${SUPPORTED_MIDI})
	add_definitions(-DUSE_${FORMAT}_MIDI)
endforeach(FORMAT)

# SDL
find_path(SDL_INCLUDE_DIR SDL2/SDL.h)
find_library(SDL_LIBRARY SDL2)

include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDL_INCLUDE_DIR}/SDL2)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(timidity)
include_directories(native_midi)

target_link_libraries(SDL2_mixer ${SDL_LIBRARY} winmm)

# external dependencies
foreach(DEPENDENCY ${DEPENDENCIES})
	find_package(${DEPENDENCY})
	
	if(NOT DEFINED ${DEPENDENCY}_FLAG)
		set(${DEPENDENCY}_FLAG ${DEPENDENCY})
	endif()
	
	add_definitions(-DLOAD_${${DEPENDENCY}_FLAG})
	if(${DEPENDENCY}_FOUND)
		message(STATUS "  --> linking statically.")
		target_link_libraries(SDL2_mixer ${${DEPENDENCY}_LIBRARIES})
	elseif(DEFINED ${DEPENDENCY}_DYNAMIC)
		message(STATUS "  --> linking dynamically.")
		add_definitions(-D${${DEPENDENCY}_FLAG}_DYNAMIC=${${DEPENDENCY}_DYNAMIC})
		set(RUNTIME_DEPENDENCIES ON)
	else()
		message(STATUS "  --> skipping.")
	endif()
endforeach(DEPENDENCY)

if(DEFINED RUNTIME_DEPENDENCIES)
	include_directories(VisualC/external/include)
endif()


install(TARGETS SDL2_mixer
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)

install(FILES SDL_mixer.h DESTINATION include/SDL2 CONFIGURATIONS Release)


message(STATUS "Link-time dependencies:")
message(STATUS "  " ${SDL_LIBRARY})
foreach(DEPENDENCY ${DEPENDENCIES})
	if(${DEPENDENCY}_FOUND)
		message(STATUS "  " ${DEPENDENCY})
	endif()
endforeach(DEPENDENCY)

if(DEFINED RUNTIME_DEPENDENCIES)
	message(STATUS "Run-time dependencies:")
	foreach(DEPENDENCY ${DEPENDENCIES})
		if(NOT ${DEPENDENCY}_FOUND AND DEFINED ${DEPENDENCY}_DYNAMIC)
			message(STATUS "  " ${${DEPENDENCY}_DYNAMIC})
		endif()
	endforeach(DEPENDENCY)
endif()