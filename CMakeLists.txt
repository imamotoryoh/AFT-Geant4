#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(FiberTarget)

#----------------------------------------------------------------------------
# Find Geant4 package, no UI and Vis drivers activated
#
find_package(Geant4 REQUIRED)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
#find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
#================================
# change the include path of ROOT for my mac pc (imamoto)
execute_process(
COMMAND root-config --incdir
OUTPUT_VARIABLE ROOT_INCLUDE_DIR
OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
COMMAND root-config --libs
OUTPUT_VARIABLE ROOT_LIBRARIES
OUTPUT_STRIP_TRAILING_WHITESPACE
)
#================================
include(${Geant4_USE_FILE})
#set(ROOT_INCLUDE_DIR /Users/imamotoryou/root/root-6.36.02/include)
#set(ROOT_INCLUDE_DIR /usr/local/include)
#set(ROOT_LIBRARIES -L/usr/local/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executables, and link it to the Geant4 libraries
add_executable(FiberTarget FiberTarget.cc ${sources} ${headers})
target_link_libraries(FiberTarget ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX /home/dragon/dragon/sim/FiberTarget)
install(TARGETS FiberTarget RUNTIME DESTINATION bin)



