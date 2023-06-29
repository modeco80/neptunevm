# Neptuned HTTP API

This is essentially a draft of what the API for talking to Neptuned would look like.

# `/api/v0` (version 0 API)

# Objects

## CmnHvKind

Hypervisor kind enumerattion.

```ts
type CmnHvKind = "qemu";
```


## LoginRequest

```ts
type LoginRequest = {
	"username": string,
	"password_hash": string // password hash.
}
```

## LoginResponse

```ts
type LoginResponse = {
	// error
	"error": string | undefined,

	// valid
	"token": string | undefined // JWT token
}
```

## RegisterRequest

```ts
type RegisterRequest = {
	"username": string,
	"password": string
}
```

## RegisterResponse

```ts
type RegisterResponse = {
	// error
	"error": string | undefined,
	"token": string | undefined, // newly issued JWT token.
}
```

## AdminUserEntry

```ts
type AdminUserEntry = {
	"id": string, // Snowflake of user
	"user_type": "user" | "moderator" | "administrator",
	"banned": boolean,
	"ban_reason": string | undefined
}
```

## ListVMEntry

A VM list entry.

```ts
type ListVMEntry = {
	"id": string,
	"name": string,
	"info": {
		"hypervisor": CmnHvKind,

		"os": {
			"kind": "windows" | "macos" | "linux" | "other",
			"release": string | undefined,
			"arch": "x86" | "x86_64" | string
		}, 
		"disk": [
			{
				"type": "hdd" | "ssd",
				"size": number
			}
		],
		"graphics": {
			"type": "vga" | "burst",
			"vram_size": number | undefined //(if type != "burst")
		}
	}

}
```


## ConfigServerSettings

```ts
type ConfigServerSettings = {
	"registrations_open": boolean, // if false, registering will not be allowed
	"chat": {
		"messages_max_rate": number, // seconds
		"messages_max_count": number, // count
	}

}
```

## ConfigVMEntry

A VM entry used for configuration.

```ts
type ConfigVMEntry = {
	"id": string
	"name": string,
	"hypervisor": CmnHvKind,
	"autostart": boolean, // if true, the VM will autostart (upon creation) when the Neptuned process is launched

	"private": boolean, // if true, then this VM will show up in the list, but have a dummy list screenshot, and require a password to enter.

	// if hypervisor == "qemu"
	"qemu_use_uefi": boolean, // true == use UEFI
	"qemu_enable_agent": boolean, // true == add IVSHMEM device. Only works on q35, server will reject pc machines which turn this on, and reject patches to turn it on on a PC machine.
	"qemu_uuid": string,
	"qemu_machine_type": "pc" | "q35",
	"qemu_machine_version": string,
	"qemu_devices": [ ConfigQemuDevice ]

}
```

## ConfigQemuDevice

A QEMU device. NeptuneVM tries to abstract the QEMU command line into this device model for a couple reasons:

- There generally isn't much good that NeptuneVM doesn't expose, and for any case there is anything like that, you can use the `custom_cmd` device
- Most command line options that *aren't* exposed by NeptuneVM probably aren't exposed for a good reason. For instance, `-daemonize` would be a bad idea to put in to the command line, since it **will** break NeptuneVM supervising QEMU horribly.


```ts
type ConfigQemuDevice = {
	"id": string, // ID of device. Can be referenced by other device model devices.
	"type": "cpu" | "drive" | "hba" | "audio" | "display" | "network" | "nic" | "custom_cmd",

	// if type == "cpu"
	"cpu_model": string,
	"cpu_features": [ string ],
	"cpu_cores": number,

	// if type == drive
	"drive_type": "hdd" | "ssd" | "cdrom",
	"drive_cache_type": string,
	"drive_use_io_uring": boolean,
	"drive_bus": "ide" | "sata" | "scsi",

	"drive_bps_limit": string, // example: "15000000 25000000" (15 MB/s max, 25 MB/s burst)
	"drive_iops_limit": string // example: "1000 1500" (1000 iops, 1500 burst)

	// if drive_type == hdd | ssd
	"drive_image": string,
	"drive_format": string | undefined,

	// if type == hba
	"hba_type": "virtio-scsi" | "lsi",
	// if hba_type == virtio-scsi
	"hba_add_iothread": boolean,
	"hba_num_queues": number,

	// if type == audio
	"audio_type": "hda" | "ac97" | "sb16",
	
	// if type == display
	"display_type": "vga" | "qxl" | "cirrus" | "mdev",
	// if display_type == mdev
	"display_mdev_profile": string,
	// else
	"display_vgamem_mb": number,


	// if type == network
	"network_type": "user" | "tap",
	// if network_type == tap
	"tap_interface_name": string,
	"tap_use_vhost": boolean,

	// if type == nic
	"nic_type": "rtl8139" | "pcnet" | "e1000" | "virtio",
	"nic_mac": string,
	"nic_netdev": string, // Device Model id of previously created network device to attach

	// if type == custom_cmd
	"command_line": string
	
}
```

