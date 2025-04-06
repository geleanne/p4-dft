# P4 - Distributed Fault Tolerance

For this exercise, we are tasked to create an **online enrollment system** with different services distributed across multiple nodes.

## Build/Compilation Steps (MinGW)
### Frontend 
1. Open `login.html` with Live Server (VSCode)

### Backend
1. Type `cd backend`
2. Copy paste `g++ src/main.cpp -o course_server.exe -I./include -I./lib -static-libgcc -static-libstdc++ -lws2_32 -lpthread -std=c++17` to the terminal
3. Refresh
