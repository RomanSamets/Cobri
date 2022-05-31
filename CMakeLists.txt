cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)

project("cobri" VERSION 0.1.0 LANGUAGES C)

add_executable(${PROJECT_NAME} game.c RenderEngine.c menu.c)