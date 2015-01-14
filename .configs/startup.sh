#!/bin/bash

# Start up shell-script to restore dev environment on immutable system
# Author : Brett Nicholas

cd ~/workspace

# ensure git is up to date
git status

# Prompt user to pull from remote git repo
read -p "Would you like to pull from remote git repository? (y/n) > " -n 1 
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then 
  git pull origin master 
fi 

# Install Vim because Vi sucks
echo; echo; echo "Checking for Vim...(sudo password: armdev"; echo; echo
sudo apt-get install vim 
echo; echo; echo "Checking for .vimrc file..."; echo
if test -f "/home/armdev/workspace/.configs/.vimrc"; then {
  echo; echo "updating .vimrc file..."; echo
  cat ~/workspace/.configs/.vimrc > ~/.vimrc
}; else
  echo "ERROR... Could not locate ~/workspace/.configs/.vimrc"
fi	
echo; echo "Dev environment restored....happy hacking!!"
