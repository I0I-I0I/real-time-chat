export interface IUser {
    createdAt: string
    id: number
    login: string
    password: string
    username: string
    hash: string
}

export interface IFetchData<T> {
    status: number
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
    name: string
    name2: string
    createdAt: string
    lastMessage?: IMessage
    img?: string
    type: "group" | "friend"
}

export interface IChatPost {
    name: string
    name2: string
    lastMessage?: string
}

export interface IMessage {
    id: number
    chatId: number
    authorId: number
    body: string
    createdAt: string
}

export interface IMessagePost {
    chatId: number
    authorId: number
    body: string
}
