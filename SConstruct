env = Environment(
    CPPPATH=['#/include'],
    CCFLAGS=['-O2'],
)
Export('env')

for subdir in ['src', 'examples']:
    env.SConscript(dirs=[subdir], variant_dir='build/' + subdir, duplicate=False)
