
conf = configuration_data()
conf.set('version', meson.project_version())
conf.set('app_name', meson.project_name())
conf.set('USE_COLORS', true)
conf.set('DEBUG', true)
#conf.set('zeus_exec_name', zeus.exec)
if buildtype == 'debug' or buildtype == 'debugoptimized'
	conf.set('rcx_directory', '../tests/fake_etc/rcX.d')
	conf.set('rcx_xchar_location', '20')
	conf.set('rcx_buffer_len', '24')
	conf.set('run_directory', '../tests/run')
	conf.set('caos_config_directory', '../caos/init/')
else
	conf.set('rcx_directory', '/etc/rcX.d')
	conf.set('rcx_xchar_location', '7')
	conf.set('rcx_buffer_len', '11')
	conf.set('run_directory', '/run')
	conf.set('caos_config_directory', '/etc/caos/init/')
endif

configure_file(
	input: 'config.h.in',
	output: 'config.h',
	configuration: conf
)

