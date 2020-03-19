project('stb_image', 'cpp')

inc = include_directories('.')


stb_image = static_library('stb_image', 'stb_image.cpp', include_directories: inc, install: true )

stb_image_dep = declare_dependency( include_directories: inc, link_with: stb_image )