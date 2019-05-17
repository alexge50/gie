{
    "targets": [{
        "target_name": "gie",
        "sources": [
            "gie/main.cpp"
        ],
        "cflags_cc": ["-std=c++17"],
        'cflags_cc!': [ "-fno-rtti", "-std=c++11", "-fno-exceptions" ],
        "include_dirs": ["build_gie/include", "<!@(node -p \"require('node-addon-api').include\")", "/usr/include/python3.7m"],
        "libraries": ["../build_gie/lib/libgie.a"],
        "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
        "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
        'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'CLANG_CXX_LIBRARY': 'libc++',
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
        },
            'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1 },
        },
        "actions": [{
                    "action_name": "gie",
                    "inputs": [],
                    "outputs": [
                        "build_gie/lib/libgie.a",
                        "build_gie/include"
                    ],
                    "action": ["python", "build_gie.py"]
                }]
    }]
}