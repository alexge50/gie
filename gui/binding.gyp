{
    "targets": [{
        "target_name": "gie",
        "sources": [
            "gie/main.cpp"
        ],
        "include_dirs": ["../build_gie/include", "<!@(node -p \"require('node-addon-api').include\")"],
        "libraries": ["../build_gie/lib/libgie.a"],
        "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
        "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
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