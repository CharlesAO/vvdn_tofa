cmake_minimum_required(VERSION 2.8)
project( stereoCalibHomography )
find_package( OpenCV REQUIRED )
FILE(GLOB MyCSources *.cpp)
add_executable( stereoCalibHomography
${MyCSources}

"../Modules/Main/Main.cpp"
"../Modules/Helpers/Helpers.cpp"
"../Modules/CloseRangePattern/CloseRangePattern.cpp"
)

add_definitions(-D__PC__)
set (CMAKE_EXE_LINKER_FLAGS)
include_directories(

"../Modules/Helpers"
"../Modules/Main"
"../Modules/CloseRangePattern"

)
target_link_libraries( stereoCalibHomography ${OpenCV_LIBS} )
