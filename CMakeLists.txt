cmake_minimum_required(VERSION 3.12)
project(dua_math)

set(CMAKE_BUILD_TYPE "RelWithDebInfo")

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fvisibility=hidden)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 3.4 REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)

# DUA math library
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE c_std_99 cxx_std_17)
target_compile_definitions(${PROJECT_NAME} INTERFACE "DUA_MATH_BUILDING_LIBRARY")
target_include_directories(${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
ament_target_dependencies(${PROJECT_NAME} INTERFACE
  geometry_msgs
  tf2
)

# Library installation
install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export targets and dependencies
ament_export_targets(export_${PROJECT_NAME})
ament_export_include_directories(include)
ament_export_dependencies(
  eigen3_cmake_module
  geometry_msgs
  tf2
)

ament_package()
