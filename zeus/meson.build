project('Zeus', 'c', license: 'GPLV2', version: '0.0.1')
buildtype = get_option('buildtype')

conf = configuration_data()
conf.set('version', meson.project_version())
conf.set('app_name', meson.project_name())
conf.set('USE_COLORS', true)
if buildtype == 'debug' or buildtype == 'debugoptimized'
	conf.set('rcx_directory', '../tests/fake_init.d/rcX.d')
	conf.set('rcx_xchar_location', '23')
	conf.set('rcx_buffer_len', '27')
else
	conf.set('rcx_directory', '/etc/rcX.d')
	conf.set('rcx_xchar_location', '7')
	conf.set('rcx_buffer_len', '11')
endif

configure_file(
	input: './src/config.h.in',
	output: 'config.h',
	configuration: conf
)

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

# Executables for tests
exeTests = executable(
	't_screen_print',
	'./tests/screen_print.c',
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
