export function logger(msg: string, type: string = "INFO") : void {
	console.log("[" + type + "] " + msg)
}

export function sleep(ms: number) : Promise<void> {
	return new Promise(resolve => setTimeout(resolve, ms))
}

