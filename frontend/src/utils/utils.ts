import { IPacket } from "../globals/types.js";

export function logger(msg: string, type: string = "INFO") : void {
	console.log("[" + type + "] " + msg)
}

export function sleep(ms: number) : Promise<void> {
	return new Promise((resolve : () => void) =>
					   setTimeout(resolve, ms))
}

export function parse_data(input: string): IPacket {
    const firstColonIndex = input.indexOf(":");
    const secondColonIndex = input.indexOf(":", firstColonIndex + 1);

	let packet: IPacket = { length: 0, type: "", body: "" }
    packet.length = Number(input.slice(0, firstColonIndex));
    packet.type = input.slice(firstColonIndex + 1, secondColonIndex);
    packet.body = input.slice(secondColonIndex + 1);

    return packet
}

