## NeptuneVM project structure

- base
	- core library. provides asio defines and implementation (we compile asio seperately here), and other stuff we might want to use elsewhere
		- consumes Boost.Asio & others

- vm
	- Virtual machine management/agent client library; provides QEMU VM support, VNC & AgentBlast display protocols

- neptuned
	- Actual NeptuneVM server source code. This is where the fun is.
