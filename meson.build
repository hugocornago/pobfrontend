project('POB Frontend', 'cpp', default_options : ['cpp_std=c++17'])

qt5_dep = dependency('qt5', modules : ['Gui'])
lua_dep = dependency('lua51')
if build_machine.system() == 'darwin'
   gl_dep = dependency('appleframeworks', modules: ['OpenGL'])
else
   gl_dep = dependency('gl')
endif
zlib_dep = dependency('zlib')

# Import the extension module that knows how
# to invoke Qt tools.
qt_headers = [
  'src/subscript.hpp',
  'src/pobwindow.hpp',
  ]
sources = [
  'src/main.cpp',
  'src/pobwindow.cpp',
  'src/lua_cb_gfx.cpp',
  'src/lua_utils.cpp',
  'src/utils.cpp',
  ]
qt5 = import('qt5')
prep = qt5.preprocess(moc_headers : qt_headers)

extra_args = []
extra_ld_args = []
if meson.get_compiler('cpp').get_id() == 'gcc'
  extra_args = ['-Wno-reorder']
  sanitizer = []
  #sanitizer = ['-fsanitize=leak']
  # gprof
  #sanitizer = ['-pg']
  extra_args += sanitizer
  extra_ld_args = sanitizer
endif
extra_args += ['-Wall', '-Os', '-g', '-fno-omit-frame-pointer']

executable('pobfrontend',
  sources : sources + prep,
  dependencies : [qt5_dep, gl_dep, zlib_dep, lua_dep],
  cpp_args: extra_args,
  link_args: extra_ld_args
  )
