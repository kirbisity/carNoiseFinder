add_library(carlib car.cpp car.cpp parser.cpp polynomial.cpp)

add_executable(CARNOISE main.cpp)

target_link_libraries(CARNOISE carlib)