# Endpoints

# General

### GET `/list`

Returns a [ListEntry](#listentry) array of all running virtual machines, and information on them.

### GET `/list/{vm}`

Like GET `/list`, however returns a single ListEntry object for a running virtual machine, or a error if the VM is not running or doesn't exist.

### GET `/screenshot/{vm}`

Returns a minimized PNG screenshot of the given virtual machine, or a error if the VM is not running or doesn't exist. This is internally updated by the server at about 2 frames per second.

### GET (WebSockets) `/connect/{vm}`

Begins handshaking a WebRTC connection to the server, using WebSockets as a signaling mechanism, from client to server

This API call can return an error (in the WebSocket handshake, not opening the connection) in the following cases:

- The VM does not exist
- You are not logged into NeptuneVM
- You have been banned from using NeptuneVM

Once WebRTC connection has succedded, the connection to this WebSocket can safely be dropped.

# Authentication

### POST `/login`

Login to NeptuneVM.

The POST body is a LoginRequest, and the response will be a LoginResponse

### POST `/register`

Register an account. Will fail if registrations are not open.

### POST `/logout`

Log out, revoking this session token.

### POST `/change_password`

Change this logged in account's password. This will revoke all sessions except for the one changing the password.

### POST `/delete_account`

Delete the logged-in account. This will

- Destroy all sessions
- Revoke all tokens
- Remove all traces of the account from the database.

# Admin/Config

All Admin APIs require a valid admin authentication token be provided.

### GET `/admin/config`

Returns the current Neptuned settings.

### PATCH `/admin/config`

Updates the Neptuned settings. The body should be a [AdminConfig](#adminconfig) object.

## Users

### GET `/admin/users`

Returns a list of [AdminUserEntry](#adminuserentry) objects, for all registered users.

### GET `/admin/users/{user_id}`

Returns a single [AdminUserEntry](#adminuserentry) object for the given user ID.

### POST `/admin/users/create`

Creates a user. The body must be a [AdminUserEntry](#adminuserentry), with the following extended fields:

```ts
type ExtendedAdminUserEntry = AdminUserEntry | {
	"password": string, // password to assign
}
```

### POST `/admin/users/{user_id}/change_type`

Change the type of this user. Body:

```json
{
	"type": "user" | "moderator" | "administrator"
}
```

### POST `/admin/users/{user_id}/change_password`

Change the password for a given user. 
Body:

```json
{
	"password": string
}
```

This reacts like `/change_password`, but will revoke all sessions.

### DELETE `/admin/users/{user_id}`
Delete the user with the given username. This performs basically the same thing as `/delete_account`, but is always accessible by any administrator.

### POST `/admin/users/{user_id}/ban`

Ban a user from NeptuneVM. This does not delete the account, because the user can optionally be unbanned afterwards. (also, the user can also delete their account)

Optional body:

```json
{
	"reason": string, // The reason the user was banned (will be saved in the database and displayed to the user)
}
```

### POST `/admin/users/{user_id}/unban`

Unban a previously banned user.

## Virtual Machines

### GET `/admin/vms/list`

Returns a list of [ConfigVMEntry](#configvmentry) objects for all VMs in the database.

The entry objects will have the following extended fields:

```json
{
	"state": "stopped" | "starting" | "running"
}
```

### GET `/admin/vms/{vm}`

Returns a singular [ConfigVMEntry](#configvmentry) for a VM in the database. Also adds extended fields like `/admin/vms/list`.

### POST `/admin/vms/create`

Create a VM. The POST body should be a [ConfigVMEntry](#configvmentry).

### PATCH `/admin/vms/{vm}`

Updates the given VM's configuration. This expects a valid [JSON Patch](https://datatracker.ietf.org/doc/html/rfc6902/) document to be provided to update the VM configuration with. 

Any errors applying the patch will roll back the configuration and send an error response.

Successful patch will respond with the following body:

```json
{
	"needs_restart": boolean // false == all changes were appliable at runtime
							// true == changes may need a full vm process restart
}
```

### DELETE `/admin/vms/{vm}`

Delete the VM with the given ID. Returns an error if the VM does not exist.
