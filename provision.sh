#!/bin/bash

UBUNTU_VERSION=$(cat /etc/apt/sources.list | grep ^deb | grep ubuntu | grep main | head -n 1 | cut -d\  -f 3)
case $UBUNTU_VERSION in
	precise)
		UBUNTU1X=ubuntu12
		;;
	trusty)
		UBUNTU1X=ubuntu14
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
apt-get -y install vim make gcc g++ macaulay2 git gdb valgrind m4 unzip libboost1.54-dev ffmpeg
#apt-get -y install sagemath-upstream-binary

#mkswap -f /dev/sdb
#swapon /dev/sdb

su vagrant << EOF
git config --global user.email "jonathan.picht@jpicht.de"
git config --global user.name "Jonathan Picht"
git config --global push.default simple
cd
test -e moGVW || (git clone /vagrant moGVW && cd moGVW && git checkout master)
EOF
