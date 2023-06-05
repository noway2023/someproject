cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(MiniDraw VERSION 1.0)
message(STATUS "[Project] ${PROJECT_NAME}")




include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()
set(Eigen3_DIR D:/project/myProject/USTC_CG/Homeworks/1_MiniDraw/eigen/install/share/eigen3/cmake) # change into your path
find_package(Eigen3 REQUIRED)
Ubpa_QtInit(COMPONENTS Widgets)

Ubpa_AddSubDirsRec(src)
