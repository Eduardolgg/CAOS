thread_dep = dependency('threads')

sources = ['pty_example.c']

pty_example = executable(
	'pty_example',
	sources,
	include_directories: inc,
	install: true,
	install_dir: '../bin',
	dependencies: thread_dep,
	link_args: '-lutil'
)

sources_colorized_output_from_child = ['colorized_output_from_child.c']

colorized_output_from_child = executable(
	'colorized_output_from_child',
	sources_colorized_output_from_child,
	include_directories: inc,
	install: true,
	install_dir: '../bin',
	dependencies: thread_dep,
	link_args: '-lutil'
)
