#GLAD
add_library(glad "glad/src/glad.c")
target_include_directories(glad SYSTEM PUBLIC "glad/include")
