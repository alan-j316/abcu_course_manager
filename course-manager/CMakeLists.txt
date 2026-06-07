add_executable(ABCUApplication)


set(SourceDir "src/")
set(CompilationUnits "CourseManager.cpp;Course.cpp;Main.cpp")
list(TRANSFORM CompilationUnits PREPEND ${SourceDir})

set(HeaderDir "include/")
set(HeaderFiles "Utils.h;CourseManager.h;Course.h;CourseNode.h")
list(TRANSFORM HeaderFiles PREPEND ${HeaderDir})

target_sources(
        ABCUApplication
        PRIVATE
        ${CompilationUnits}

        PRIVATE
        FILE_SET HEADERS
        BASE_DIRS ${HeaderDir}
        FILES ${HeaderFiles}
)

target_include_directories(
        ABCUApplication PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(
        ABCUApplication PRIVATE
        StrictWarnings
)

# Copy the csv file into the directory of exec for testing
add_custom_command(
        TARGET ABCUApplication POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/ABCU_Advising_Program_Input.csv"
        "$<TARGET_FILE_DIR:ABCUApplication>/ABCU_Advising_Program_Input.csv"
)