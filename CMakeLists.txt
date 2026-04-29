cmake_minimum_required(VERSION 3.22)

# Windows static runtime
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# MacOS unversal binary
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

# MacOS minimum deployment target
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

# Main project
project(EDI_AUDIO VERSION 0.0.1 LANGUAGES C CXX)

# Get add_plugin function
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(add_plugin)


### Plugins

# Global plugin variables
set(company_name "EDI Audio")
set(company_copyright "EDI Audio Technologies")
set(company_code "EDIa")
set(company_bundle_id "com.EDIAudio")

# Helper variable for source dir
set(source_dir ${CMAKE_SOURCE_DIR}/source)

# Add "PhaseVocoder" plugin target
add_plugin(PhaseVocoder
    VERSION 0.0.1
    SYNTH FALSE
    PLUGIN_NAME "Phase Vocoder"
    PROD_NAME "PhaseVocoder"
    PROD_CODE "Phvc"
    SOURCES
        ${source_dir}/PhaseVocoder/PhaseVocoderProcessor.cpp
        ${source_dir}/PhaseVocoder/PhaseVocoderEditor.cpp
        ${source_dir}/DSP/OverlapSave.cpp
        ${source_dir}/DSP/OverlapAdd.cpp
        ${source_dir}/DSP/Phase.cpp
    INCLUDE_DIRS
        ${source_dir}/PhaseVocoder
        ${source_dir}/DSP
)
