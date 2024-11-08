import net from "net";
import { logger, sleep } from "./utils/utils.js";

const client = new net.Socket();
const PORT = 8080;
const HOST = "localhost";

async function send_data(type, msg) {
	let send_msg = type + ":" + msg + "\0";
	send_msg = send_msg.length + ":" + send_msg;
	client.write(send_msg);
	logger("Sending: " + send_msg, "MSG");
	await sleep(100);
}

client.connect(PORT, HOST, async function() {
	logger('Connected to -> ' + HOST + ":" + PORT, "CONN");
	await send_data("message", "Hello: Se:rver!");
	await send_data("close", "BYE!");
});

client.on("data", function(data) {
	logger('RECV: ' + data.toString(), "MSG");
});

client.on("close", function() {
	logger("Connection closed", "CLOSE");
	client.destroy();
});
