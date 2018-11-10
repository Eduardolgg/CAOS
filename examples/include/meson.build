
conf = configuration_data()
conf.set('version', meson.project_version())
conf.set('app_name', meson.project_name())

configure_file(
	input: 'config.h.in',
	output: 'config.h',
	configuration: conf
)

