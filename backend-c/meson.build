project('Backend in C', 'c')

open_ssl = dependency('openssl')

executable(
	'backend',
	['main.c', 'server.c', 'request.c', 'response.c', 'lib.c', './json-str.c', './types.c', './cJSON/cJSON.c'],
	include_directories: '.',
	dependencies: [open_ssl],
)
