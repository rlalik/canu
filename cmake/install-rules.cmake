if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/canu-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

# Project is configured with no languages, so tell GNUInstallDirs the lib dir
set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "")

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package canu)

install(
    DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT canu_Development
)

install(
    TARGETS canu_canu
    EXPORT canuTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)

# Allow package maintainers to freely override the path for the configs
set(
    canu_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE canu_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(canu_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${canu_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT canu_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${canu_INSTALL_CMAKEDIR}"
    COMPONENT canu_Development
)

install(
    EXPORT canuTargets
    NAMESPACE canu::
    DESTINATION "${canu_INSTALL_CMAKEDIR}"
    COMPONENT canu_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
