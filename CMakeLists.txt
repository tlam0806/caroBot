cmake_minimum_required(VERSION 3.16)
project(caroAI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)

add_executable(caro_app MACOSX_BUNDLE ${SRC})
target_include_directories(caro_app PRIVATE src)

# ---------- SFML ----------
if(WIN32)
  include(FetchContent)
  FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
  )
  # Build shared libs so we can ship DLLs easily
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SFML)

  target_link_libraries(caro_app PRIVATE sfml-graphics sfml-window sfml-system)

  # Copy SFML DLLs next to exe after build (portable)
  add_custom_command(TARGET caro_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:sfml-graphics> $<TARGET_FILE_DIR:caro_app>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:sfml-window>   $<TARGET_FILE_DIR:caro_app>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:sfml-system>   $<TARGET_FILE_DIR:caro_app>
  )

elseif(APPLE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SFML REQUIRED IMPORTED_TARGET
    sfml-graphics
    sfml-window
    sfml-system
  )
  target_link_libraries(caro_app PRIVATE PkgConfig::SFML)
  target_include_directories(caro_app PRIVATE ${SFML_INCLUDE_DIRS})

else()
  # Linux (nếu sau này)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SFML REQUIRED IMPORTED_TARGET sfml-graphics sfml-window sfml-system)
  target_link_libraries(caro_app PRIVATE PkgConfig::SFML)
  target_include_directories(caro_app PRIVATE ${SFML_INCLUDE_DIRS})
endif()

# ---------- Assets ----------
# macOS: copy vào app bundle Resources/assets
# others: copy vào cạnh exe ./assets
add_custom_command(TARGET caro_app POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
    $<IF:$<PLATFORM_ID:Darwin>,
      $<TARGET_BUNDLE_CONTENT_DIR:caro_app>/Resources/assets,
      $<TARGET_FILE_DIR:caro_app>/assets>
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<IF:$<PLATFORM_ID:Darwin>,
      $<TARGET_BUNDLE_CONTENT_DIR:caro_app>/Resources/assets,
      $<TARGET_FILE_DIR:caro_app>/assets>
)
