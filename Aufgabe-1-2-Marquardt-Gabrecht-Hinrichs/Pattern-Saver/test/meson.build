cmockadep = dependency('cmocka')
testexe = executable('testexe','test-pattern-save.c',
include_directories: inc,
link_with: patternsaverlib,
dependencies: cmockadep)

test('patternsaverlib test',testexe)