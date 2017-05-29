#!/bin/bash

UBUNTU_VERSION=$(cat /etc/apt/sources.list | grep ^deb | grep ubuntu | grep main | head -n 1 | cut -d\  -f 3)
case $UBUNTU_VERSION in
	precise)
		UBUNTU1X=ubuntu12
		;;
	trusty)
		UBUNTU1X=ubuntu14
		;;
	xenial)
		UBUNTU1X=ubuntu16
		;;
esac

echo "deb http://www.math.uiuc.edu/Macaulay2/Repositories/Ubuntu $UBUNTU_VERSION main" > /etc/apt/sources.list.d/macaulay2.list
apt-key adv --keyserver hkp://keys.gnupg.net --recv-key CD9C0E09B0C780943A1AD85553F8BD99F40DCB31

# echo "deb ftp://jim.mathematik.uni-kl.de/repo/$UBUNTU1X $UBUNTU_VERSION main" > /etc/apt/sources.list.d/singular.list
# wget -q ftp://jim.mathematik.uni-kl.de/repo/extra/gpg -O - | apt-key add -

# apt-add-repository -y ppa:aims/sagemath

add-apt-repository ppa:mc3man/trusty-media

apt-get update
apt-get dist-upgrade
apt-get -y install vim make gcc g++ git gdb m4 unzip libboost-dev ffmpeg zlib1g-dev criu
#apt-get -y install macaulay2 valgrind
#apt-get -y install sagemath-upstream-binary

#mkswap -f /dev/sdb
#swapon /dev/sdb

su vagrant << EOF
test -e groebner || git clone https://github.com/pichtj/groebner.git
EOF
