project('gsttutor', 'cpp', version: '1.0', default_options: ['warning_level=3', 'cpp_std=c++17'])
add_project_arguments('-std=c++17', language: 'cpp')
add_project_arguments('-Wfatal-errors', language: 'cpp')
add_project_arguments('-fpermissive', language: 'cpp')
add_project_arguments('-Wno-narrowing', language: 'cpp')
add_project_arguments('-Wno-missing-field-initializers', language: 'cpp')


gstreamer = dependency('gstreamer-1.0', version : ['>=1.20'])

dependencies = [gstreamer]


executable(
    'app_dev_basic_01_simple_init',
    'src/application-development-basics/01_simple_init.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_02_goption_interface',
    'src/application-development-basics/02_goption_interface.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_03_creating_gstelement',
    'src/application-development-basics/03_creating_gstelement.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_04_using_element_as_gobject',
    'src/application-development-basics/04_using_element_as_gobject.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_05_get_info_about_element',
    'src/application-development-basics/05_get_info_about_element.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_06_linking_elements',
    'src/application-development-basics/06_linking_elements.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_07_creating_a_bin',
    'src/application-development-basics/07_creating_a_bin.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_08_custom_bins',
    'src/application-development-basics/08_custom_bins.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_09_how_to_use_a_bus',
    'src/application-development-basics/09_how_to_use_a_bus.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'app_dev_basic_10_dynamic_pads',
    'src/application-development-basics/10_dynamic_pads.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'basic_turor_01_hello_world',
    'src/basic-tutorial/01_hello_world.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'basic_turor_02_concepts',
    'src/basic-tutorial/02_concepts.cpp',
    dependencies: dependencies,
    install: true
)

library(
    'basic_turor_03_dynamic_pipelines',
    'src/basic-tutorial/03_dynamic_pipelines.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'udpsinksample',
    'src/h264-rtpstreaming/udpsink.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'udpsrcsample',
    'src/h264-rtpstreaming/udpsrc.cpp',
    dependencies: dependencies,
    install: true
)

executable(
    'device-monitor',
    'src/device-monitor/device-monitor-sample.cpp',
    dependencies: dependencies,
    install: true,
)

executable(
    'multicamera',
    'src/multiple-camera/multiple-camera.cpp',
    dependencies: dependencies,
    install: true,
)

executable(
    'streaming-receiver',
    'src/streaming/streaming-receiver.cpp',
    'src/streaming/bus.cpp',
    'src/streaming/clock.cpp',
    dependencies: dependencies,
    install: true,
)

executable(
    'streaming-sender',
    'src/streaming/streaming-sender.cpp',
    'src/streaming/bus.cpp',
    'src/streaming/clock.cpp',
    dependencies: dependencies,
    install: true,
)

executable(
    'video-preview',
    'src/streaming/video-preview.cpp',
    'src/streaming/bus.cpp',
    'src/streaming/clock.cpp',
    dependencies: dependencies,
    install: true,
)

