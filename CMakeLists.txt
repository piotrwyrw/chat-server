cmake_minimum_required(VERSION 3.23)
project(ChatServer C)

set(CMAKE_C_STANDARD 99)

add_executable(ChatServer src/main.c src/user.h src/user.c src/server.h src/bool.h src/server.c src/netserver.h src/netserver.c src/msg.h src/msg.c src/netmsg.c src/netmsg.h)
