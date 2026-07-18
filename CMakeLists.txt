cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("class_gen_imac_edition" VERSION 1.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchDependency.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildLib.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BuildExample.cmake)

file(GLOB_RECURSE ${PROJECT_NAME}_LIB_H
    CMAKE_CONFIGURE_DEPENDS
    "include/*.h"
)
file(GLOB_RECURSE ${PROJECT_NAME}_LIB_CPP
    CMAKE_CONFIGURE_DEPENDS
    "src/*.cpp"
)

file(GLOB_RECURSE ${PROJECT_NAME}_TESTS
    CMAKE_CONFIGURE_DEPENDS
    #"tests/*.cpp"
    #"tests/AliasGenTests.cpp"
    "tests/ClassGenTests.cpp"
)

set(${PROJECT_NAME}_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(glm REQUIRED)
set(${PROJECT_NAME}_LIBS
    glm::glm
)

option(${PROJECT_NAME}_BUILD_EXAMPLE "Построить пример для проекта" ON)
option(${PROJECT_NAME}_BUILD_TESTS "Построить тесты для проекта" ON)
option(${PROJECT_NAME}_ASSET_TO_BUILD "Копировать ассеты в папку сборки" OFF)

create_library(${PROJECT_NAME}
    HEADERS ${${PROJECT_NAME}_LIB_H}
    SOURCES ${${PROJECT_NAME}_LIB_CPP}
    INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE}
    LIBS ${${PROJECT_NAME}_LIBS}
)

if(${PROJECT_NAME}_BUILD_EXAMPLE)
    set(${PROJECT_NAME}_EXAMPLE_FILEPATH main.cpp)
    set(${PROJECT_NAME}_EXAMPLE_LIBS "")

    create_executable(${PROJECT_NAME}_example
        SOURCE ${${PROJECT_NAME}_EXAMPLE_FILEPATH}
        COPY_ASSETS ${PROJECT_NAME}_ASSET_TO_BUILD
        LIBS ${PROJECT_NAME} ${${PROJECT_NAME}_EXAMPLE_LIBS}
    )
    set_source_files_properties(${${PROJECT_NAME}_EXAMPLE_FILEPATH} PROPERTIES
        OBJECT_DEPENDS "${${PROJECT_NAME}_LIB_H}"
    )
endif()

if(${PROJECT_NAME}_BUILD_TESTS)
    if(${PROJECT_NAME}_TESTS)        
        find_package(GTest CONFIG REQUIRED)        
        
        list(GET ${PROJECT_NAME}_TESTS 0 FIRST_TEST_FILE)        
        list(REMOVE_AT ${PROJECT_NAME}_TESTS 0)

        create_executable(run_tests
            SOURCE ${FIRST_TEST_FILE}
            LIBS 
                ${PROJECT_NAME}
                GTest::gtest
                GTest::gtest_main
        )        
        
        if(TARGET run_tests AND ${PROJECT_NAME}_TESTS)            
            target_sources(run_tests PRIVATE ${${PROJECT_NAME}_TESTS})        
        endif()        
        
        if(TARGET run_tests)            
            enable_testing()            
            include(GoogleTest)
            gtest_discover_tests(run_tests)        
        else()            
            message(WARNING "[WARNING] Таргет run_tests не был создан.")        
        endif()
        
    else()        
        message(STATUS "[NOTICE] Тесты не будут собраны: папка tests/ пуста.")    
    endif()
endif()