Exec {
    path        => '/usr/bin:/bin:/usr/sbin:/sbin',
}

exec { 'dpkg-reconfigure':
  command   => 'dpkg-reconfigure locales',
  user      => 'root'
}

exec { 'apt-update':
  command   => '/usr/bin/apt-get update',
  user      => 'root'
}

exec { 'apt-upgrade':
  command   => '/usr/bin/apt-get upgrade -y',
  user      => 'root',
  require   => Exec['apt-update']
}

package {
  ["gcc-multilib", "gdb", "libc6-dbg:i386", "git", "libsdl1.2-dev", "build-essential", "libtool", "autoconf", "libpixman-1-0", "libpixman-1-dev"]:
    ensure  => present,
    require => Exec['apt-update']
}

exec { 'get-qemu':
  command   => '/usr/bin/git clone http://web.mit.edu/ccutler/www/qemu.git -b 6.828-2.3.0',
  cwd       => '/tmp',
  user      => 'root',
  onlyif    => 'test ! -f /usr/local/bin/qemu-system-i386',
  creates   => '/tmp/qemu',
  require   => Package['git']
}

exec { 'configure-qemu':
  command   => '/tmp/qemu/configure --disable-kvm --target-list="i386-softmmu x86_64-softmmu"',
  cwd       => '/tmp/qemu/',
  user      => 'root',
  onlyif    => 'test ! -f /usr/local/bin/qemu-system-i386',
  require   => [ Exec['get-qemu'],
                Package['gcc-multilib','build-essential','libtool','autoconf','libsdl1.2-dev']
                ]
}

exec { 'make-qemu':
  command   => 'make',
  cwd       => '/tmp/qemu/',
  user      => 'root',
  timeout   => 1800,
  onlyif    => 'test ! -f /usr/local/bin/qemu-system-i386',
  require   => Exec['configure-qemu']
}

exec { 'install-qemu':
  command   => 'make install',
  cwd       => '/tmp/qemu/',
  user      => 'root',
  creates   => '/usr/local/bin/qemu-system-i386',
  require   => Exec['configure-qemu','make-qemu']
}

