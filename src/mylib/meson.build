gl = dependency('gl')
assimp = dependency( 'assimp' )
x11 = dependency('x11')
threads = dependency('threads')
glfw3 = dependency( 'glfw3' )
glad_sp = subproject( 'glad' )
glad = glad_sp.get_variable( 'glad_dep' )
spdlog_sp = subproject( 'spdlog' )
spdlog = spdlog_sp.get_variable( 'spdlog_dep' )
stb_image_sp = subproject( 'stb_image' )
stb_image = stb_image_sp.get_variable( 'stb_image_dep' )
glm = dependency( 'glm' )
cc = meson.get_compiler('c')
dl = cc.find_library('dl', required : false)

my_dep = [gl,glfw3,glad,assimp,spdlog,stb_image,glm,dl,x11,threads]


# run the shell script
c = run_command('fetch_files.sh')
# and strip the output by newline and making the result as sources
sources = c.stdout().strip().split('\n')

mylib = library( 'mylib', 
                  sources: sources,
                  dependencies: my_dep,
                  cpp_pch: 'pch/pch.hpp')

mylib_dep = declare_dependency(dependencies: my_dep,
                               include_directories : include_directories('src/'), link_with : mylib)