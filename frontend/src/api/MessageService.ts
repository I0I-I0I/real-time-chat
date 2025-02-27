import { DB_URL } from "@/consts"
import { IFetchData, IMessage } from "@/types"

const URL = DB_URL + "/messages"

export default class MessageService {
    static async getById(id: number, chatId: number): Promise<IMessage | null> {
        const resp = await fetch(URL + "?chat_id=" + chatId + "?id=" + id, {
            method: "GET"
        })
        const data = await resp.json() as IFetchData<IMessage>
        if (data.status !== "OK") {
            return null
        }
        return data.data[0]
    }

    static async getAll(chatId: number): Promise<IMessage[] | null> {
        const resp = await fetch(URL + "?chat_id=" + chatId, {
            method: "GET",
        })
        const data = await resp.json() as IFetchData<IMessage>
        if (data.status !== "OK") {
            return null
        }
        return data.data
    }

    static async createOne(post_data: IMessage): Promise<string | null>  {
        const resp = await fetch(URL, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                chatId: post_data.id,
                authorId: post_data.authorId,
                text: post_data.text,
            }])
        })
        const data = await resp.json() as IFetchData<IMessage>
        if (data.status === "OK") {
            return data.status
        }
        return null
    }
}
