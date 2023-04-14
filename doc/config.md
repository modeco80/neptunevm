## Saparid config

Use capnp text format?

```capnp
# This is an example saparid configuration file

using Neptuned = import "Neptuned.capnp";

# This will define the configuration for saparid to use
const config :Saparid.Config = (

	# The IP address to listen on
	listen = "0.0.0.0",

	# The base port of the saparid server. Unless manually overridden,
	# each world will take (base + list_index).
	basePort = 5126,

	# this is where a SAPARi client distribution is expected to be,
	# to grab the required VRML files. (IDEA ONLY)
	#clientRoot = "/srv/sapari",

	# Finally, this defines the worlds your saparid instance will host.
	# You can host several worlds on even commodity hardware, but 
	# this is only reccomended for larger servers.
	#
	# Small servers can stick to a single-world setup and be fine.
	worlds = [
		# Define one world. This will be hosted on :5126
		( world_path = "worlds/test.wrl", maxclients = 100 )
	]

);
```

Advantages:
- Fairly easy to work with
- Doesn't require consuming another library



Disadvantages:
- Needs importing capnp
	- though for plans I have I'll have to do this anyways so this is more of a thing than a problem
- capnp text format may be unfamilar
	- it is fairly easy to read though
