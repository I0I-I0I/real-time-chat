export function logger(msg: string, type: string = "INFO") : void {
	console.log("[" + type + "] " + msg)
}

export function sleep(ms: number) : Promise<void> {
	return new Promise(resolve => setTimeout(resolve, ms))
}

export function parseString(input: string) : string[] {
    const firstColonIndex = input.indexOf(":");
    const secondColonIndex = input.indexOf(":", firstColonIndex + 1);
    const length = input.slice(0, firstColonIndex);
    const type = input.slice(firstColonIndex + 1, secondColonIndex);
    const body = input.slice(secondColonIndex + 1);
    return [length, type, body];
}

