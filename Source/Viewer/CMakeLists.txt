add_executable(RoxyViewer
        Main.cpp)

target_link_libraries(RoxyViewer PRIVATE Roxy::Runtime)

add_executable(Roxy::Viewer ALIAS RoxyViewer)