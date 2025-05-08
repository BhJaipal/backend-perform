project('Backend in C++', 'cpp')

jsoncpp = dependency('jsoncpp')
open_ssl = dependency('openssl')

executable(
	'backend',
	['main.cpp', 'server.cpp', 'request.cpp', 'response.cpp', 'lib.cpp'],
	include_directories: '.',
	dependencies: [jsoncpp, open_ssl],
)