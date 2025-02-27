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
    name: string
    createdAt: string
    lastMessage?: string
    img?: string
}

export interface IChatPost {
    name: string
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
