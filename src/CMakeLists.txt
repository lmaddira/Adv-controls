project(mpc)
add_executable(src main.cpp)
# Find OSQP library and headers
find_package(osqp REQUIRED)

# Link the OSQP shared library
target_link_libraries(src PRIVATE osqp::osqp)

# or...

# Link the OSQP static library
target_link_libraries(src PRIVATE osqp::osqpstatic)


