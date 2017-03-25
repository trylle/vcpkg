# Common Ambient Variables:
#   VCPKG_ROOT_DIR = <C:\path\to\current\vcpkg>
#   TARGET_TRIPLET is the current triplet (x86-windows, etc)
#   PORT is the current port name (zlib, etc)
#   CURRENT_BUILDTREES_DIR = ${VCPKG_ROOT_DIR}\buildtrees\${PORT}
#   CURRENT_PACKAGES_DIR  = ${VCPKG_ROOT_DIR}\packages\${PORT}_${TARGET_TRIPLET}
#

include(vcpkg_common_functions)
set(SOURCE_PATH ${CURRENT_BUILDTREES_DIR}/src/SDL2_mixer-2.0.1)
vcpkg_download_distfile(ARCHIVE
    URLS "https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.1.zip"
    FILENAME "SDL2_mixer-2.0.1.zip"
    SHA512 7399f08c5b091698c90d49fcc2996677eae8a36f05a65b4470807c9cf2c04730669e0ca395893cfa49177a929f8c5b2b10b6c541ba2fe2646300dcdad4ec1d9e
)
vcpkg_extract_source_archive(${ARCHIVE})

vcpkg_apply_patches(
    SOURCE_PATH ${SOURCE_PATH}
    PATCHES 
        ${CMAKE_CURRENT_LIST_DIR}/correct-sdl-headers-dir.patch)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt DESTINATION ${SOURCE_PATH})

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    # OPTIONS
    # OPTIONS_RELEASE -DOPTIMIZE=1
    # OPTIONS_DEBUG -DDEBUGGABLE=1
)

vcpkg_install_cmake()

# Handle copyright
file(COPY ${SOURCE_PATH}/COPYING.txt DESTINATION ${CURRENT_PACKAGES_DIR}/share/sdl2-mixer)
file(RENAME ${CURRENT_PACKAGES_DIR}/share/sdl2-mixer/COPYING.txt ${CURRENT_PACKAGES_DIR}/share/sdl2-mixer/copyright)

vcpkg_copy_pdbs()