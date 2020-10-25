## Description

This is a Client-Server Application. Client reads
messages of any length from the console and sends them to the server (ASCII characters only). The response which received from the server fill via console. The protocol over which the client sends messages is specified when it is started using as a command line parameter. The server accepts text messages from the client using UDP and TCP protocols.

The server processes the received message as follows:
1. Extract numbers.
2. Process:
   1. Sort numbers.
   2. Sum them.
3. Send two strings:
   1. Sorted numbers.
   2. Their sum.

Client and server are based on linux Berkeley Sockets.

The message max length = 1024 bytes. Exceeding the length is ignored by the server

## Build

```
git clone https://github.com/connest/BerkeleySockets
cd BerkeleySockets

mkdir build
cd build

cmake ..
make -j4
```

## Usage

### UDP

```
./Server -p <port>
./Client -h <ip-address> -p <port> --UDP
```
### TCP

```
./Server -p <port>
./Client -h <ip-address> -p <port> --TCP
```

## Example

Client:

```
20 apples, 30 bananas, 15 peaches and 1 watermelon
```

Server:

```
1 15 20 30
66
```