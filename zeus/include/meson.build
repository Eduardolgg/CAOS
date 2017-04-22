
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
	input: 'config.h.in',
	output: 'config.h',
	configuration: conf
)

