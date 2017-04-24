# Executables for tests
thread_dep = dependency('threads')

sources = ['./screen_print.c']
screen_print_tests = executable(
	't_screen_print',
	sources,
	include_directories: inc,
	install: false
)

sources = ['test_waitpid.c']
waitpid_tests = executable(
	't_waitpid',
	sources,
	include_directories: inc,
	install: false,
	dependencies: thread_dep
)

sources = ['test_waitpid_aux.c']
waitpid_aux_tests = executable(
	't_waitpid_aux',
	sources,
	include_directories: inc,
	install: false
)

# Screen print tests.
test('Usage screen_print', screen_print_tests, args: [], should_fail: true)
test('Info print', screen_print_tests, args: ['a'])
test('Warning print', screen_print_tests, args: ['b'])
test('Error print', screen_print_tests, args: ['c'])
test('Text print', screen_print_tests, args: ['d'])
test('Missing argument', screen_print_tests, args: ['e'], should_fail: true)

# Aux waitpid tests.
test('Success child exit status', waitpid_tests, args: ['i'])
test('Error child exit status', waitpid_tests, args: ['e'], should_fail: true)
test('Crash child', waitpid_tests, args: ['b'], should_fail: true)
test('Wait for child', waitpid_tests, args: ['s'])

# Zeus tests.
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
test('Switch runlevel, diferents scripts',
	zeus,
	args: ['3'],
	env: ['PREVLEVEL=2', 'RUNLEVEL=3']
)
test('Switch runlevel, same scripts',
	zeus,
	args: ['4'],
	env: ['PREVLEVEL=3', 'RUNLEVEL=4']
)
test('Switch runlevel, empty runlevel',
	zeus,
	args: ['5'],
	env: ['PREVLEVEL=3', 'RUNLEVEL=5']
)
