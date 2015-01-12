set nocompatible   " be iMproved
filetype off       " required!

syntax on
set autoindent
set backspace=indent,eol
set showmode		" Always display mode
set scrolloff=2		" Start scrolling x lines from bottom
set number 		" enable line numbers
set showmatch 		" enable matching parenthesis 
set wildmenu		" enable enhanced command line completion
set incsearch		" enable incremental seach
set ignorecase		" see next: use in conjunction with smartcase
set smartcase 		" searchin is case INSENSITIVE for ALL LOWERCASE, otherwise is case sensitive
"set nowrap		" disable text wrapping
"set cul			" Highlight current line
"set backup		" enable backups
"set backupdir=~/.vim_backup " backup directory

"size of a hard tabstop
" set tabstop=4
" size of an indent
set shiftwidth=2

" CHEATSHEET

" WINDOW MANAGEMENT
" ================
	" Vertical Split
	map <Bar>   <C-W>v<C-W><Right>
	" Horizontal Split
	map _       <C-W>s<C-W><Down>
	" Switch Window
	map <Tab>   <C-W>w
	" Reverse switch window
	map <Esc>[Z <C-W>W
	" Close window
	map <C-c>   <C-W>c 
" MAPPINGS
" ==============
	" Write and save
	:command WQ wq
	:command Wq wq
	:command W w
	:command Q q	

