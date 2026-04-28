# Celeris 

**Celeris** is a lightweight, high-performance web server written in modern C++. It allows developers to spin up custom HTTP servers using a simple, HTML-like configuration system. Celeris is designed for speed, flexibility, and ease of use — whether you're building APIs, serving static files, or experimenting with new ideas.

---

##  Features

-  **Fast and Lightweight** – Built in C++ for maximum performance.
-  **Custom Configuration** – Define your server behavior using intuitive HTML-like syntax.
-  **Routing** – Easily create and manage routes.
-  **Authentication Support** – Secure endpoints with built-in auth mechanisms.
-  **Static File Serving** – Serve HTML, CSS, JS, and media files.
-  **Multithreading** – Handles concurrent requests efficiently.
-  **Extensible** – Built to support plugins and middleware in the future.

---

##  Installation

> Coming soon: Install via `.exe` installer, package manager, or source.

### Cross-Platform Build Instructions

Celeris now supports Windows, Linux, macOS, and other Unix-like systems with compile-time OS detection.

#### Prerequisites
- **C++20 compatible compiler** (GCC 9+, Clang 10+, MSVC 2019+)
- **CMake 3.16+**
- **Platform-specific libraries** (automatically linked by CMake)

#### Build Commands

**Windows (Visual Studio):**
```bash
git clone https://github.com/amethystcoder/Web-Serve.git
cd Web-Serve
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

**Linux/macOS:**
```bash
git clone https://github.com/amethystcoder/Web-Serve.git
cd Web-Serve
mkdir build && cd build
cmake ..
make -j$(nproc)
```

**Alternative (make):**
```bash
git clone https://github.com/amethystcoder/Web-Serve.git
cd Web-Serve
make   # or cmake . && make
./Celeris
```

### Platform Features
- **Windows**: Native Winsock2 support, process management via Windows API
- **Linux/macOS**: POSIX socket support, process management via standard Unix APIs
- **Cross-platform**: Compile-time OS detection ensures optimal performance on each platform

---

##  Configuration

Celeris uses a lightweight markup-style config file:

```html
<server port='5000' message='server is running on port {self.port}'>
    <ratelimit maxRequests='100' per='60s' name='someratelimitname' message="rate limiter is created... or something"/>
    <filesystem name='thefilesys' path='/' />
    <api>
        <route endpoint='/name' method='GET' rescontenttype='html' response='HTMLPage.html' rateLimit='someratelimitname'></route>
        <route endpoint='/stats' method='GET' rescontenttype='html' response='kjfdkj.html'></route>
        <route endpoint='/wonder' method='GET' rescontenttype='json' response='some file.json'></route>
        <route endpoint='/equip' method='GET' rescontenttype='html' response='Hello, You are in the equip page!'></route>
    </api>
</server> 
```

More details and examples coming soon.

---

##  Project Structure

```bash
celerisd/            # Core source code
configs/             # Sample server config files
public/              # Static assets (optional)
docs/                # Documentation (in progress)
tests/               # Unit and integration tests
```

---

##  Running Tests

```bash
make test
# or
./run_tests
```

---

## Roadmap

- [x] Basic routing and static server
- [x] HTML-like configuration parsing
- [ ] Request handling concurrency
- [ ] Middleware support
- [ ] Plugin system
- [ ] HTTPS support
- [ ] WebSocket integration

---

##  Contributing

Contributions are welcome! Please open an issue or PR.

1. Fork the repo
2. Create your feature branch (`git checkout -b feature/foo`)
3. Commit your changes (`git commit -am 'Add foo'`)
4. Push to the branch (`git push origin feature/foo`)
5. Open a pull request

---

##  License

MIT License. See `LICENSE` file for details.

---

##  Acknowledgments

Developed with ❤️ by [Joshua Collins](https://github.com/amethystcoder).
