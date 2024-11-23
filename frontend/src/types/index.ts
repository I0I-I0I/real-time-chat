export type MessageType = {
	text: string
	author: string
}

export interface IUser {
	createdAt: string
	id: number
	login: string
	password: string
	username: string
}

export interface IFetchData<T> {
	status: string
	data: T[]
	message: string
}

export interface IPostUser {
	login: string
	password: string
	username: string
}

export interface ICheckUser {
	login: string
	password: string
}

export interface IChat {
	id: number
	messages: MessageType[]
}
