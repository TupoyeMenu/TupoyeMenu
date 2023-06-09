include(FetchContent)

set(BUILD_TESTING_BEFORE ${BUILD_TESTING})
set(CURL_DISABLE_TESTS OFF) 
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        e65e685fa907492fc5f337e97a7e9713758cc9e7
    GIT_PROGRESS TRUE
) 
message("cpr")
FetchContent_MakeAvailable(cpr)

set(BUILD_TESTING ${BUILD_TESTING_BEFORE} CACHE INTERNAL "" FORCE)