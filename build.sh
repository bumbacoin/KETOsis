#!/bin/bash

set -e

date

#################################################################
# Update Ubuntu and install prerequisites for running KETOsis   #
#################################################################
sudo apt-get update
#################################################################
# Build KETOsis from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building KETOsis           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

# By default, assume running within repo
repo=$(pwd)
file=$repo/src/ketosisd
if [ ! -e "$file" ]; then
	# Now assume running outside and repo has been downloaded and named ketosis
	if [ ! -e "$repo/ketosis/build.sh" ]; then
		# if not, download the repo and name it ketosis
		git clone https://github.com/KETOsisDev/KETOsis-Master.git ketosis
	fi
	repo=$repo/ketosis
	file=$repo/src/ketosisd
	cd $repo/src/
fi
make -j$NPROC -f makefile.unix

cp $repo/src/ketosisd /usr/bin/ketosisd

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.ketosis
if [ ! -e "$file" ]
then
        mkdir $HOME/.ketosis
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | tee $HOME/.ketosis/ketosis.conf
file=/etc/init.d/ketosis
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo ketosisd' | sudo tee /etc/init.d/ketosis
        sudo chmod +x /etc/init.d/ketosis
        sudo update-rc.d ketosis defaults
fi

/usr/bin/ketosisd
echo "KETOsis has been setup successfully and is running..."

