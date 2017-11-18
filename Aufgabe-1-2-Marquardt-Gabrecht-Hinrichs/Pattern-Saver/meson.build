project('patternsaverlib','c',
version: '1.0.0',
license : 'Apache 2.0')
inc = include_directories('include')
subdir('include')
subdir('src')
subdir('test')

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries: patternsaverlib,
version: '1.0',
name: 'libpatternsaver',
filebase: 'patternsaver',
description: 'Speichert patterns')