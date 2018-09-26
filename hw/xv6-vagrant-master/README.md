# ics143a-vagrant
A Vagrant machine setup for ics143a Course

# How to use

1. Create a new directory

	```
	$ mkdir ~/ics143a/
	```

2. Download a zip of the repository from [Here](https://github.com/kensaggy/os152-vagrant/archive/master.zip), extract it and place `Vagrantfile` and the `manifest` directory inside `os152`

3. Create a clone of BGU's xv6 fork in your working directory:

	```
	$ cd ~/os152/
	$ git clone http://www.cs.bgu.ac.il/~os152/xv6.git
	```

4. Bring up your new vagrant machine

	```
	$ vagrant up
	```
	
5. Profit! 

	You're new vagrant machine should have everything you need to compile and run your xv6 code.
	Vagrant automatically shares the local machine directory the `Vagrantfile` is in at `/vagrant` directory of the virtual machine.
	You can use `vagrant ssh` on your local machine to get a ssh session on the virtual machine.
    Remember: Since your virtual machine is running in headless mode, so you can't run QEMU the normal way (in a window of it's own) - so when you want to run the xv6 image, use `make qemu-nox` instead of `make qemu`


## Tip
Since we're running QEMU in headless mode (`make clean qemu-nox`) you don't have a GUI window to close whenever you want.. if you want to exit the xv6 OS you need to kill
the QEMU process from another terminal.
A useful shortcut is to define an `alias` in your local machine as follows:

```
alias kill-qemu='vagrant ssh -c "killall qemu-system-i386"'
```
Add this to your `~/.bash_profile` or `~/.zshrc` file to make it persistant
This will send the `killall qemu-system-i386` command over ssh to your vagrant machine.
Notice this command will only work if you're running it from somewhere in the directory path of the Vagrantfile running this machine

