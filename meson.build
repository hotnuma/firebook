project(
    'firebook',
    ['cpp'],
    version : '1.0',
    license : 'GPL-2.0',
    default_options : [
        'cpp_std=c++11',
    ],
)

app_deps = [
    dependency('tinycpp'),
]

cpp_args = [
    '-Wall',
    '-fno-rtti',
    '-fno-exceptions',
    '-O2',
]

app_src = [
    'main.cpp',
]

executable(
    'firebook',
    dependencies : app_deps,
    cpp_args : cpp_args,
    sources : app_src,
    install : true,
)


