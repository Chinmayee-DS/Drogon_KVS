Step 1. Install drogon application using the following commands.

Install by source in Linux\
cd $WORK_PATH\
git clone https://github.com/drogonframework/drogon

cd drogon\
git submodule update --init\
mkdir build\
cd build\
cmake ..\
make && sudo make install

The default is to compile the debug version. If you want to compile the release version, the cmake command should take the following parameters:

cmake -DCMAKE_BUILD_TYPE=Release ..

After the installation is complete, the following files will be installed in the system（One can change the installation location with the CMAKE_INSTALL_PREFIX option）:

The header file of drogon is installed into /usr/local/include/drogon;\
The drogon library file libdrogon.a is installed into /usr/local/lib;\
Drogon's command line tool drogon_ctl is installed into /usr/local/bin;\
The trantor header file is installed into /usr/local/include/trantor;\
The trantor library file libtrantor.a is installed into /usr/local/lib;


Step 2. compile each of the individual instances.

  Step 2a. Go to the build directory of each of the instances. Run the following commands.\
            cmake ..\
            make

Step 3. Make sure 127.0.0.1:8080, 127.0.0.2:8081, 127.0.0.3:8081 ips are up and accessible. 

Step4. Run each of the instances using the below command in the build directory. 

      ./fourhwsc
      ./twohwsc
      ./threehwsc



Step 5. Run the benchmark.py script. Client side partitioning has been implemented in the benchmark script.
