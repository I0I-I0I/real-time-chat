import net from "net"
import { logger, sleep } from "./utils/utils.js"

const client = new net.Socket()
const PORT = 8080
const HOST = "localhost"

async function send_data(type: string, msg: string) : Promise<void> {
	let send_msg = type + ":" + msg + "\0"
	send_msg = send_msg.length + ":" + send_msg
	client.write(send_msg)
	logger("Sending: " + send_msg, "MSG")
	await sleep(100)
}

client.connect(PORT, HOST, async function() : Promise<void> {
	logger("Connected to -> " + HOST + ":" + PORT, "CONN")
	await send_data("message", "Hello: Se:rver!\n")
	await send_data("message", "How are u?\n")
	await send_data("close", "BYE!\n")
})

client.on("data", function(data: string) : void {
	logger("RECV: " + data.toString(), "MSG")
})

client.on("close", function() : void {
	logger("Connection closed", "CLOSE")
	client.destroy()
})
