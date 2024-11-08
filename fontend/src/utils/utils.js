export function logger(msg, type = "INFO") {
	console.log("[" + type + "] " + msg);
}

export function sleep(ms) {
	return new Promise(resolve => setTimeout(resolve, ms));
}

