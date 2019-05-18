import os

install_path = '../dump'
modules = ['nodeeditor']

os.system('mkdir build')
os.chdir('build')
for module in modules:
    os.system(f'cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX:PATH={install_path}  ../../3rdparty/{module}')
    os.system('cmake --build . --target install')

os.chdir('..')
os.system('rm -rf build')