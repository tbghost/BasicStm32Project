# ================================================================================
# CMake Listfile root/tests
# ================================================================================

enable_testing()

add_executable(gTestUnit 
							test_main.cpp)

# Sources
file(GLOB_RECURSE GTEST_FIXTURES "${CMAKE_SOURCE_DIR}/src/*.cxx")
target_sources(gTestUnit
								PRIVATE
		    				${GTEST_FIXTURES}
							)

# Includes
target_include_directories(gTestUnit
													 PUBLIC 
													)


target_link_libraries(gTestUnit 
                      Utils
											gtest 
                      gmock
                      gtest_main)

add_test(NAME GTestUnit COMMAND gTestUnit)

set_target_properties(gTestUnit PROPERTIES OUTPUT_NAME "gTestUnit")

include(GoogleTest)
	



















if ((PLATFORM STREQUAL "Windows") AND (BUILD_UNITTESTS))

    ################################
    # GTest
    ################################
    option(CMAKE_USE_WIN32_THREADS_INIT "using WIN32 threads" ON) # Not sure if needed, but was included in the examples that used gtest_disable_pthreads on Windows
	set(gtest_disable_pthreads  ON CACHE INTERNAL "" FORCE)       # Disable pthread explicitly for CLion
    set(GTEST_HAS_PTHREAD 0)

	option(INSTALL_GTEST "Disable Gtest installation" OFF)
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY https://appme.miele.com/dvcs/epd/forks/googletest.git
	  GIT_TAG        release-1.12.1
	)
	FetchContent_MakeAvailable(googletest)

	# Prevent overriding the parent project's compiler/linker settings on Windows
	set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)

	# Add unittest executable
	string(ASCII 27 Esc)
	set(ColourReset "${Esc}[m")
	set(BoldYellow  "${Esc}[1;33m")
	message("${BoldYellow}Add unittest executable")
	message ("Unittests use rootDir: ${CMAKE_SOURCE_DIR}${ColourReset}")

	add_executable(gTestUnit)
	add_test(NAME TestUnit COMMAND gTestUnit)
	set_target_properties(gTestUnit PROPERTIES OUTPUT_NAME "gTestUnit")

	# Includes
	target_include_directories(gTestUnit 
        PUBLIC 
            ${INCLUDE_DIRS} 
            $<INSTALL_INTERFACE:include>
    )

	# Sources
	file(GLOB_RECURSE GTEST_FIXTURES "${CMAKE_SOURCE_DIR}/src/*.cxx")
	target_sources(gTestUnit
	    PRIVATE
		    ${GTEST_FIXTURES}
	)

	target_link_libraries(gTestUnit 
	  PUBLIC
		ca_cu_lib
        cookingprocess
		gtest
		gmock    
	)

	include(GoogleTest)
	gtest_discover_tests(gTestUnit DISCOVERY_MODE PRE_TEST)
endif()