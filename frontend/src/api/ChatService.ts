import { DB_URL } from "@/consts"
import { IFetchData, IChatPost, IChat } from "@/types"

const URL = DB_URL + "/chats"

export default class ChatService {
    static async getById(id: number): Promise<IChat | null> {
        const resp = await fetch(URL + "?id=" + id, {
            method: "GET"
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status !== "OK") {
            return null
        }
        return data.data[0]
    }

    static async getAll(): Promise<IChat[] | null> {
        const resp = await fetch(URL, {
            method: "GET",
            headers: {
                "Connection": "close"
            }
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status !== "OK") {
            return null
        }
        return data.data
    }

    static async createOne(post_data: IChatPost): Promise<string | null>  {
        const resp = await fetch(URL, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                name: post_data.name,
                last_message: post_data?.lastMessage,
            }])
        })
        const data = await resp.json() as IFetchData<IChat>
        if (data.status === "OK") {
            return data.status
        }
        return null
    }
}
