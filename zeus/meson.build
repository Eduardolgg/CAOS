project('zeus', 'c')
sources = ['./src/main.c', './src/serial_start.c', './src/filesistem.c']
zeus = executable('zeus', sources, install: true, install_dir: '/sbin')
test('No params but env',
	zeus,
	args: [],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S'],
        should_fail: true
)
test('No params or env',
	zeus,
	args: [],
	env: [],
        should_fail: true
)
test('No env but params',
	zeus,
	args: ['2'],
	env: [],
        should_fail: true
)
test('Init code[9] error',
	zeus,
	args: ['9'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=9'],
        should_fail: true
)
test('Env PREVLEVEL code[9] error',
	zeus,
	args: ['2'],
	env: ['PREVLEVEL=9', 'RUNLEVEL=2'],
        should_fail: true
)
test('Env PREVLEVEL code[unknown] error',
	zeus,
	args: ['2'],
	env: ['PREVLEVEL=unknown', 'RUNLEVEL=2'],
        should_fail: true
)
test('Env RUNLEVEL code[9] error',
	zeus,
	args: ['2'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=9'],
        should_fail: true
)
test('Booting rcS scrips',
	zeus,
	args: ['S'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S']
)
test('Booting rc2 scrips',
	zeus,
	args: ['2'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=2']
)
test('Switch to init 3',
	zeus,
	args: ['3'],
	env: ['PREVLEVEL=2', 'RUNLEVEL=3']
)
test('Switch to init 2',
	zeus,
	args: ['2'],
	env: ['PREVLEVEL=3', 'RUNLEVEL=2']
)

# Executables for tests
exeTests = executable(
	'screen_print.c',
	'./tests/screen_print.c',
	install: false
)
test('Info print', exeTests, args: ['a'])
test('Infoln print', exeTests, args: ['b'])
test('Warning print', exeTests, args: ['c'])
test('Warningln print', exeTests, args: ['d'])
test('Error print', exeTests, args: ['e'])
test('Errorln print', exeTests, args: ['f'])
test('Text print', exeTests, args: ['g'])
test('Textln print', exeTests, args: ['h'])
