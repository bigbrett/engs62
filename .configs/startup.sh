#!/bin/bash
cd ~/workspace

sudo apt-get install vim 

#git config --global user.name "Brett: ARMDEV"
#git config --global user.email Brett.r.nicholas.15@dartmouth.edu
git status

read -p "Would you like to pull from remote git repository? (y/n) > \n" -n 1 
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then 
	git pull origin master 
fi 

[! -f ~/workspace/.configs/.vimrc] && [echo ".vimrc not found"; exit 2]
cat ~/workspace/.configs/.vimrc > ~/.vimrc
	

