# Executables for tests

sources = ['pty_example.c', '../src/log.c']
pty_example = executable(
	'pty_example',
	sources,
	include_directories: inc,
	install: false,
	dependencies: thread_dep
)

# Aux waitpid tests.
#test('Success child exit status', pty_example, args: ['i'])
#test('Error child exit status', pty_example, args: ['e'], should_fail: true)
#test('Crash child', pty_example, args: ['b'], should_fail: true)
#test('Wait for child', pty_example, args: ['s'])
