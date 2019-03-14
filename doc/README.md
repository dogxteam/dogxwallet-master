dogxcoin Core
=============

Setup
---------------------
dogxcoin Core is the original dogxcoin client and it builds the backbone of the network. It downloads and, by default, stores the entire history of dogxcoin transactions, which requires a few hundred gigabytes of disk space. Depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download dogxcoin Core, visit [dogxcoincore.org](https://dogxcoincore.org/en/download/).

Running
---------------------
The following are some helpful notes on how to run dogxcoin Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/dogxcoin-qt` (GUI) or
- `bin/dogxcoind` (headless)

### Windows

Unpack the files into a directory, and then run dogxcoin-qt.exe.

### macOS

Drag dogxcoin Core to your applications folder, and then run dogxcoin Core.

### Need Help?

* See the documentation at the [dogxcoin Wiki](https://en.dogxcoin.it/wiki/Main_Page)
for help and more information.
* Ask for help on [#dogxcoin](http://webchat.freenode.net?channels=dogxcoin) on Freenode. If you don't have an IRC client, use [webchat here](http://webchat.freenode.net?channels=dogxcoin).
* Ask for help on the [dogxcoinTalk](https://dogxcointalk.org/) forums, in the [Technical Support board](https://dogxcointalk.org/index.php?board=4.0).

Building
---------------------
The following are developer notes on how to build dogxcoin Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [macOS Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)
- [Windows Build Notes](build-windows.md)
- [FreeBSD Build Notes](build-freebsd.md)
- [OpenBSD Build Notes](build-openbsd.md)
- [NetBSD Build Notes](build-netbsd.md)
- [Gitian Building Guide (External Link)](https://github.com/dogxcoin-core/docs/blob/master/gitian-building.md)

Development
---------------------
The dogxcoin repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/dogxcoin/doxygen/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [Travis CI](travis-ci.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [dogxcoinTalk](https://dogxcointalk.org/) forums, in the [Development & Technical Discussion board](https://dogxcointalk.org/index.php?board=6.0).
* Discuss project-specific development on #dogxcoin-core-dev on Freenode. If you don't have an IRC client, use [webchat here](http://webchat.freenode.net/?channels=dogxcoin-core-dev).
* Discuss general dogxcoin development on #dogxcoin-dev on Freenode. If you don't have an IRC client, use [webchat here](http://webchat.freenode.net/?channels=dogxcoin-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [dogxcoin.conf Configuration File](dogxcoin-conf.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [ZMQ](zmq.md)
- [PSBT support](psbt.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
