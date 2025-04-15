import { DB_URL } from "@/consts"
import { IFetchData, IChatPost, IChat } from "@/types"

const URL = DB_URL + "/chats"

export default class ChatService {
    static async getById(id: number): Promise<IChat | null> {
        const resp = await fetch(URL + "?id=" + id, {
            method: "GET"
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status !== 200) {
            return null
        }
        return data.data[0]
    }

    static async getAll(id: number): Promise<IChat[] | null> {
        const resp = await fetch(URL + "?userId=" + id, {
            method: "GET",
        })
        if (resp.status !== 200) return null
        const data = await resp.json() as IFetchData<IChat>
        if (data.status !== 200) {
            return null
        }
        return data.data
    }

    static async createOne(post_data: IChatPost, user_id: number): Promise<IChat | null>  {
        const resp = await fetch(URL + "?userId=" + user_id, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                name: post_data.name,
                name2: post_data.name2,
                lastMessageId: post_data?.lastMessage,
            }])
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status === 200) {
            return data.data[0]
        }
        return null
    }

    static async addUserToChat(chat_id: number, user_id: number): Promise<number | null>  {
        const resp = await fetch(URL + "?type=addParticipants", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                userId: "" + user_id,
                chatId: "" + chat_id
            }])
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status === 200) {
            return data.status
        }
        return null
    }

    static async removeOne(id: number): Promise<number | null>  {
        const resp = await fetch(URL + "?id=" + id, {
            method: "DELETE",
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status === 200) {
            return data.status
        }
        return null
    }
}
