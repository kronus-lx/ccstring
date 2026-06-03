vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO https://github.com/kronus-lx/ccstring.git
    REF 8c56304c51539840e786266d09afa7aa0424556d
    SHA512 f1d506891361e74f417c8e44b6b9a9da5c316f7c70b518f75b460d90cbb6daee1cc5936c2b2f9ea28bb4603baddf8c60b3fa48c5c1f13fea52fbff192022b135
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
)

vcpkg_install_cmake()
vcpkg_copy_pdbs()
vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/ccstring)
