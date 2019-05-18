import os

install_path = '../dump'
modules = ['nodeeditor', '../gie']

os.system('mkdir build')
os.chdir('build')
for module in modules:
    dir = module.split('/')[-1]
    try:
        os.mkdir(f'{dir}')
    except:
        pass
    os.chdir(f'{dir}')
    os.system(f'cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX:PATH=../{install_path}  ../../../3rdparty/{module}')
    os.system('cmake --build . --target install -j4')
    os.chdir('..')


os.chdir('..')