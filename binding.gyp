{
  'targets': [
    {
      'target_name': 'eac',
      'sources': [
        'src/addon.cpp',
        'src/Constants.hpp',
        'src/Constants.cpp',
        'src/Context.hpp',
        'src/Context.cpp',
        'src/SharedSecret.hpp',
        'src/SharedSecret.cpp',
        'src/PACE.hpp',
        'src/PACE.cpp',
        'src/SM.hpp',
        'src/SM.cpp',
      ],
      'includes': [
        'except.gypi',
      ],
      'include_dirs': [
        '<!@(node -p \'require("node-addon-api").include\')',
        '<!@(pkg-config --cflags-only-I libeac | sed s/-I//g)'
      ],
      'libraries': [
        '<!@(pkg-config --libs libeac)',
      ],
    },
  ],
}
