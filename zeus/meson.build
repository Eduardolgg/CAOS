project('Zeus', 'c', 
	license: 'GPLV2',
	meson_version: '>=0.39.1',
	version: '0.1.1b'
)

buildtype = get_option('buildtype')

#add_global_arguments('-lutil',language : 'c')
#add_global_link_arguments('-lutil',language : 'c')

inc = include_directories('include')
subdir('include')
subdir('src')
subdir('tests')

