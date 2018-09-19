# ANON

[![Build Status](https://travis-ci.com/ByeBugDevelopment/anon-backup.svg?token=WBBgtRXJbdCRsjxqqhJy&branch=master)](https://travis-ci.com/ByeBugDevelopment/anon-backup)

**Anonymous Bitcoin v.1.1.1**

ANON is an implementation of the zerocash protocol, bootstrapped with a merge of the Official Bitcoin and Zclassic UTXO sets via a snapshot and airdrop, and with the additional implementation of masternodes.

## Table of Contents

- [Security](#security)
- [Build](#build)
	- [Testnet](#testnet)
	- [Linux](#linux)
	- [Windows](#windows)
	- [Mac](#mac)
- [Usage](#usage)
- [Masternodes](#masternodes)
- [Contact](#contact)
- [Contribute](#contribute)
- [License](#license)


## Security

See important security warnings in
[doc/security-warnings.md](doc/security-warnings.md).

**Anonymous Bitcoin is unfinished and highly experimental.** Use at your own risk.
<!-- ### Any optional sections -->

## Background

[Anonymous Bitcoin](https://www.anonymousbitcoin.io/), like [Zclassic](https://zclassic.org/) and [Zcash](https://z.cash/), is an implementation of the "Zerocash" protocol.
Based on Zclassic's code, it intends to offer a far higher standard of privacy
through a sophisticated zero-knowledge proving scheme that preserves
confidentiality of transaction metadata. Technical details are available
in the Zcash [Protocol Specification](https://github.com/zcash/zips/raw/master/protocol/protocol.pdf).

This software is the Anonymous Bitcoin client. It downloads and stores the entire history
of Anonymous Bitcoin transactions. Depending on the speed of your computer and network
connection, the synchronization process could take a day or more once the
blockchain has reached a significant size.

It includes both `anond` (the daemon) and `anon-cli` (the command line tools).

**To [DOWNLOAD](https://github.com/anonymousbitcoin/anon/releases) pre-built binaries, visit [RELEASES](https://github.com/anonymousbitcoin/anon/releases).**

## Build

### Testnet

Build the latest version of ANON using the instructions below, then follow the [testnet guide](doc/testnet.md)

Please note the Tesnet is running on p2p port: 33129

You should take the appropriate measures to open this port on your firewall to allow incoming/outgoing connections on this port.

### Linux

Update System:
```{r, engine='bash'}
sudo apt-get update

sudo apt-get upgrade
```

Get dependencies:
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Build:
```{r, engine='bash'}
# Checkout
git clone https://github.com/anonymousbitcoin/anon.git
cd anon

# Build
# if this fails. run the same command, instead without the '-j$(nproc)'
./anonutil/build.sh -j$(nproc)

# Fetch Zcash ceremony keys
./anonutil/fetch-params.sh
```

Create Config File:
```{r, engine='bash'}
mkdir ~/.anon
touch ~/.anon/anon.conf
vi ~/.anon/anon.conf
```

Add following lines to `anon.conf` and be sure to change the rpcpassword:
```{r, engine='bash'}
rpcuser=anonrpc
rpcpassword=set-a-password
rpcallowip=127.0.0.1
txindex=1
```

Run:
```{r, engine='bash'}
./src/anond
```

### Windows

Windows is not a fully supported build - however there are two ways to build ANON for Windows:

* On Linux using [Mingw-w64](https://mingw-w64.org/doku.php) cross compiler tool chain. Ubuntu 16.04 Xenial is proven to work and the instructions is for such release.
* On Windows 10 (64-bit version) using [Windows Subsystem for Linux (WSL)](https://msdn.microsoft.com/commandline/wsl/about) and Mingw-w64 cross compiler tool chain.

With Windows 10, Microsoft released a feature called WSL. It basically allows you to run a bash shell directly on Windows in an ubuntu environment. WSL can be installed with other Linux variants, but as mentioned before, the distro proven to work is Ubuntu.
Follow this [link](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide) for installing WSL first

1. Get the usual dependencies:
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake make cmake mingw-w64
```

2. Set the default mingw32 gcc/g++ compiler option to posix, fix problem with packages in Xenial
```{r, engine='bash'}
sudo apt install software-properties-common
sudo add-apt-repository "deb http://archive.ubuntu.com/ubuntu zesty universe"
sudo apt update
sudo apt upgrade
sudo update-alternatives --config x86_64-w64-mingw32-gcc
sudo update-alternatives --config x86_64-w64-mingw32-g++
```

3. Install Rust
```{r, engine='bash'}
curl https://sh.rustup.rs -sSf | sh
source ~/.cargo/env
rustup install stable-x86_64-unknown-linux-gnu
rustup install stable-x86_64-pc-windows-gnu
rustup target add x86_64-pc-windows-gnu
vi  ~/.cargo/config
```

and add:
```{r, engine='bash'}
[target.x86_64-pc-windows-gnu]
linker = "/usr/bin/x86_64-w64-mingw32-gcc"
```

Note that in WSL, the Anon source code must be somewhere in the default mount file system. i.e /usr/src/anon, and not on /mnt/d/. What this means is that you cannot build directly on the windows system

4. Build for Windows

```{r, engine='bash'}
# Checkout
git clone https://github.com/anonymousbitcoin/anon.git
cd anon

PATH=$(echo "$PATH" | sed -e 's/:\/mnt.*//g') # strip out problematic Windows %PATH% imported var
./anonutil/build-win.sh
```

5. Installation

If compiling on linux, use the following command to build the executables in `./depends/x86_64-w64-mingw32/bin/

```{r, engine='bash'}
sudo make install DESTDIR=
```

If compiling using WSL, use the following command to build the executables in `c:\anon\anon

```{r, engine='bash'}
sudo make install DESTDIR=/mnt/c/anon/anon
```

### Mac

Get dependencies:
```{r, engine='bash'}
# Install xcode
xcode-select --install

/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
brew install cmake autoconf libtool automake libevent coreutils pkgconfig gmp wget

brew install gcc5 --without-multilib
```

Install:
```{r, engine='bash'}
# Checkout
git clone https://github.com/anonymousbitcoin/anon.git
cd anon

# Build
./anonutil/build-mac.sh -j$(sysctl -n hw.physicalcpu)

# Fetch keys
./anonutil/fetch-params.sh

# Run
./src/anond
```

### Additional notes

If you plan to build for windows and linux at the same time, be sure to delete all the built files for whatever you build first. An easy way to do this is by taking the binaries out of the repo, delete all files except the .git folder and then do a git hard reset.

When modifying your `anon.conf` file, refer to the [following guide](contrib/debian/examples/anon.conf) for aditional parameters.

## Usage

After installing the full node, you can access the cli in the `/src/` directory with the following command:

```{r, engine='bash'}
./src/anon-cli help
```

The `help` command will pull up all of the avaliable RPC commands that can be executed through the terminal.

## Masternodes

To set up a masternode, refer to the [following guide](/doc/masternode_setup.md).

## Contact

- Visit the Discord or Telegram servers for help and more information.
	- Discord: https://discord.gg/hjeBaBx
	- Telgram: https://t.me/anonymousbitcoin
	
For more information regarding the disclosure of vulnerabilities within ANON, please refer to the [disclosure](https://www.anonfork.io/disclosure) page.

## Contribute

* Code review is welcome!
* If you want to get to know us join our Discord: https://discord.gg/hjeBaBx


Participation in the Anonymous Bitcoin project is subject to a
[Code of Conduct](code_of_conduct.md).

Small note: If editing the Readme, please conform to the [standard-readme](https://github.com/RichardLitt/standard-readme) specification.

## License

For license information see the file [COPYING](COPYING).
