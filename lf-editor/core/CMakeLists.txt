add_library(lf_core STATIC
    lf_editor.cpp
    common/lf_mpv_session.cpp
    player/lf_mpv_media_player.cpp
    render/lf_mpv_gl_preview.cpp
    render/lf_video_surface.cpp
    timeline/lf_otio_timeline.cpp
    export/lf_ffmpeg_exporter.cpp
)

target_include_directories(lf_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

option(LF_ENABLE_MPV "Enable mpv player backend" ON)
option(LF_ENABLE_OTIO "Enable OpenTimelineIO timeline" OFF)
option(LF_ENABLE_FFMPEG "Enable FFmpeg export" OFF)

if(LF_ENABLE_MPV)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)
    target_compile_definitions(lf_core PUBLIC LF_HAS_MPV=1)
    target_link_libraries(lf_core PUBLIC PkgConfig::MPV)
else()
    message(FATAL_ERROR "lf-editor requires libmpv; install mpv and pkg-config, or set LF_ENABLE_MPV=OFF")
endif()

if(LF_ENABLE_FFMPEG)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(FFMPEG IMPORTED_TARGET QUIET
            libavcodec libavformat libavutil libswscale)
    endif()
    if(TARGET PkgConfig::FFMPEG)
        target_compile_definitions(lf_core PUBLIC LF_HAS_FFMPEG=1)
        target_link_libraries(lf_core PRIVATE PkgConfig::FFMPEG)
    else()
        message(WARNING "LF_ENABLE_FFMPEG is ON but FFmpeg was not found")
    endif()
endif()

if(LF_ENABLE_OTIO)
    find_package(opentimelineio QUIET)
    if(opentimelineio_FOUND)
        target_compile_definitions(lf_core PUBLIC LF_HAS_OTIO=1)
        target_link_libraries(lf_core PRIVATE opentimelineio::opentimelineio)
    else()
        message(WARNING "LF_ENABLE_OTIO is ON but OpenTimelineIO was not found")
    endif()
endif()
