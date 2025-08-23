cmake_minimum_required(VERSION 3.14)
project(KarplusSynth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# PortAudio
FetchContent_Declare(
  portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(portaudio)

# libsndfile
FetchContent_Declare(
  libsndfile
  GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(libsndfile)

# Собираем все cpp из src/
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Создаём исполняемый файл
add_executable(karplus ${SRC_FILES})

# Подключаем директорию include для .h файлов
target_include_directories(karplus PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Линкуем библиотеки
target_link_libraries(karplus PRIVATE
    portaudio
    sndfile
)

# Компиляционные флаги
if(MSVC)
    target_compile_options(karplus PRIVATE /W4 /permissive-)
else()
    target_compile_options(karplus PRIVATE -Wall -Wextra -pedantic)
endif()
