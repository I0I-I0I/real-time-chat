import { Socket } from "net"
import { logger, sleep } from "../utils/utils.js"

export function send_set_socket(client: Socket) : (type: string, msg: string) => Promise<void> {
	return async (type: string, msg: string) : Promise<void>  => {
		let send_msg = type + ":" + msg
		send_msg = send_msg.length + ":" + send_msg
		client.write(send_msg)
		logger("SEND: " + send_msg, "MSG")
		await sleep(100)
	}
}

