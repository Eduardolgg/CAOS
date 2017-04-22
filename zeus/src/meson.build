
sources = ['main.c', 'serial_start.c', 'filesistem.c']
zeus = executable(
	'zeus',
	sources,
	include_directories: inc,
	install: true,
	install_dir: '/sbin'
)

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

