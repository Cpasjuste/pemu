[![Windows](https://github.com/continental/fineftp-server/actions/workflows/build-windows.yml/badge.svg)](https://github.com/continental/fineftp-server/actions/workflows/build-windows.yml) [![Ubuntu](https://github.com/continental/fineftp-server/actions/workflows/build-ubuntu.yml/badge.svg)](https://github.com/continental/fineftp-server/actions/workflows/build-ubuntu.yml) [![macOS](https://github.com/continental/fineftp-server/actions/workflows/build-macos.yml/badge.svg)](https://github.com/continental/fineftp-server/actions/workflows/build-macos.yml)

# fineFTP Server

FineFTP is a minimal FTP server library for Windows and Unix flavors. The project is CMake based and only depends on asio, which is integrated as git submodule. No boost is required.

You can easily embed this library into your own project in order to create an embedded FTP Server. It was developed and tested on Windows 10 (Visual Studio 2015 / 2019, MinGW) and Ubuntu 16.04 - 21.10 (gcc 5.4.0 - 11.2.0).

## Features

- FTP Passive mode (the only mode you need nowadays)
- Listing directories
- Uploading and downloading files
- Creating and removing files and directories
- User authentication (and anonymous user without authentication)
- Individual local home path for each user
- Access control on a per-user-basis
- UTF8 support (On Windows MSVC only)

*fineFTP does not support any kind of encryption. You should only use fineFTP in trusted networks.*

## Example

Using fineFTP in your application is simple. Just create an FtpServer object, add one or multiple users and start the server.

```cpp
#include <fineftp/server.h>
#include <thread>
 
int main() {
  // Create an FTP Server on port 2121. We use 2121 instead of the default port
  // 21, as your application would need root privileges to open port 21.
  fineftp::FtpServer ftp_server(2121);
 
  // Add the well known anonymous user. Clients can log in using username
  // "anonymous" or "ftp" with any password. The user will be able to access
  // your C:\ drive and upload, download, create or delete files. On Linux just
  // replace "C:\\" with any valid path. FineFTP is designed to be cross-platform.
  ftp_server.addUserAnonymous("C:\\", fineftp::Permission::All);
  
  // Start the FTP Server with a thread-pool size of 4.
  ftp_server.start(4);
 
  // Prevent the application from exiting immediately
  for (;;) std::this_thread::sleep_for(std::chrono::milliseconds(100));
  return 0;
}
```

## How to checkout and build

There is an example project provided that will create an FTP Server at `C:\` (Windows) or `/` (Unix).

1. Install cmake and git / git-for-windows

2. Checkout this repo and the asio submodule
	```console
	git clone https://github.com/continental/fineftp-server.git
	cd fineftp-server
	git submodule init
	git submodule update
	```

3. CMake the project *(Building as debug will add some debug output that is helpful so see if everything is working)*
	```console
	mkdir _build
	cd _build
	cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=_install
	```

4. Build the project
	- Linux: `make`
	- Windows: Open `_build\fineftp.sln` with Visual Studio and build the example project

5. Start `example` / `example.exe` and connect with your favorite FTP Client (e.g. FileZilla) on port 2121 *(This port is used so you don't need root privileges to start the FTP server)*


## Contribute

Awesome, you want to contribute to FineFTP? Here is how you can do that!

- Leave us a star ⭐️ (That's GitHub money!)
- Create an issue and write about a feature you would like or a bug you have found (maybe we will find some spare time to implement it 😉)
- Fork this repository, implement the feature yourself and create a pull request
