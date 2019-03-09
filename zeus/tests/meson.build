# Executables for tests
add_test_setup('valgrind_trace_children',
	exe_wrapper: ['valgrind', '--trace-children=yes']
)

sources = ['log.c', '../src/log.c']
log_tests = executable(
	't_log',
	sources,
	include_directories: inc,
	install: false
)

sources = ['test_waitpid.c', '../src/log.c']
waitpid_tests = executable(
	't_waitpid',
	sources,
	include_directories: inc,
	install: false,
	dependencies: thread_dep
)

sources = ['test_waitpid_aux.c', '../src/log.c']
waitpid_aux_tests = executable(
	't_waitpid_aux',
	sources,
	include_directories: inc,
	install: false
)

sources = ['filesystem.c', '../src/filesystem.c', '../src/log.c']
filesystem_tests = executable(
	't_filesystem',
	sources,
	include_directories: inc,
	install: false
)

sources = ['runlevel_utils.c', '../src/runlevel_utils.c',
           '../src/filesystem.c', '../src/log.c']
runlevel_tests = executable(
	't_runlevel',
	sources,
	include_directories: inc,
	install: false
)

# Screen print tests.
test('Usage log', log_tests, args: [], should_fail: true)
test('Info print', log_tests, args: ['a'])
test('Warning print', log_tests, args: ['b'])
test('Error print', log_tests, args: ['c'])
test('Text print', log_tests, args: ['d'])
test('Text break line', log_tests, args: ['e'])
test('Missing argument', log_tests, args: ['f'], should_fail: true)

# Aux waitpid tests.
test('Success child exit status', waitpid_tests, args: ['i'])
test('Error child exit status', waitpid_tests, args: ['e'], should_fail: true)
test('Crash child', waitpid_tests, args: ['b'], should_fail: true)
test('Wait for child', waitpid_tests, args: ['s'])

# Filesystem tests.
test('Change init diff usage', filesystem_tests, args: [], should_fail: true)
test('Change init diff 2->4', filesystem_tests, args: ['a', '2', '4', '3'])
test('Change init diff 2->5', filesystem_tests, args: ['a', '2', '5', '0'])
test('Change init diff 5->2', filesystem_tests, args: ['a', '5', '2', '4'])
test('Change init diff 3->4', filesystem_tests, args: ['a', '3', '4', '0'])
test('Change init diff 2->S', filesystem_tests, args: ['a', '2', 'S', '2'])
test('Change init diff 2->9', filesystem_tests, args: ['a', '2', '9', '1'],
	should_fail: true)
test('Change init diff 9->2', filesystem_tests, args: ['a', '9', '2', '1'],
	should_fail: true)
test('Get script dir, wrong code', filesystem_tests, args: ['b', '9', '-', '-'])
test('free_script_llist', filesystem_tests, args: ['c', '2', '5', '-'])
test('Check interactive scripts', filesystem_tests, args: ['d', '2', '3', '-'])


# Runlevel tests.
test('Runlevel uninitialized pointers', runlevel_tests,
	args: ['a'], should_fail: true)
test('Runlevel free memory', runlevel_tests,
	args: ['b'], env: ['PREVLEVEL=N', 'RUNLEVEL=S'])
test('Check runlevels', runlevel_tests,
	args: ['c'])
test('Check set runlevel 1', runlevel_tests,
	args: ['d'], env: ['NONE_SET=:)'])
test('Check set runlevel 2', runlevel_tests,
	args: ['d'], env: ['RUNLEVEL=S'])
test('Check runlevel param error', runlevel_tests,
	args: ['e'], should_fail: true)
test('Check runlevel test usage', runlevel_tests,
	args: [], should_fail: true)

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
	args: ['serial'],
	env: [],
        should_fail: true
)
test('Init code[9] error',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=9'],
        should_fail: true
)
test('Env PREVLEVEL code[9] error',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=9', 'RUNLEVEL=2'],
        should_fail: true
)
test('Env PREVLEVEL code[unknown] error',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=unknown', 'RUNLEVEL=2'],
        should_fail: true
)
test('Env RUNLEVEL code[9] error',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=9'],
        should_fail: true
)
test('Unknown startup algorithm',
	zeus,
	args: ['no_algoritm'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S'],
        should_fail: true
)
test('Unknown startup algorithm S',
	zeus,
	args: ['S'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S'],
        should_fail: true
)
test('Booting rcS scrips',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S']
)
test('Booting rc2 scrips',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=2']
)
test('Switch runlevel, diferents scripts',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=2', 'RUNLEVEL=3']
)
test('Switch runlevel, same scripts',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=3', 'RUNLEVEL=4']
)
test('Switch runlevel, empty runlevel',
	zeus,
	args: ['serial'],
	env: ['PREVLEVEL=3', 'RUNLEVEL=5']
)
test('LP Booting rcS scrips',
	zeus,
	args: ['low_parallel'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=S']
)
test('LP Booting rc2 scrips',
	zeus,
	args: ['low_parallel'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=2']
)
test('LP Switch runlevel, diferents scripts',
	zeus,
	args: ['low_parallel'],
	env: ['PREVLEVEL=2', 'RUNLEVEL=3']
)
test('LP too many scripts',
	zeus,
	args: ['low_parallel'],
	env: ['PREVLEVEL=N', 'RUNLEVEL=6']
)
