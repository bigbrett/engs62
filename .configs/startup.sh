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


# setup solarized
echo; echo "Setting up Solarized Color Scheme...."; echo; echo
bash ~/workspace/.configs/gnome-terminal-colors-solarized/install.sh
eval `dircolors ~/workspace/.configs/dircolors-solarized/dircolors.256dark` 

if test -f "~/.vim/colors/solarized.vim"; then {
	echo "colors updated"
}; else {
  mkdir ~/.vim/colors
  mv ~/workspace/.configs/vim-colors-solarized/colors/solarized.vim ~/.vim/colors/
}; 
fi


echo; echo "Dev environment restored....happy hacking!!"
