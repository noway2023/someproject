macro(Ubpa_InitUCMake)
  include(FetchContent)
  set(UCMake_VERSION 0.6.0)
  message(STATUS "find package: UCMake ${UCMake_VERSION}")

  set(Qt5_DIR "H:/tools/qt/5.12.2/msvc2017_64/lib/cmake/Qt5")
  set(Qt5Core_DIR "H:/tools/qt/5.12.2/msvc2017_64/lib/cmake/Qt5Core")
  set(Qt5Gui_DIR "H:/tools/qt/5.12.2/msvc2017_64/lib/cmake/Qt5Gui")
  set(Qt5Widgets_DIR "H:/tools/qt/5.12.2/msvc2017_64/lib/cmake/Qt5Widgets")
  find_package(UCMake ${UCMake_VERSION} QUIET)

  if(NOT UCMake_FOUND)
    message(STATUS "UCMake ${UCMake_VERSION} not found")
    set(_address "https://github.com/Ubpa/UCMake")
    message(STATUS "fetch: ${_address} with tag ${UCMake_VERSION}")
    FetchContent_Declare(
      UCMake
      GIT_REPOSITORY ${_address}
      GIT_TAG ${UCMake_VERSION}
    )
    FetchContent_MakeAvailable(UCMake)
    message(STATUS "UCMake ${UCMake_VERSION} build done")
  endif()
endmacro()
