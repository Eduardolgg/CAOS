project('zeus', 'c')
sources = ['./src/main.c', './src/serial_start.c', './src/filesistem.c']
executable('zeus', sources)

# Executables for tests
exeTests = executable('screen_print.c', './tests/screen_print.c')
test('Info print', exeTests, args: ['a'])
test('Infoln print', exeTests, args: ['b'])
test('Warning print', exeTests, args: ['c'])
test('Warningln print', exeTests, args: ['d'])
test('Error print', exeTests, args: ['e'])
test('Errorln print', exeTests, args: ['f'])
test('Text print', exeTests, args: ['g'])
test('Textln print', exeTests, args: ['h'])
