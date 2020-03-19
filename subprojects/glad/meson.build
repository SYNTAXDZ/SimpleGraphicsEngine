project('glad', 'c')

inc = include_directories('include')

cc = meson.get_compiler('c')
dl_dep = cc.find_library( 'dl', required: false )

glad = static_library('glad', 'src/glad.c', include_directories: inc,dependencies: [dl_dep] ,install: true )

glad_dep = declare_dependency( include_directories: inc, link_with: glad )