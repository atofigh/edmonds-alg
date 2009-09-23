# Set up the command line options of the SConstruct file
opts = Variables()
opts.AddVariables(
    PathVariable('boost', 'where the root of boost is installed', '/usr'),
    )
env = Environment(options = opts)
Help(opts.GenerateHelpText(env))

# Set up boost flags
env.AppendUnique(CPPPATH = [env.subst('$boost/include')],
                 LIBPATH = [env.subst('$boost/lib')])

# Build the documentation
algorithm_description = env.PDF(target = 'doc/algorithm-description/algorithm-description.pdf',
                                source = 'doc/algorithm-description/algorithm-description.latex')

# Build the test program
test_program = env.Program(target = 'test/test',
                           source = 'test/test.cpp',
                           CCFLAGS = '-O2')

# Define a test target that runs the test scripts
test_target = env.Command('test', None, '@cd test; python test.py')
env.Depends(test_target, test_program)
env.AlwaysBuild(test_target)

env.Default(algorithm_description, test_program)
