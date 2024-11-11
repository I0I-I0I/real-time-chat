import { IPacket, IResponse, IHandlers } from "../globals/types.js"

const handlers: IHandlers = {
	message: (msg: string) : IResponse => {
		return {
			type: "response",
			msg: "OK"
		}
	},
	error: (msg: string) : IResponse => {
		return {
			type: "response",
			msg: "ERROR"
		}
	},
	json: (msg: string) : IResponse => {
		const json_body = JSON.parse(msg)
		return {
			type: "response",
			msg: "Data received"
		}
	}
}

export function response_handler(packet: IPacket) : IResponse {
	if (packet.type in handlers === false) {
		return {
			type: "error",
			msg: "Wrong packet type"
		}
	}
	return handlers[packet.type as keyof IHandlers](packet.body)
}
