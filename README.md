# Hybrid High Performance Parallel Computing
Distributed High Performance Parallel Computing example project which employs OpenMPI, OpenMP C++ Libraries on Google Cloud Infrastructure.

# Project Architecture

<img width="695" alt="screen shot 2019-01-15 at 1 32 17 am" src="https://user-images.githubusercontent.com/2838457/51145684-ad505f80-1865-11e9-94ad-96d019fe8969.png">


# Eclipse IDE for Scientific Computing Installation

We will utilize Eclipse IDE to develop C/C++ projects.

![screenshot from 2019-01-15 01 02 44](https://user-images.githubusercontent.com/2838457/51144492-38c7f180-1862-11e9-92a5-736adf300923.png)

![screenshot from 2019-01-15 01 02 54](https://user-images.githubusercontent.com/2838457/51144491-38c7f180-1862-11e9-9b82-f7bdbdb98e98.png)

We give "-fopenmp" command to C/C++ compilers to support OpenMP library. With this way, wecan utilize shared memory parallelization on multi-core CPUs. Our virtual machines are 4 CPU or 2 CPU.

![screenshot from 2019-01-15 01 05 14](https://user-images.githubusercontent.com/2838457/51144490-382f5b00-1862-11e9-8460-101686586dbb.png)

![screenshot from 2019-01-15 01 05 24](https://user-images.githubusercontent.com/2838457/51144489-382f5b00-1862-11e9-824c-2417d90ea12c.png)


We can run OpenMPI on our local machine. Eclipse gives an opportunity to run it as similation. Wecan configure settings from "Run Configurations" panel.

![screenshot from 2019-01-15 01 06 27](https://user-images.githubusercontent.com/2838457/51144488-382f5b00-1862-11e9-9228-5e2b071b90d0.png)

![screenshot from 2019-01-15 01 06 34](https://user-images.githubusercontent.com/2838457/51144486-382f5b00-1862-11e9-961a-2d3c385614f5.png)


OpenMPI & OpenMP results in our local machine.

![screenshot from 2019-01-15 01 09 31](https://user-images.githubusercontent.com/2838457/51144483-3796c480-1862-11e9-8db7-7e25b54cdf20.png)



# Install Google Cloud SDK on your local machine. 

Create environment variable for correct distribution
```console
export CLOUD_SDK_REPO="cloud-sdk-$(lsb_release -c -s)"
```

Add the Cloud SDK distribution URI as a package source
```console
echo "deb http://packages.cloud.google.com/apt $CLOUD_SDK_REPO main" | sudo tee -a /etc/apt/sources.list.d/google-cloud-sdk.list
```

Import the Google Cloud Platform public key
```console
curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
```

Update the package list and install the Cloud SDK
```console
sudo apt-get update && sudo apt-get install google-cloud-sdk
```


# Google Compute Engine (Ubuntu 18.04 VM On Google Cloud)

We will install necessary tools and developments packages to use in Google Cloud Compute Engine Virtual Machine instances. C/C++ compiler will be installed after running these commands.
```console
sudo apt-get update
sudo apt-get install gcc
sudo apt-get install g++
sudo apt install make
```

# 1- OpenMPI Installation Steps

This library is used in high-performance computing. It bring low latency and high throughput. It is also  designed to be scalable.
```console
sudo apt-get install libibnetdisc-dev
```

Create a folder and change directory
```console
mkdir openmpi
cd openmpi
```

We have to download OpenMPI installation packages
```console
wget https://download.open-mpi.org/release/open-mpi/v3.1/openmpi-3.1.3.tar.gz
```

Extract packages from zipped file.
```console
gunzip openmpi-3.1.3.tar.gz
tar -xvf openmpi-3.1.3.tar
cd openmpi-3.1.3/
```

Now, we will configure and install OpenMPI tools.
```console
./configure --prefix="/home/$USER/.openmpi"
make
sudo make install
```

Setup path in Environment Variable.
```console
export PATH="$PATH:/home/$USER/.openmpi/bin"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/home/$USER/.openmpi/lib/"
```

We will need these tools to run MPI tasks on command line interface.
```console
sudo apt-get install openmpi-bin
sudo apt install libopenmpi-dev
```

# 2- OpenMP Installation Steps

We will employ OpenMP to take advantage of shared memory based parallelization on multi-core CPUs.
```console
sudo apt-get install libomp-dev
```

We can check whether OpenMP is installed or not.
```console
echo | cpp -fopenmp -dM | grep -i open
```
Output:
```console
define _OPENMP 201107
```

# Create Snapshot and Image from VM Instance

After we installed all tools and libraries. We will create a snapshot to create a custom VM image.

![screenshot from 2019-01-15 01 00 21](https://user-images.githubusercontent.com/2838457/51144495-39f91e80-1862-11e9-98c7-f33b4ce246c8.png)

![screenshot from 2019-01-15 01 00 50](https://user-images.githubusercontent.com/2838457/51144494-39608800-1862-11e9-8c57-8403c74fa9b0.png)

![screenshot from 2019-01-15 01 01 22](https://user-images.githubusercontent.com/2838457/51144493-39608800-1862-11e9-9852-20497fe666b5.png)


Note!!! After VM instances are fired up, we should add master machine's ssh key, which named as "mpi-instance", to worker machines' authorized_keys file. With this way, master can connect to other machines without password via ssh connection.

After that, we should copy main.c file from local desktop to all other Google Cloud VM instances. We employed "gcloud compute copy-files" command.


# Install Google Cloud SDK and Run OpenMPI Project on Cloud Platform

# 1- Install Google Cloud SDK on your local machine

Download Mac OS package "google-cloud-sdk-237.0.0-darwin-x86_64.tar.gz"

```console
python -V  (Python 2.7)

cd ~/Desktop/google-cloud-sdk

./install.sh

gcloud init
```

# 2- Create Hostfile for OpenMPI Master Node

We prepare host file on our local machine via google cloud commands. After that, we have to copy hostfile to master node which named as "mpi-instance".

```console
gcloud compute instances list
```

```console
gcloud compute instances list | awk 'NR>1 {print $4}' > hostfile
```

# 3- Connect to Master Node 

```console
gcloud compute --project "white-position-228408" ssh --zone "ZONE_NAME" "INSTANCE_NAME"

cd ~/.ssh

ssh-keygen

cat id_rsa.pub
```

# 4- Add Master's SSH Key to Worker Machines

List compute engine instances

```console
gcloud compute instances list
```

We have 3 worker virtual machines on cloud platform.

For Worker-2, Worker-3, Worker-4

```console
gcloud compute --project "white-position-228408" ssh --zone "ZONE_NAME" "INSTANCE_NAME"

cd ~/.ssh/

vim authorized_keys

Add Master's ssh key
```

# 5- Run OpenMPI

Compile source code

```console
mpicc -fopenmp main.c

mpirun -np 4 -mca btl ^openib  --hostfile /YOUR_PATH_VM_MACHINE/hostfile ./a.out
```


# Distributed Parallel Computing results on Google Cloud Platform

Create a.out file. Compile "main.c" C file.

mpicc -fopenmp main.c

Run mpirun command to start distributed parallel computing.

```console
mpirun -np 4 -mca btl ^openib  --hostfile /home/batuxbey/openmpi/hostfile ./a.out
```

<img width="1436" alt="screen shot 2019-01-15 at 12 02 09 am" src="https://user-images.githubusercontent.com/2838457/51145685-ad505f80-1865-11e9-9ffd-87fffc07947d.png">



# References
https://computing.llnl.gov/tutorials/mpi/
http://butka.me/linux/how-to-install-openmpi-on-ubuntu-14-04/
https://www.reidatcheson.com/c/programming/hpc/mpi/cloud/2015/12/27/mpi-on-gcompute.html
http://mpitutorial.com/tutorials/
http://www.slac.stanford.edu/comp/unix/farm/mpi_and_openmp.html
https://computing.llnl.gov/tutorials/openMP/samples/C/omp_dotprod_hybrid.c
https://computing.llnl.gov/tutorials/mpi/
https://computing.llnl.gov/tutorials/mpi/exercise.html
https://cloud.google.com/sdk/docs/quickstart-debian-ubuntu
https://cloud.google.com/compute/docs/images/create-delete-deprecate-private-images#bundle_image
https://cloud.google.com/compute/docs/instances/transfer-files#gcstransfer
https://cloud.google.com/sdk/gcloud/reference/compute/instances/list
https://help.eclipse.org/luna/index.jsp?topic=%2Forg.eclipse.ptp.doc.user%2Fhtml%2Frms%2FopenMPI.html
https://github.com/open-mpi/ompi/issues/3158
https://medium.com/swlh/openmp-on-ubuntu-1145355eeb2
http://supercomputingblog.com/category/openmp/
http://www.shodor.org/media/content/petascale/materials/UPModules/GameOfLife/Life_Module_Document_pdf.pdf
http://www.red-ricap.org/documents/1071192/1486573/OpenMP_01_Introduction.pdf/2a2c91a8-60cf-4716-9ae4-57684a56e4b8
https://computing.llnl.gov/tutorials/openMP/
https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html
https://www.codingame.com/playgrounds/349/introduction-to-mpi/non-blocking-communications---exercise
https://hpc.llnl.gov/sites/default/files/DavidCronkSlides.pdf
http://jakascorner.com/blog/2016/06/omp-for-scheduling.html
