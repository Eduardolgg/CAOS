# Executables for tests

sources = ['./screen_print.c']
exeTests = executable(
	't_screen_print',
	sources,
	include_directories: inc,
	install: false
)
test('Usage screen_print', exeTests, args: [], should_fail: true)
test('Info print', exeTests, args: ['a'])
test('Infoln print', exeTests, args: ['b'])
test('Warning print', exeTests, args: ['c'])
test('Warningln print', exeTests, args: ['d'])
test('Error print', exeTests, args: ['e'])
test('Errorln print', exeTests, args: ['f'])
test('Text print', exeTests, args: ['g'])
test('Textln print', exeTests, args: ['h'])
test('Missing argument', exeTests, args: ['i'], should_fail: true)
