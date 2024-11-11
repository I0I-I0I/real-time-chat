export interface IPacket {
	length: number
	type: string
	body: string
}

export interface IResponse {
	type: string
	msg: string
}

export interface IHandlers {
  message: (msg: string) => IResponse;
  error: (msg: string) => IResponse;
  json: (msg: string) => IResponse;
}
