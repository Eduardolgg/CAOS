thread_dep = dependency('threads')

sources = [
	'main.c', 'log.c', 'runlevel_utils.c', 'filesystem.c',
	'run.c', 'toml.c', 'script_config.c', 'serial_start.c',
	'low_parallel_start.c', 'processes.c'
]
zeus = executable(
	'zeus',
	sources,
	include_directories: inc,
	install: true,
	install_dir: '/sbin',
	dependencies: thread_dep,
	link_args: '-lutil'
)
