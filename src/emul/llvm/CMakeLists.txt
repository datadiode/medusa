set(INCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)
set(SRCROOT ${CMAKE_SOURCE_DIR}/src/emul/llvm)

set(SRC
  ${SRCROOT}/main.cpp
  ${SRCROOT}/llvm_emulator.cpp
  ${INCROOT}/llvm_emulator.hpp
  )

add_library(emul_llvm SHARED ${SRC})

if (UNIX AND LLVM_BUILD_ROOT)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LLVM_BUILD_ROOT}/share/llvm/cmake")
  include(LLVMConfig)
  include_directories(${LLVM_INCLUDE_DIRS})
  link_directories(${LLVM_LIBRARY_DIRS})
  add_definitions(${LLVM_DEFINITIONS})
  llvm_map_components_to_libraries(LLVM_LIBRARIES mcjit native)
  target_link_libraries(emul_llvm Medusa "-L ${LLVM_LIBRARY_DIRS}" ${LLVM_LIBRARIES}) # -L should not be here, but link_directories doesn't seem to work...

elseif (WIN32 AND LLVM_BUILD_ROOT_DEBUG AND LLVM_BUILD_ROOT_RELEASE)

  set(LLVM_ALL_LIBRARIES)
  set(LLVM_ALL_LIBRARY_DIRS)

  # Start with the debug configuration
  list(APPEND CMAKE_MODULE_PATH "${LLVM_BUILD_ROOT_DEBUG}/share/llvm/cmake")
  include(LLVMConfig)

  # This configuration is common (debug/release)
  include_directories(${LLVM_INCLUDE_DIRS})

  list(APPEND CMAKE_CXX_FLAGS_DEBUG LLVM_DEFINITIONS)
  llvm_map_components_to_libraries(LLVM_LIBRARIES_DEBUG mcjit native)
  foreach (LIB ${LLVM_LIBRARIES_DEBUG})
    list(APPEND LLVM_ALL_LIBRARIES debug "${LLVM_LIBRARY_DIRS}/${LIB}.lib")
  endforeach()

  # I have no idea what I'm doing
  list(REMOVE_ITEM CMAKE_MODULE_PATH "${LLVM_BUILD_ROOT_DEBUG}/share/llvm/cmake")
  list(APPEND CMAKE_MODULE_PATH "${LLVM_BUILD_ROOT_RELEASE}/share/llvm/cmake")
  include(LLVMConfig)

  list(APPEND CMAKE_CXX_FLAGS_RELEASE LLVM_DEFINITIONS)
  llvm_map_components_to_libraries(LLVM_LIBRARIES_RELEASE mcjit native)
  foreach (LIB ${LLVM_LIBRARIES_RELEASE})
    list(APPEND LLVM_ALL_LIBRARIES optimized "${LLVM_LIBRARY_DIRS}/${LIB}.lib")
  endforeach()

  target_link_libraries(emul_llvm Medusa ${LLVM_ALL_LIBRARIES})

endif()


if(WIN32)
  install(TARGETS emul_llvm RUNTIME DESTINATION .)
else()
  install(TARGETS emul_llvm LIBRARY DESTINATION .)
endif()
