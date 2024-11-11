import net from "net"

import { logger, parse_data } from "./utils/utils.js"
import { send_set_socket } from "./transfer_data/index.js"
import { response_handler } from "./handlres/handlres.js"

import { opts } from "./globals/consts.js"

import { IPacket, IResponse } from "./globals/types.js"

const client = new net.Socket()
const send_data = send_set_socket(client)

client.connect(opts.port, opts.host, async () : Promise<void> => {
	logger("Connected to -> " + opts.host + ":" + opts.port, "CONN")
	await send_data("message", "Hello: Se:rver!")
	await send_data("message", "get")
	await send_data("close", "BYE!")
})

client.on("data", (data: string) : void => {
	const packet: IPacket = parse_data(data)
	logger("RECV: " + packet.length + ":" + packet.type + ":" + packet.body, "MSG")
	if (packet.type == "close") return

	const response: IResponse = response_handler(packet)
	logger(response.type + ": " + response.msg, "RESP")
	send_data(response.type, response.msg)
})

client.on("close", () : void => {
	logger("Connection closed", "CLOSE")
	client.destroy()
})
