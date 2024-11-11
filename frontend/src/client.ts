import net from "net"
import { logger, parseString as parse_data, sleep } from "./utils/utils.js"

const client = new net.Socket()
const PORT = 8080
const HOST = "localhost"

async function send_data(type: string, msg: string) : Promise<void> {
	let send_msg = type + ":" + msg
	send_msg = send_msg.length + ":" + send_msg
	client.write(send_msg)
	logger("SEND: " + send_msg, "MSG")
	await sleep(100)
}

client.connect(PORT, HOST, async function() : Promise<void> {
	logger("Connected to -> " + HOST + ":" + PORT, "CONN")

	await send_data("message", "Hello: Se:rver!")

	await send_data("message", "get")

	await send_data("close", "BYE!")
})

client.on("data", function(data: string) : void {
	const [_, type, body] = parse_data(data)

	let log_type = "MSG"
	if (type == "json") {
		send_data("message", "ACK")
		const json_body = JSON.parse(body)
		log_type = "DATA"
	}
	logger("RECV: " + data, log_type)
})

client.on("close", function() : void {
	logger("Connection closed", "CLOSE")
	client.destroy()
})
