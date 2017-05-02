thread_dep = dependency('threads')

sources = ['main.c', 'log.c', 'runlevel_utils.c', 'filesystem.c',
           'serial_start.c', 'low_parallel_start.c'
]
zeus = executable(
	'zeus',
	sources,
	include_directories: inc,
	install: true,
	install_dir: '/sbin',
	dependencies: thread_dep
)
