## Neptuned config

```toml
[neptuned]
# The IP address to listen on.
# It is reccomended to set this to `::1`
# or `127.0.0.1`.
listen_address = "127.0.0.1"

# The port neptuned will listen for http on.
# WebRTC sessions do not use a fixed port.
listen_port = 5126

# This is where VM data (images, firmware files) will be put
vm_data_dir = "/srv/neptune/"

# Path to the JWT secret data to use.
# This file should be kept secret.
jwt_secret_path = "/etc/neptune/jwt_secret.bin"

# This defines the MariaDB database connection
# `unix:` uses unix sockets @ the default MariaDB path
# A functioning MariaDB database is required to host NeptuneVM
[neptuned.database]
connection = "unix:"
username = "neptune"
password = "neptuneSecret"
database_name = "neptune"

# WebRTC configuration
[neptuned.webrtc]
# STUN servers to use
stun_servers = [
	"stun.l.google.com:19302"
]
```
