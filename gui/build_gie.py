import os
os.system("mkdir build_gie")
os.system("cd build_gie && cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX:PATH=./ ../../gie && make && make install")