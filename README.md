# LetsScore

## Two player dice roll game based on client-server architecture in C

* Simulated two-player dice roll game to attain a specified maximum score
* Systems programming in C to implement a client-server architecture
* Parent-child processes [fork()] and sockets used to establish communication between referee (server) and players (clients)


## Steps to Run

* `Clone https://github.com/gagannagpal131/LetsScore`

* `Change to "LetsScore" as the current directory`

* `Open 3 Terminal windows`

* `gcc server.c -o server`

* `gcc client.c -o client`

* `./server <Port #>`

* `./client on 2 Terminal windows (one for each player)`

* `./client <IP> <Port #> <Player Name>`

## Screenshots

<img src="https://github.com/gagannagpal131/LetsScore/blob/master/screenshots/ss1.png" width="650">

<img src="https://github.com/gagannagpal131/LetsScore/blob/master/screenshots/ss2.png" width="650">

<img src="https://github.com/gagannagpal131/LetsScore/blob/master/screenshots/ss3.png" width="650">

<img src="https://github.com/gagannagpal131/LetsScore/blob/master/screenshots/ss4.png" width="650">
