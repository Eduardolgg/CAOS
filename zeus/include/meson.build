pj_root = meson.source_root() + '/'

conf = configuration_data()

conf.set('version', meson.project_version())
conf.set('app_name', meson.project_name())
conf.set('USE_COLORS', true)
conf.set('DEBUG', false)
#conf.set('zeus_exec_name', zeus.exec)
if buildtype == 'debug' or buildtype == 'debugoptimized'
	conf.set('rcx_directory', pj_root + 'tests/fake_etc/rcX.d')
	conf.set('run_directory', pj_root + 'tests/run')
	conf.set('caos_config_directory', pj_root + 'tests/fake_etc/caos/init/')
	conf.set('caos_config_dir_user_interactive', pj_root + 'tests/fake_etc/caos/init/interactive/')
	conf.set('caos_config_dir_no_user_interactive', pj_root + 'tests/fake_etc/caos/init/no_interactive/')
else
	conf.set('rcx_directory', '/etc/rcX.d')
	conf.set('run_directory', '/run')
	conf.set('caos_config_directory', '/etc/caos/init/')
	conf.set('caos_config_dir_user_interactive', '/etc/caos/init/interactive/')
	conf.set('caos_config_dir_no_user_interactive', '/etc/caos/init/no_interactive/')
endif

configure_file(
	input: 'config.h.in',
	output: 'config.h',
	configuration: conf
)

